#include "service/alunoService.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "event/events.hpp"
#include "service/agendamentoService.hpp"

using namespace std;

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

// Helper: Busca todos os Alunos por Matrícula.
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

// --- MÉTODOS DE VALIDAÇÃO DE NEGÓCIO (Unicidade) ---

bool AlunoService::existsByEmail(string email) {
    // Verifica se a busca retorna algum resultado.
    return !getByEmail(email).empty();
}

bool AlunoService::existsByEmailAndIdNot(string email, long id) {
    auto email_matches = getByEmail(email);

    // Itera os Models para verificar se o email pertence a um ID diferente.
    for (const auto& aluno : email_matches) {
        if (aluno->getId() != id) {
            return true;
        }
    }

    return false;
}

bool AlunoService::existsByMatricula(long matricula) {
    // Verifica se a busca retorna algum resultado.
    return !getByMatricula(matricula).empty();
}

bool AlunoService::existsByMatriculaAndIdNot(long matricula, long id) {
    auto matricula_matches = getByMatricula(matricula);

    // Itera os Models para verificar se a matrícula pertence a um ID diferente.
    for (const auto& aluno : matricula_matches) {
        if (aluno->getId() != id) {
            return true;
        }
    }

    return false;
}

// --- MÉTODOS PÚBLICOS (CRUD) ---

// CREATE
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

// READ BY ID
shared_ptr<Aluno> AlunoService::getById(long id) {
    cache.invalidate();

    if (cache.contains(id))
        return cache.at(id);

    string linha = connection.selectOne(ALUNO_TABLE, id);

    auto aluno = loadAluno(linha);

    cache.put(id, aluno);

    return aluno;
}

// READ BY EMAIL
shared_ptr<Aluno> AlunoService::getOneByEmail(const string& email) {
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

// UPDATE
shared_ptr<Aluno> AlunoService::updateById(long id, const string& nome,
                                           const string& email,
                                           const string& senha,
                                           long matricula) {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade para UPDATE, excluindo o próprio ID)
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

    // 2. DAL WRITE (Atualiza a linha no arquivo)
    stringstream dados;
    dados << nome << "," << email << "," << senha << "," << matricula;

    connection.update(ALUNO_TABLE, id, dados.str());

    string updatedStr = to_string(id) + "," + dados.str();
    auto updated = loadAluno(updatedStr);

    cache.put(id, updated);

    return updated;
}

// DELETE
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