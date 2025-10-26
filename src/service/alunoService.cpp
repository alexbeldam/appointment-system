#include "service/alunoService.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "event/events.hpp"

using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---

// Define o nome da "tabela" (arquivo CSV) gerenciada por este Service.
#define ALUNO_TABLE "alunos"
// Índices das colunas usados para buscas específicas.
#define EMAIL_COL_INDEX 2
#define MATRICULA_COL_INDEX 4

// Construtor: Inicializa todas as dependências de forma segura (Injeção de
// Dependência). A ordem é crucial para membros const e referências.
AlunoService::AlunoService(const MockConnection& connection, EventBus& bus,
                           const AgendamentoService& service)
    : connection(connection), bus(bus), service(service) {}

// --- MÉTODOS PRIVADOS/AUXILIARES ---

// Helper: Converte CSV em Model e injeta as dependências de outras entidades.
Aluno AlunoService::mapAndInjectAgendamentos(const string& csv_line) const {
    Aluno aluno;

    stringstream ss(csv_line);
    string item;
    vector<string> tokens;

    while (getline(ss, item, ',')) {
        tokens.push_back(item);
    }

    if (tokens.size() < 5) {
        throw runtime_error("Registro CSV inválido: " + csv_line);
    }

    aluno.setId(stol(tokens[0]));
    aluno.setNome(tokens[1]);
    aluno.setEmail(tokens[2]);
    aluno.setSenha(tokens[3]);
    aluno.setMatricula(stol(tokens[4]));

    vector<Agendamento> agendamentos = service.listByIdAluno(aluno.getId());

    aluno.setAgendamentos(move(agendamentos));

    return aluno;
}

