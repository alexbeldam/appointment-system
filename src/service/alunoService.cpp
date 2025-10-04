#include "service/alunoService.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "helper/mock.hpp"
using namespace std;

// Definições de macros
#define ALUNO_FILE "data/alunos.csv"
#define EMAIL_COL_INDEX 2
#define MATRICULA_COL_INDEX 4

// Construtor: Realiza a Injeção de Dependência das referências constantes
AlunoService::AlunoService(const AlunoMapper& mapper,
                           const AgendamentoService& service)
    : mapper(mapper), service(service) {}

// --- MÉTODOS PRIVADOS/AUXILIARES ---

// Converte uma linha CSV em Model e injeta Agendamentos (Responsabilidade do
// Service)
Aluno AlunoService::mapAndInjectAgendamentos(const string& csv_line) const {
    Aluno aluno;

    // 1. Mapeamento CSV -> Model (Mapper lança invalid_argument em caso de
    // dados malformados)
    try {
        aluno = mapper.fromCsvLine(csv_line);
    } catch (const invalid_argument& e) {
        // Promove o erro de mapeamento para runtime_error (indicando falha
        // crítica no registro)
        throw runtime_error("Falha ao processar registro CSV: " +
                            string(e.what()));
    }

    // 2. Injeção de Agendamentos (Consulta a serviço externo/outra entidade)
    // NOTE: AgendamentoService::listByIdAluno é a dependência injetada.
    vector<Agendamento> agendamentos = service.listByIdAluno(aluno.getId());
    // Usa std::move para transferência eficiente do vetor de Agendamentos
    aluno.setAgendamentos(move(agendamentos));

    return aluno;
}

