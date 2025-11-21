#include "service/agendamentoService.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "event/events.hpp"
#include "service/alunoService.hpp"
#include "service/horarioService.hpp"

using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---
#define ID_ALUNO_COL_INDEX 1
#define ID_HORARIO_COL_INDEX 2

// --- CONSTRUTOR ---
AgendamentoService::AgendamentoService(EntityManager* manager,
                                       const MockConnection& connection,
                                       EventBus& bus)
    : manager(manager),
      connection(connection),
      bus(bus),
      cache({AGENDAMENTO_TABLE, HORARIO_TABLE}) {}

// --- MÉTODOS CRUD ---

/**
 * @brief Salva um novo agendamento, aplicando as regras de negócio.
 */
shared_ptr<Agendamento> AgendamentoService::save(long alunoId, long horarioId) {
    auto horarioService = manager->getHorarioService();

    if (!horarioService->isDisponivelById(horarioId)) {
        throw invalid_argument(
            "Este horário não está aberto para agendamentos.");
    }

    stringstream dados;
    dados << alunoId << "," << horarioId << "," << "PENDENTE";

    long newId = connection.insert(AGENDAMENTO_TABLE, dados.str());

    string new_record_csv = to_string(newId) + "," + dados.str();

    auto salvo = loadAgendamento(new_record_csv);

    cache.put(newId, salvo);

    return salvo;
}

shared_ptr<Agendamento> AgendamentoService::getById(long id) {
    cache.invalidate();

    if (cache.contains(id))
        return cache.at(id);

    string linha = connection.selectOne(AGENDAMENTO_TABLE, id);

    auto agendamento = loadAgendamento(linha);

    cache.put(id, agendamento);

    return agendamento;
}

shared_ptr<Agendamento> AgendamentoService::updateById(long id, long alunoId,
                                                       long horarioId,
                                                       const Status& status) {
    if (status == Status::CONFIRMADO) {
        auto horarioService = manager->getHorarioService();

        if (!horarioService->isDisponivelById(horarioId))
            throw invalid_argument(
                "Este horário não está aberto para agendamentos.");
    }

    shared_ptr<Agendamento> late = getById(id);

    if (!late)
        return nullptr;

    long lateHorarioId = late->getHorarioId();
    Status lateStatus = late->getStatus();

    stringstream dados;
    dados << alunoId << "," << horarioId << "," << stringify(status);
    connection.update(AGENDAMENTO_TABLE, id, dados.str());

    if (lateStatus == Status::CONFIRMADO && status != Status::CONFIRMADO) {
        this->bus.publish(HorarioLiberadoEvent(lateHorarioId));
    } else if (lateStatus != Status::CONFIRMADO &&
               status == Status::CONFIRMADO) {
        this->bus.publish(HorarioOcupadoEvent(horarioId));
    }

    string updatedStr = to_string(id) + "," + dados.str();
    auto updated = loadAgendamento(updatedStr);

    cache.put(id, updated);

    return updated;
}

shared_ptr<Agendamento> AgendamentoService::updateStatusById(
    long id, const Status& status) {
    auto agendamento = getById(id);

    if (!agendamento)
        return nullptr;

    return updateById(id, agendamento->getAlunoId(),
                      agendamento->getHorarioId(), status);
}

bool AgendamentoService::deleteById(long id) {
    auto agendamento = getById(id);

    if (!agendamento)
        return false;

    Status statusOriginal = agendamento->getStatus();
    long horarioId = agendamento->getHorarioId();

    connection.deleteRecord(AGENDAMENTO_TABLE, id);

    cache.erase(id);

    if (statusOriginal == Status::CONFIRMADO) {
        bus.publish(HorarioLiberadoEvent(horarioId));
    }

    return true;
}

// --- MÉTODOS EXISTENTES ---

