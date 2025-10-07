#include "service/professorService.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "event/events.hpp"
using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---

// Define o nome da "tabela" (arquivo CSV) gerenciada por este Service.
#define PROFESSOR_TABLE "professores"
// Índices das colunas usados para buscas específicas.
#define EMAIL_COL_INDEX 2

// Construtor: Inicializa todas as dependências de forma segura (Injeção de
// Dependência).
ProfessorService::ProfessorService(const MockConnection& connection,
                                   const EventBus& bus,
                                   const ProfessorMapper& mapper,
                                   const HorarioService& service)
    : connection(connection), bus(bus), mapper(mapper), service(service) {}

// --- MÉTODOS PRIVADOS/AUXILIARES ---

// Helper: Converte CSV em Model e injeta Horarios disponíveis (Responsabilidade
// do Service)
Professor ProfessorService::mapAndInjectHorarios(const string& csv_line) const {
    Professor professor;

    // 1. Mapeamento CSV -> Model (Mapper lança erro se o formato dos dados for
    // inválido).
    try {
        professor = mapper.fromCsvLine(csv_line);
    } catch (const invalid_argument& e) {
        // Promove o erro de mapeamento para runtime_error (indicando falha na
        // integridade do registro).
        throw runtime_error("Falha ao processar registro CSV: " +
                            string(e.what()));
    }

    // 2. Injeção de Horarios (Consulta a serviço externo/outra entidade)
    // Busca a lista de horários disponíveis do professor por meio do
    // HorarioService.
    vector<Horario> horarios =
        service.listDisponivelByIdProfessor(professor.getId());
    // Usa std::move para transferência eficiente do vetor de Horarios.
    professor.setHorarios(move(horarios));

    return professor;
}

// Helper: Busca todos os Professors por Email (usa o DAL e mapeia o resultado).
vector<Professor> ProfessorService::getByEmail(const string& email) const {
    vector<string> csv_matches;
    vector<Professor> professors;

    try {
        // Chamada ao DAL (MockConnection) para buscar na tabela
        // PROFESSOR_TABLE.
        csv_matches =
            connection.selectByColumn(PROFESSOR_TABLE, EMAIL_COL_INDEX, email);
    } catch (const invalid_argument& e) {
        // Captura "Nenhum registro encontrado" do DAL e traduz para um vetor
        // vazio.
        return {};
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O críticos.
    }

    // Mapeia todas as linhas CSV válidas para Model.
    for (const string& csv_line : csv_matches) {
        try {
            professors.push_back(mapAndInjectHorarios(csv_line));
        } catch (const runtime_error& e) {
            // Logs e ignora registros malformados encontrados no arquivo.
            cerr << "Aviso: Pulando registro malformado durante getByEmail: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return professors;
}

// --- MÉTODOS DE VALIDAÇÃO DE NEGÓCIO ---

bool ProfessorService::existsByEmail(string email) const {
    // Verifica se a lista retornada por getByEmail não está vazia.
    return !getByEmail(email).empty();
}

bool ProfessorService::existsByEmailAndIdNot(string email, long id) const {
    vector<Professor> email_matches = getByEmail(email);

    // Itera os Models para verificar se o email pertence a um ID diferente.
    for (const Professor& professor : email_matches) {
        if (professor.getId() != id) {
            return true;  // Encontrado outro professor com o mesmo email.
        }
    }

    return false;
}

// --- MÉTODOS PÚBLICOS (CRUD) ---

// CREATE
Professor ProfessorService::save(const ProfessorDTO& professor) const {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade de Email)
    if (existsByEmail(professor.getEmail())) {
        throw invalid_argument("O email '" + professor.getEmail() +
                               "' já está em uso por outro professor.");
    }

    // 2. DAL WRITE
    string data_csv = mapper.toCsvData(professor);
    // Persiste os dados e obtém o novo ID (lança runtime_error em I/O).
    long id = connection.insert(PROFESSOR_TABLE, data_csv);

    // 3. MONTAGEM DO MODELO (Evita leitura desnecessária do DB)
    string new_record_csv = to_string(id) + "," + data_csv;

    try {
        // Mapeia, injeta Horarios e retorna o Model completo.
        return mapAndInjectHorarios(new_record_csv);
    } catch (const runtime_error& e) {
        // Erro crítico após sucesso na escrita: relança com contexto.
        throw runtime_error(
            "Inserção bem-sucedida (ID: " + to_string(id) +
            "), mas falha ao processar o registro: " + string(e.what()));
    }
}

// READ BY ID
optional<Professor> ProfessorService::getById(long id) const {
    try {
        // selectOne lança invalid_argument se ID não existe.
        string csv_line = connection.selectOne(PROFESSOR_TABLE, id);

        // Se encontrou, mapeia e retorna o optional<Professor> com valor.
        return mapAndInjectHorarios(csv_line);
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" e retorna optional vazio.
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O.
    }
}

// READ BY EMAIL
optional<Professor> ProfessorService::getOneByEmail(const string& email) const {
    vector<Professor> results = getByEmail(email);
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
vector<Professor> ProfessorService::listAll() const {
    // Pega todas as linhas de dados da tabela Professores.
    vector<string> csv_records = connection.selectAll(PROFESSOR_TABLE);
    vector<Professor> professors;

    // Mapeia e injeta Horarios para todos os registros.
    for (const string& csv_line : csv_records) {
        try {
            professors.push_back(mapAndInjectHorarios(csv_line));
        } catch (const runtime_error& e) {
            // Logs e ignora registros malformados.
            cerr << "Aviso: Pulando registro malformado durante listAll: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return professors;
}

// UPDATE
optional<Professor> ProfessorService::updateById(
    long id, const ProfessorDTO& professor) const {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade para UPDATE, excluindo o próprio ID)
    if (existsByEmailAndIdNot(professor.getEmail(), id)) {
        throw invalid_argument("O email '" + professor.getEmail() +
                               "' já está em uso por outro professor.");
    }

    // 2. DAL WRITE (Atualiza a linha no arquivo)
    string data_csv = mapper.toCsvData(professor);
    string updated_csv;

    try {
        // update() lança invalid_argument se ID não existe.
        connection.update(PROFESSOR_TABLE, id, data_csv);

        // Constrói a linha CSV completa do registro atualizado.
        updated_csv = to_string(id) + "," + data_csv;
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" e retorna optional vazio.
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O.
    }

    // 3. MONTAGEM DO MODELO
    Professor updated_professor = mapAndInjectHorarios(updated_csv);

    // 4. PUBLICAÇÃO DE EVENTO (Sucesso no Update)
    // Cria o shared_ptr para o objeto e publica.
    shared_ptr<Usuario> user_ptr = make_shared<Professor>(updated_professor);
    bus.publish(UsuarioUpdatedEvent(user_ptr));

    return updated_professor;
}

// DELETE
bool ProfessorService::deleteById(long id) const {
    try {
        // 1. DAL WRITE
        // deleteRecord() lança invalid_argument se ID não existe.
        connection.deleteRecord(PROFESSOR_TABLE, id);

        // 2. PUBLICAÇÃO DE EVENTO (Sucesso no Delete)
        // Publica o ID do usuário deletado.
        bus.publish(ProfessorDeletedEvent(id));

        return true;  // Sucesso na remoção.
    } catch (const invalid_argument& e) {
        return false;  // ID não encontrado.
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O.
    }
}