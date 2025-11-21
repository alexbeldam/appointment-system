#include "service/professorService.hpp"

#include <algorithm>

#include "event/events.hpp"
#include "service/horarioService.hpp"

using std::invalid_argument;
using std::make_shared;
using std::runtime_error;
using std::shared_ptr;
using std::sort;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

#define EMAIL_COL_INDEX 2

ProfessorService::ProfessorService(EntityManager* manager,
                                   const MockConnection& connection,
                                   EventBus& bus)
    : manager(manager),
      connection(connection),
      bus(bus),
      cache({PROFESSOR_TABLE, HORARIO_TABLE, AGENDAMENTO_TABLE}) {}

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

bool ProfessorService::existsByEmail(string email) {
    return !getByEmail(email).empty();
}

bool ProfessorService::existsByEmailAndIdNot(string email, long id) {
    auto email_matches = getByEmail(email);

    for (const auto& professor : email_matches) {
        if (professor->getId() != id) {
            return true;
        }
    }

    return false;
}

shared_ptr<Professor> ProfessorService::save(const string& nome,
                                             const string& email,
                                             const string& senha,
                                             const string& disciplina) {
    if (existsByEmail(email)) {
        throw invalid_argument("O email '" + email +
                               "' já está em uso por outro professor.");
    }

    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << disciplina;
    string data_csv = dados.str();
    long id = connection.insert(PROFESSOR_TABLE, data_csv);

    string new_record_csv = to_string(id) + "," + data_csv;

    auto salvo = loadProfessor(new_record_csv);

    cache.put(id, salvo);

    return salvo;
}

shared_ptr<Professor> ProfessorService::getById(long id) {
    cache.invalidate();

    if (cache.contains(id))
        return cache.at(id);

    string linha = connection.selectOne(PROFESSOR_TABLE, id);

    auto professor = loadProfessor(linha);

    cache.put(id, professor);

    return professor;
}

shared_ptr<Professor> ProfessorService::getOneByEmail(const string& email) {
    auto results = getByEmail(email);
    if (results.size() > 1) {
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

vector<shared_ptr<Professor>> ProfessorService::listAll() {
    cache.invalidate();

    vector<string> csv_records = connection.selectAll(PROFESSOR_TABLE);
    vector<shared_ptr<Professor>> professors;

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

shared_ptr<Professor> ProfessorService::updateById(long id, const string& nome,
                                                   const string& email,
                                                   const string& senha,
                                                   const string& disciplina) {
    if (existsByEmailAndIdNot(email, id)) {
        throw invalid_argument("O email '" + email +
                               "' já está em uso por outro professor.");
    }

    auto late = getById(id);

    if (!late)
        return nullptr;

    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << disciplina;
    string data_csv = dados.str();

    connection.update(PROFESSOR_TABLE, id, data_csv);

    string updatedStr = to_string(id) + "," + data_csv;
    auto updated = loadProfessor(updatedStr);

    cache.put(id, updated);

    return updated;
}

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