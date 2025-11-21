#include "service/horarioService.hpp"

#include <algorithm>

#include "event/events.hpp"
#include "service/agendamentoService.hpp"

using std::getline;
using std::invalid_argument;
using std::make_shared;
using std::runtime_error;
using std::shared_ptr;
using std::sort;
using std::stol;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

#define ID_PROFESSOR_COL_INDEX 1

HorarioService::HorarioService(EntityManager* manager,
                               const MockConnection& connection, EventBus& bus)
    : manager(manager),
      connection(connection),
      bus(bus),
      cache({HORARIO_TABLE, AGENDAMENTO_TABLE}) {
    bus.subscribe<HorarioLiberadoEvent>(
        [this](const HorarioLiberadoEvent& event) {
            updateDisponivelById(event.horarioId, true);
        });

    bus.subscribe<HorarioOcupadoEvent>(
        [this](const HorarioOcupadoEvent& event) {
            updateDisponivelById(event.horarioId, false);
        });
}

shared_ptr<Horario> HorarioService::save(long idProfessor, Timestamp inicio,
                                         Timestamp fim) {
    if (fim <= inicio) {
        throw invalid_argument(
            "O horário final deve ser posterior ao horário inicial.");
    }

    auto horarios = listByIdProfessor(idProfessor);

    for (const auto& horario : horarios) {
        if (inicio == horario->getInicio() && fim == horario->getFim()) {
            throw invalid_argument(
                "Já existe um horário cadastrado nesse período.");
        }
    }

    stringstream dados;
    dados << idProfessor << "," << inicio << "," << fim << ",1";
    long newId = connection.insert(HORARIO_TABLE, dados.str());

    string new_record_csv = to_string(newId) + "," + dados.str();

    auto salvo = loadHorario(new_record_csv);

    cache.put(newId, salvo);

    return salvo;
}

bool HorarioService::deleteByIdProfessor(long id) {
    auto horarios = listByIdProfessor(id);

    if (horarios.empty())
        return false;

    const auto& agendamentoService = manager->getAgendamentoService();

    for (const auto& horario : horarios) {
        cache.erase(horario->getId());

        agendamentoService->deleteByIdHorario(horario->getId());
    }

    connection.deleteByColumn(HORARIO_TABLE, ID_PROFESSOR_COL_INDEX,
                              to_string(id));

    return true;
}

bool HorarioService::deleteById(long id) {
    auto horario = getById(id);

    if (!horario)
        return false;

    const auto& agendamentoService = manager->getAgendamentoService();

    agendamentoService->deleteByIdHorario(id);

    connection.deleteRecord(HORARIO_TABLE, id);

    cache.erase(id);

    return true;
}

vector<shared_ptr<Horario>> HorarioService::listByIdProfessor(long id) {
    cache.invalidate();

    vector<string> csv_records = connection.selectByColumn(
        HORARIO_TABLE, ID_PROFESSOR_COL_INDEX, to_string(id));
    vector<shared_ptr<Horario>> horarios;

    for (const string& csv_line : csv_records) {
        long id = getIdFromLine(csv_line);

        if (cache.contains(id))
            horarios.push_back(cache.at(id));
        else {
            auto horario = loadHorario(csv_line);
            cache.put(id, horario);
            horarios.push_back(horario);
        }
    }

    sort(horarios.begin(), horarios.end(),
         [](const shared_ptr<Horario>& first,
            const shared_ptr<Horario>& second) { return *first < *second; });

    return horarios;
}

shared_ptr<Horario> HorarioService::getById(long id) {
    cache.invalidate();

    if (cache.contains(id))
        return cache.at(id);

    string linha = connection.selectOne(HORARIO_TABLE, id);

    auto horario = loadHorario(linha);

    cache.put(id, horario);

    return horario;
}

bool HorarioService::isDisponivelById(long id) {
    auto horario = this->getById(id);

    return horario->isDisponivel();
}

shared_ptr<Horario> HorarioService::updateById(long id, long idProfessor,
                                               Timestamp inicio, Timestamp fim,
                                               bool disponivel) {
    if (fim <= inicio) {
        throw invalid_argument(
            "O horário final deve ser posterior ao horário inicial.");
    }

    auto horarios = listByIdProfessor(idProfessor);

    for (const auto& horario : horarios) {
        if (id != horario->getId() && inicio == horario->getInicio() &&
            fim == horario->getFim()) {
            throw invalid_argument(
                "Já existe um horário cadastrado nesse período.");
        }
    }

    auto late = getById(id);

    if (!late)
        return nullptr;

    stringstream dados;
    dados << idProfessor << "," << inicio << "," << fim << "," << disponivel;
    string data_csv = dados.str();

    connection.update(HORARIO_TABLE, id, data_csv);

    string updatedStr = to_string(id) + "," + data_csv;
    auto updated = loadHorario(updatedStr);

    cache.put(id, updated);

    return updated;
}

shared_ptr<Horario> HorarioService::updateDisponivelById(long id,
                                                         bool disponivel) {
    auto horario = getById(id);

    if (!horario)
        return nullptr;

    return updateById(id, horario->getProfessorId(), horario->getInicio(),
                      horario->getFim(), disponivel);
}

shared_ptr<Horario> HorarioService::loadHorario(const string& line) {
    stringstream ss(line);
    string idStr, professorIdStr, inicioStr, fimStr, disponivelStr;

    getline(ss, idStr, ',');
    getline(ss, professorIdStr, ',');
    getline(ss, inicioStr, ',');
    getline(ss, fimStr, ',');
    getline(ss, disponivelStr, ',');

    long id = stol(idStr);
    long professorId = stol(professorIdStr);
    long inicio = stol(inicioStr);
    long fim = stol(fimStr);
    bool disponivel = disponivelStr == "1";

    auto& professorLoader = manager->getProfessorLoader();
    auto& agendamentosLoader = manager->getHorarioAgendamentosListLoader();

    auto horario =
        make_shared<Horario>(id, professorId, inicio, fim, disponivel,
                             professorLoader, agendamentosLoader);

    return horario;
}