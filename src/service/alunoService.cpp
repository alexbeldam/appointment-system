#include "service/alunoService.hpp"

#include "event/events.hpp"
#include "service/agendamentoService.hpp"

using std::invalid_argument;
using std::make_shared;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

#define EMAIL_COL_INDEX 2
#define MATRICULA_COL_INDEX 4

AlunoService::AlunoService(EntityManager* manager,
                           const MockConnection& connection, EventBus& bus)
    : manager(manager),
      connection(connection),
      bus(bus),
      cache({ALUNO_TABLE, AGENDAMENTO_TABLE, HORARIO_TABLE}) {}

vector<shared_ptr<Aluno>> AlunoService::getByEmail(const string& email) {
    cache.invalidate();

    vector<shared_ptr<Aluno>> alunos;
    auto linhas =
        connection.selectByColumn(ALUNO_TABLE, EMAIL_COL_INDEX, email);

    for (const auto& linha : linhas) {
        long id = getIdFromLine(linha);

        if (cache.contains(id))
            alunos.push_back(cache.at(id));
        else {
            auto aluno = loadAluno(linha);
            cache.put(id, aluno);
            alunos.push_back(aluno);
        }
    }

    return alunos;
}

vector<shared_ptr<Aluno>> AlunoService::getByMatricula(long matricula) {
    cache.invalidate();

    vector<shared_ptr<Aluno>> alunos;
    auto linhas = connection.selectByColumn(ALUNO_TABLE, MATRICULA_COL_INDEX,
                                            to_string(matricula));

    for (const auto& linha : linhas) {
        long id = getIdFromLine(linha);

        if (cache.contains(id))
            alunos.push_back(cache.at(id));
        else {
            auto aluno = loadAluno(linha);
            cache.put(id, aluno);
            alunos.push_back(aluno);
        }
    }

    return alunos;
}

bool AlunoService::existsByEmail(string email) {
    return !getByEmail(email).empty();
}

bool AlunoService::existsByEmailAndIdNot(string email, long id) {
    auto email_matches = getByEmail(email);

    for (const auto& aluno : email_matches) {
        if (aluno->getId() != id) {
            return true;
        }
    }

    return false;
}

bool AlunoService::existsByMatricula(long matricula) {
    return !getByMatricula(matricula).empty();
}

bool AlunoService::existsByMatriculaAndIdNot(long matricula, long id) {
    auto matricula_matches = getByMatricula(matricula);

    for (const auto& aluno : matricula_matches) {
        if (aluno->getId() != id) {
            return true;
        }
    }

    return false;
}

shared_ptr<Aluno> AlunoService::save(const string& nome, const string& email,
                                     const string& senha, long matricula) {
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

    auto salvo = loadAluno(new_record_csv);

    cache.put(id, salvo);

    return salvo;
}

shared_ptr<Aluno> AlunoService::getById(long id) {
    cache.invalidate();

    if (cache.contains(id))
        return cache.at(id);

    string linha = connection.selectOne(ALUNO_TABLE, id);

    auto aluno = loadAluno(linha);

    cache.put(id, aluno);

    return aluno;
}

shared_ptr<Aluno> AlunoService::getOneByEmail(const string& email) {
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

shared_ptr<Aluno> AlunoService::updateById(long id, const string& nome,
                                           const string& email,
                                           const string& senha,
                                           long matricula) {
    if (existsByEmailAndIdNot(email, id)) {
        throw invalid_argument("O email '" + email +
                               "' já está em uso por outro aluno.");
    }
    if (existsByMatriculaAndIdNot(matricula, id)) {
        throw invalid_argument("A matrícula '" + to_string(matricula) +
                               "' já está registrada por outro aluno.");
    }

    auto late = getById(id);

    if (!late)
        return nullptr;

    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << matricula;

    connection.update(ALUNO_TABLE, id, dados.str());

    string updatedStr = to_string(id) + "," + dados.str();
    auto updated = loadAluno(updatedStr);

    cache.put(id, updated);

    return updated;
}

bool AlunoService::deleteById(long id) {
    auto aluno = getById(id);

    if (!aluno)
        return false;

    const auto& agendamentoService = manager->getAgendamentoService();

    agendamentoService->deleteByIdAluno(id);

    connection.deleteRecord(ALUNO_TABLE, id);

    cache.erase(id);

    bus.publish(AlunoDeletedEvent(id));

    return true;
}

shared_ptr<Aluno> AlunoService::loadAluno(const string& line) {
    stringstream ss(line);
    string idStr, nome, email, senha, matriculaStr;

    getline(ss, idStr, ',');
    getline(ss, nome, ',');
    getline(ss, email, ',');
    getline(ss, senha, ',');
    getline(ss, matriculaStr, ',');

    long id = stol(idStr);
    long matricula = stol(matriculaStr);

    auto& agendamentosLoader = manager->getAlunoAgendamentosListLoader();

    auto aluno = make_shared<Aluno>(id, nome, email, senha, matricula,
                                    agendamentosLoader);

    return aluno;
}