vector<shared_ptr<Agendamento>> AgendamentoService::listByIdAluno(long id) {
    cache.invalidate();

    vector<shared_ptr<Agendamento>> agendamentos;
    auto linhas = connection.selectByColumn(AGENDAMENTO_TABLE,
                                            ID_ALUNO_COL_INDEX, to_string(id));
    for (const auto& linha : linhas) {
        long id = getIdFromLine(linha);

        if (cache.contains(id))
            agendamentos.push_back(cache.at(id));
        else {
            auto agendamento = loadAgendamento(linha);
            cache.put(id, agendamento);
            agendamentos.push_back(agendamento);
        }
    }

    sort(
        agendamentos.begin(), agendamentos.end(),
        [](const shared_ptr<Agendamento>& first,
           const shared_ptr<Agendamento>& second) { return *first < *second; });

    return agendamentos;
}

bool AgendamentoService::deleteByIdAluno(long idAluno) {
    auto agendamentos = listByIdAluno(idAluno);

    if (agendamentos.empty())
        return false;

    vector<long> horariosParaLiberar;

    for (const auto& agendamento : agendamentos) {
        if (agendamento->getStatus() == Status::CONFIRMADO) {
            horariosParaLiberar.push_back(agendamento->getHorarioId());
        }

        cache.erase(agendamento->getId());
    }

    connection.deleteByColumn(AGENDAMENTO_TABLE, ID_ALUNO_COL_INDEX,
                              to_string(idAluno));

    for (long horarioId : horariosParaLiberar) {
        bus.publish(HorarioLiberadoEvent(horarioId));
    }

    return true;
}

bool AgendamentoService::deleteByIdHorario(long idHorario) {
    auto agendamentos = listByIdHorario(idHorario);

    if (agendamentos.empty())
        return false;

    connection.deleteByColumn(AGENDAMENTO_TABLE, ID_HORARIO_COL_INDEX,
                              to_string(idHorario));

    for (const auto& agendamento : agendamentos) {
        cache.erase(agendamento->getId());
    }

    return true;
}

// --- IMPLEMENTAÇÃO DA FUNÇÃO AUXILIAR ---

/**
 * @brief Método auxiliar estático para listar agendamentos por ID_HORARIO.
 * @param connection Conexão com o banco de dados.
 * @param id O id do horario.
 * @return vector<Agendamento>
 */
vector<shared_ptr<Agendamento>> AgendamentoService::listByIdHorario(long id) {
    cache.invalidate();

    vector<shared_ptr<Agendamento>> agendamentos;
    auto linhas = connection.selectByColumn(
        AGENDAMENTO_TABLE, ID_HORARIO_COL_INDEX, to_string(id));
    for (const auto& linha : linhas) {
        long id = getIdFromLine(linha);

        if (cache.contains(id))
            agendamentos.push_back(cache.at(id));
        else {
            auto agendamento = loadAgendamento(linha);
            cache.put(id, agendamento);
            agendamentos.push_back(agendamento);
        }
    }

    sort(
        agendamentos.begin(), agendamentos.end(),
        [](const shared_ptr<Agendamento>& first,
           const shared_ptr<Agendamento>& second) { return *first < *second; });

    return agendamentos;
}

shared_ptr<Agendamento> AgendamentoService::loadAgendamento(
    const string& line) {
    stringstream ss(line);
    string idStr, alunoIdStr, horarioIdStr, statusStr;

    getline(ss, idStr, ',');
    getline(ss, alunoIdStr, ',');
    getline(ss, horarioIdStr, ',');
    getline(ss, statusStr, ',');

    long id = stol(idStr);
    long alunoId = stol(alunoIdStr);
    long horarioId = stol(horarioIdStr);
    Status status = parseStatus(statusStr);

    auto horarioService = manager->getHorarioService();

    auto& horarioLoader = manager->getHorarioLoader();
    auto& alunoLoader = manager->getAlunoLoader();

    auto horario = horarioService->getById(horarioId);

    if (!horario) {
        throw runtime_error("Falha ao carregar Horário associado (ID: " +
                            to_string(horarioId) + ").");
    }

    auto agendamento = make_shared<Agendamento>(
        id, alunoId, horarioId, status, alunoLoader, horarioLoader,
        horario->getInicio(), horario->getFim());

    return agendamento;
}