// Helper: Busca todos os Alunos por Email (usa o DAL e mapeia o resultado)
vector<Aluno> AlunoService::getByEmail(const string& email) const {
    vector<string> csv_matches;
    vector<Aluno> alunos;

    try {
        // Chamada ao DAL. findByColumn retorna vetor vazio ou lança
        // runtime_error (I/O)
        csv_matches = findByColumn(ALUNO_FILE, EMAIL_COL_INDEX, email);
    } catch (const invalid_argument& e) {
        // Captura "Nenhum registro encontrado" do DAL e traduz para um vetor
        // vazio
        return {};
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O críticos
    }

    // Mapeia todas as linhas CSV válidas para Model
    for (const string& csv_line : csv_matches) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            // Logs e ignora registros malformados encontrados
            cerr << "Aviso: Pulando registro malformado durante getByEmail: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

// Helper: Busca todos os Alunos por Matrícula (funciona de forma idêntica ao
// getByEmail)
vector<Aluno> AlunoService::getByMatricula(long matricula) const {
    vector<string> csv_matches;
    vector<Aluno> alunos;
    string sMatricula =
        to_string(matricula);  // Converte matrícula para string para o DAL

    try {
        csv_matches = findByColumn(ALUNO_FILE, MATRICULA_COL_INDEX, sMatricula);
    } catch (const invalid_argument& e) {
        return {};
    } catch (const runtime_error& e) {
        throw;
    }

    for (const string& csv_line : csv_matches) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            cerr
                << "Aviso: Pulando registro malformado durante getByMatricula: "
                << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

// --- MÉTODOS DE VALIDAÇÃO DE NEGÓCIO ---

bool AlunoService::existsByEmail(string email) const {
    // Verifica se a lista retornada por getByEmail não está vazia
    return !getByEmail(email).empty();
}

bool AlunoService::existsByEmailAndIdNot(string email, long id) const {
    vector<Aluno> email_matches = getByEmail(email);

    // Itera os Models e compara o ID para verificar unicidade
    for (const Aluno& aluno : email_matches) {
        if (aluno.getId() != id) {
            return true;  // Encontrado outro aluno com o mesmo email
        }
    }

    return false;
}

bool AlunoService::existsByMatricula(long matricula) const {
    // Verifica se a lista retornada por getByMatricula não está vazia
    return !getByMatricula(matricula).empty();
}

bool AlunoService::existsByMatriculaAndIdNot(long matricula, long id) const {
    vector<Aluno> matricula_matches = getByMatricula(matricula);

    // Itera os Models e compara o ID para verificar unicidade
    for (const Aluno& aluno : matricula_matches) {
        if (aluno.getId() != id) {
            return true;  // Encontrado outro aluno com a mesma matrícula
        }
    }

    return false;
}

// --- MÉTODOS PÚBLICOS (CRUD) ---

// CREATE
Aluno AlunoService::save(const AlunoDTO& aluno) const {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade)
    if (existsByEmail(aluno.getEmail())) {
        throw invalid_argument("O email '" + aluno.getEmail() +
                               "' já está em uso por outro aluno.");
    }
    if (existsByMatricula(aluno.getMatricula())) {
        throw invalid_argument("A matrícula '" +
                               to_string(aluno.getMatricula()) +
                               "' já está registrada.");
    }

    // 2. DAL WRITE
    string data_csv = mapper.toCsvData(aluno);
    long id = insert(ALUNO_FILE, data_csv);  // Persiste os dados e obtém o novo
                                             // ID (lança runtime_error em I/O)

    // 3. MONTAGEM DO MODELO (Evita leitura desnecessária do DB)
    string new_record_csv = to_string(id) + "," + data_csv;

    try {
        // Mapeia e injeta Agendamentos (lança runtime_error em mapeamento)
        return mapAndInjectAgendamentos(new_record_csv);
    } catch (const runtime_error& e) {
        // Erro crítico após sucesso na escrita: relança com contexto
        throw runtime_error(
            "Inserção bem-sucedida (ID: " + to_string(id) +
            "), mas falha ao processar o registro: " + string(e.what()));
    }
}

// READ BY ID
optional<Aluno> AlunoService::getById(long id) const {
    try {
        // find() lança invalid_argument se ID não existe, runtime_error se I/O
        // falhar
        string csv_line = find(ALUNO_FILE, id);

        // Se encontrou, mapeia e retorna o optional<Aluno> com valor
        return mapAndInjectAgendamentos(csv_line);
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" e retorna optional vazio
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O
    }
}

// LIST ALL
vector<Aluno> AlunoService::listAll() const {
    vector<string> csv_records =
        findAll(ALUNO_FILE);  // Pega todas as linhas (exclui cabeçalho)
    vector<Aluno> alunos;

    // Mapeia e injeta Agendamentos para todos os registros
    for (const string& csv_line : csv_records) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            // Logs e ignora registros malformados
            cerr << "Aviso: Pulando registro malformado durante listAll: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

// UPDATE
optional<Aluno> AlunoService::updateById(long id, const AlunoDTO& aluno) const {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade para UPDATE, excluindo o próprio ID)
    if (existsByEmailAndIdNot(aluno.getEmail(), id)) {
        throw invalid_argument("O email '" + aluno.getEmail() +
                               "' já está em uso por outro aluno.");
    }
    if (existsByMatriculaAndIdNot(aluno.getMatricula(), id)) {
        throw invalid_argument("A matrícula '" +
                               to_string(aluno.getMatricula()) +
                               "' já está registrada por outro aluno.");
    }

    // 2. DAL WRITE (Atualiza a linha no arquivo)
    string data_csv = mapper.toCsvData(aluno);
    string updated_csv;

    try {
        // update() lança invalid_argument se ID não existe
        update(ALUNO_FILE, id, data_csv);

        // Constrói a linha CSV completa do registro atualizado (ID + novos
        // dados)
        updated_csv = to_string(id) + "," + data_csv;
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" e retorna optional vazio
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O
    }

    // 3. MONTAGEM DO MODELO
    return mapAndInjectAgendamentos(updated_csv);
}

// DELETE
bool AlunoService::deleteById(long id) const {
    try {
        // remove() lança invalid_argument se ID não existe
        remove(ALUNO_FILE, id);
        return true;  // Sucesso na remoção
    } catch (const invalid_argument& e) {
        return false;  // ID não encontrado
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O
    }
}