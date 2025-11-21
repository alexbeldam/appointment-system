#include "service/professorService.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "event/events.hpp"
#include "service/horarioService.hpp"
using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---

// Define o nome da "tabela" (arquivo CSV) gerenciada por este Service.

// Índices das colunas usados para buscas específicas.
#define EMAIL_COL_INDEX 2

// Construtor: Inicializa todas as dependências de forma segura (Injeção de
// Dependência).
ProfessorService::ProfessorService(EntityManager* manager,
                                   const MockConnection& connection,
                                   EventBus& bus)
    : manager(manager),
      connection(connection),
      bus(bus),
      cache({PROFESSOR_TABLE, HORARIO_TABLE, AGENDAMENTO_TABLE}) {}

// --- MÉTODOS PRIVADOS/AUXILIARES ---

// Helper: Busca todos os Professors por Email (usa o DAL e mapeia o resultado).
vector<shared_ptr<Professor>> ProfessorService::getByEmail(
    const string& email) {
    cache.invalidate();

    vector<shared_ptr<Professor>> professores;
    auto linhas =
        connection.selectByColumn(PROFESSOR_TABLE, EMAIL_COL_INDEX, email);

    for (const auto& linha : linhas) {
        long id = getIdFromLine(linha);

        if (cache.contains(id))
            professores.push_back(cache.at(id));
        else {
            auto professor = loadProfessor(linha);
            cache.put(id, professor);
            professores.push_back(professor);
        }
    }

    return professores;
}

// --- MÉTODOS DE VALIDAÇÃO DE NEGÓCIO ---

bool ProfessorService::existsByEmail(string email) {
    // Verifica se a lista retornada por getByEmail não está vazia.
    return !getByEmail(email).empty();
}

bool ProfessorService::existsByEmailAndIdNot(string email, long id) {
    auto email_matches = getByEmail(email);

    // Itera os Models para verificar se o email pertence a um ID diferente.
    for (const auto& professor : email_matches) {
        if (professor->getId() != id) {
            return true;  // Encontrado outro professor com o mesmo email.
        }
    }

    return false;
}

// --- MÉTODOS PÚBLICOS (CRUD) ---

// CREATE
shared_ptr<Professor> ProfessorService::save(const string& nome,
                                             const string& email,
                                             const string& senha,
                                             const string& disciplina) {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade de Email)
    if (existsByEmail(email)) {
        throw invalid_argument("O email '" + email +
                               "' já está em uso por outro professor.");
    }

    // 2. DAL WRITE
    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << disciplina;
    string data_csv = dados.str();
    // Persiste os dados e obtém o novo ID (lança runtime_error em I/O).
    long id = connection.insert(PROFESSOR_TABLE, data_csv);

    // 3. MONTAGEM DO MODELO (Evita leitura desnecessária do DB)
    string new_record_csv = to_string(id) + "," + data_csv;

    auto salvo = loadProfessor(new_record_csv);

    cache.put(id, salvo);

    return salvo;
}

// READ BY ID
shared_ptr<Professor> ProfessorService::getById(long id) {
    cache.invalidate();

    if (cache.contains(id))
        return cache.at(id);

    string linha = connection.selectOne(PROFESSOR_TABLE, id);

    auto professor = loadProfessor(linha);

    cache.put(id, professor);

    return professor;
}

// READ BY EMAIL
shared_ptr<Professor> ProfessorService::getOneByEmail(const string& email) {
    auto results = getByEmail(email);
    if (results.size() > 1) {
        // ERRO CRÍTICO: Integridade violada!
        throw runtime_error(
            "Falha de integridade: Múltiplos registros encontrados para o "
            "email: " +
            email);
    }

    if (results.empty()) {
        return nullptr;
    }

    return results.front();
}

// LIST ALL
vector<shared_ptr<Professor>> ProfessorService::listAll() {
    cache.invalidate();

    // Pega todas as linhas de dados da tabela Professores.
    vector<string> csv_records = connection.selectAll(PROFESSOR_TABLE);
    vector<shared_ptr<Professor>> professors;

    // Mapeia e injeta Horarios para todos os registros.
    for (const string& csv_line : csv_records) {
        long id = getIdFromLine(csv_line);

        if (cache.contains(id))
            professors.push_back(cache.at(id));
        else {
            auto professor = loadProfessor(csv_line);
            cache.put(id, professor);
            professors.push_back(professor);
        }
    }

    sort(professors.begin(), professors.end(),
         [](const shared_ptr<Professor>& first,
            const shared_ptr<Professor>& second) { return *first < *second; });

    return professors;
}

// UPDATE
shared_ptr<Professor> ProfessorService::updateById(long id, const string& nome,
                                                   const string& email,
                                                   const string& senha,
                                                   const string& disciplina) {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade para UPDATE, excluindo o próprio ID)
    if (existsByEmailAndIdNot(email, id)) {
        throw invalid_argument("O email '" + email +
                               "' já está em uso por outro professor.");
    }

    auto late = getById(id);

    if (!late)
        return nullptr;

    // 2. DAL WRITE (Atualiza a linha no arquivo)
    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << disciplina;
    string data_csv = dados.str();

    connection.update(PROFESSOR_TABLE, id, data_csv);

    // Constrói a linha CSV completa do registro atualizado.
    string updatedStr = to_string(id) + "," + data_csv;
    auto updated = loadProfessor(updatedStr);

    cache.put(id, updated);

    return updated;
}

// DELETE
bool ProfessorService::deleteById(long id) {
    auto professor = getById(id);

    if (!professor)
        return false;

    const auto& horarioService = manager->getHorarioService();

    horarioService->deleteByIdProfessor(id);

    connection.deleteRecord(PROFESSOR_TABLE, id);

    cache.erase(id);

    bus.publish(ProfessorDeletedEvent(id));

    return true;
}

shared_ptr<Professor> ProfessorService::loadProfessor(const string& line) {
    stringstream ss(line);
    string idStr, nome, email, senha, disciplina;

    getline(ss, idStr, ',');
    getline(ss, nome, ',');
    getline(ss, email, ',');
    getline(ss, senha, ',');
    getline(ss, disciplina, ',');

    long id = stol(idStr);

    auto& horariosLoader = manager->getHorarioListLoader();

    auto professor = make_shared<Professor>(id, nome, email, senha, disciplina,
                                            horariosLoader);

    return professor;
}