// Helper: Busca todos os Alunos por Email.
vector<Aluno> AlunoService::getByEmail(const string& email) const {
    vector<string> csv_matches;
    vector<Aluno> alunos;

    try {
        // Chamada ao DAL (MockConnection) para buscar na tabela ALUNO_TABLE.
        csv_matches =
            connection.selectByColumn(ALUNO_TABLE, EMAIL_COL_INDEX, email);
    } catch (const invalid_argument& e) {
        // Se o DAL lançar 'invalid_argument' por não encontrar nada, traduz
        // para um vetor vazio.
        return {};
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O críticos do DAL.
    }

    // Mapeia todas as linhas CSV válidas para Model.
    for (const string& csv_line : csv_matches) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            // Loga e ignora registros malformados encontrados no arquivo.
            cerr << "Aviso: Pulando registro malformado durante getByEmail: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

// Helper: Busca todos os Alunos por Matrícula.
vector<Aluno> AlunoService::getByMatricula(long matricula) const {
    vector<string> csv_matches;
    vector<Aluno> alunos;
    string sMatricula =
        to_string(matricula);  // Converte para string para o DAL.

    try {
        csv_matches = connection.selectByColumn(
            ALUNO_TABLE, MATRICULA_COL_INDEX, sMatricula);
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

// --- MÉTODOS DE VALIDAÇÃO DE NEGÓCIO (Unicidade) ---

bool AlunoService::existsByEmail(string email) const {
    // Verifica se a busca retorna algum resultado.
    return !getByEmail(email).empty();
}

bool AlunoService::existsByEmailAndIdNot(string email, long id) const {
    vector<Aluno> email_matches = getByEmail(email);

    // Itera os Models para verificar se o email pertence a um ID diferente.
    for (const Aluno& aluno : email_matches) {
        if (aluno.getId() != id) {
            return true;
        }
    }

    return false;
}

bool AlunoService::existsByMatricula(long matricula) const {
    // Verifica se a busca retorna algum resultado.
    return !getByMatricula(matricula).empty();
}

bool AlunoService::existsByMatriculaAndIdNot(long matricula, long id) const {
    vector<Aluno> matricula_matches = getByMatricula(matricula);

    // Itera os Models para verificar se a matrícula pertence a um ID diferente.
    for (const Aluno& aluno : matricula_matches) {
        if (aluno.getId() != id) {
            return true;
        }
    }

    return false;
}

// --- MÉTODOS PÚBLICOS (CRUD) ---

// CREATE
Aluno AlunoService::save(const std::string& nome, const std::string& email,
                         const std::string& senha, long matricula) const {
    if (existsByEmail(email)) {
        throw invalid_argument("O email '" + email +
                               "' já está em uso por outro aluno.");
    }
    if (existsByMatricula(matricula)) {
        throw invalid_argument("A matrícula '" + to_string(matricula) +
                               "' já está registrada.");
    }

    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << matricula;
    long id = connection.insert(ALUNO_TABLE, dados.str());

    string new_record_csv = to_string(id) + "," + dados.str();

    try {
        return mapAndInjectAgendamentos(new_record_csv);
    } catch (const runtime_error& e) {
        throw runtime_error(
            "Inserção bem-sucedida (ID: " + to_string(id) +
            "), mas falha ao processar o registro: " + string(e.what()));
    }
}

// READ BY ID
optional<Aluno> AlunoService::getById(long id) const {
    try {
        // selectOne lança invalid_argument se ID não existe.
        string csv_line = connection.selectOne(ALUNO_TABLE, id);

        // Se encontrou, mapeia e retorna o optional<Aluno> com valor.
        return mapAndInjectAgendamentos(csv_line);
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" do DAL e retorna optional vazio.
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O.
    }
}

// READ BY EMAIL
optional<Aluno> AlunoService::getOneByEmail(const string& email) const {
    vector<Aluno> results = getByEmail(email);
    if (results.size() > 1) {
        // ERRO CRÍTICO: Integridade violada!
        throw runtime_error(
            "Falha de integridade: Múltiplos registros encontrados para o "
            "email: " +
            email);
    }

    if (results.empty()) {
        return nullopt;
    }

    return results.front();
}

// LIST ALL
vector<Aluno> AlunoService::listAll() const {
    // Pega todas as linhas de dados da tabela Alunos.
    vector<string> csv_records = connection.selectAll(ALUNO_TABLE);
    vector<Aluno> alunos;

    // Mapeia e injeta Agendamentos para todos os registros.
    for (const string& csv_line : csv_records) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            // Logs e ignora registros malformados.
            cerr << "Aviso: Pulando registro malformado durante listAll: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

// UPDATE
optional<Aluno> AlunoService::updateById(long id, const std::string& nome,
                                         const std::string& email,
                                         const std::string& senha,
                                         long matricula) const {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade para UPDATE, excluindo o próprio ID)
    if (existsByEmailAndIdNot(email, id)) {
        throw invalid_argument("O email '" + email +
                               "' já está em uso por outro aluno.");
    }
    if (existsByMatriculaAndIdNot(matricula, id)) {
        throw invalid_argument("A matrícula '" + to_string(matricula) +
                               "' já está registrada por outro aluno.");
    }

    // 2. DAL WRITE (Atualiza a linha no arquivo)
    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << matricula;
    string updated_csv;

    try {
        // update() lança invalid_argument se ID não existe.
        connection.update(ALUNO_TABLE, id, dados.str());

        // Constrói a linha CSV completa do registro atualizado.
        updated_csv = to_string(id) + "," + dados.str();
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" e retorna optional vazio.
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O.
    }

    // 3. MONTAGEM DO MODELO
    Aluno updated_aluno = mapAndInjectAgendamentos(updated_csv);

    // 4. PUBLICAÇÃO DE EVENTO (Sucesso no Update)
    // Cria o shared_ptr para o objeto e publica.
    shared_ptr<Usuario> user_ptr = make_shared<Aluno>(updated_aluno);
    bus.publish(UsuarioUpdatedEvent(user_ptr));

    return updated_aluno;
}

// DELETE
bool AlunoService::deleteById(long id) const {
    try {
        // 1. DAL WRITE
        // deleteRecord() lança invalid_argument se ID não existe.
        connection.deleteRecord(ALUNO_TABLE, id);

        // 2. PUBLICAÇÃO DE EVENTO (Sucesso no Delete)
        // Publica o ID do usuário deletado.
        bus.publish(AlunoDeletedEvent(id));

        return true;  // Sucesso na remoção.
    } catch (const invalid_argument& e) {
        return false;  // ID não encontrado.
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O.
    }
}