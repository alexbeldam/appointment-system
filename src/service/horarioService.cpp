#include "service/horarioService.hpp"

#include <sstream>
#include <stdexcept>

#include "event/events.hpp"

using std::invalid_argument;
using std::runtime_error;
using std::string;
using std::to_string;
using std::vector;

#define HORARIO_TABLE "horarios"
#define ID_PROFESSOR_COL_INDEX 1

HorarioService::HorarioService(const MockConnection& connection, EventBus& bus)
    : connection(connection), bus(bus) {
    bus.subscribe<ProfessorDeletedEvent>(
        [this](const ProfessorDeletedEvent& event) {
            this->deleteByIdProfessor(event.id);
        });
}

Horario HorarioService::save(long idProfessor, std::time_t inicio,
                             std::time_t fim) const {
    if (fim <= inicio) {
        throw std::invalid_argument(
            "O horário final deve ser posterior ao horário inicial.");
    }

    auto horarios = connection.selectByColumn(
        HORARIO_TABLE, ID_PROFESSOR_COL_INDEX, std::to_string(idProfessor));

    for (const auto& linha : horarios) {
        std::stringstream ss(linha);
        std::string id, idProf, inicioExistente, fimExistente, disponivelStr;
        std::getline(ss, id, ',');
        std::getline(ss, idProf, ',');
        std::getline(ss, inicioExistente, ',');
        std::getline(ss, fimExistente, ',');
        std::getline(ss, disponivelStr, ',');

        if (inicio == std::stol(inicioExistente) &&
            fim == std::stol(fimExistente)) {
            throw std::invalid_argument(
                "Já existe um horário cadastrado nesse período.");
        }
    }

    std::stringstream dados;
    dados << idProfessor << "," << inicio << "," << fim << ",1";
    long newId = connection.insert(HORARIO_TABLE, dados.str());

    Horario novo(newId, idProfessor, inicio, fim, true);
    bus.publish(HorarioCreatedEvent(novo));

    return novo;
}

bool HorarioService::deleteByIdProfessor(long id) const {
    try {
        vector<Horario> horarios = listByIdProfessor(id);

        if (horarios.size() == 0)
            return false;

        connection.deleteByColumn(HORARIO_TABLE, ID_PROFESSOR_COL_INDEX,
                                  to_string(id));

        for (const auto& h : horarios)
            bus.publish(HorarioDeletedEvent(h.getId()));

        return true;
    } catch (const invalid_argument& e) {
        return false;
    } catch (const runtime_error& e) {
        throw;
    }
}

bool HorarioService::deleteById(long id) const {
    try {
        std::string linha = connection.selectOne(HORARIO_TABLE, id);
        if (linha.empty())
            return false;

        std::stringstream ss(linha);
        std::string idStr;
        std::getline(ss, idStr, ',');
        // O resto dos dados não é necessário para a deleção

        connection.deleteRecord(HORARIO_TABLE, id);
        bus.publish(HorarioDeletedEvent(std::stol(idStr)));

        return true;
    } catch (...) {
        return false;
    }
}

std::vector<Horario> HorarioService::listDisponivelByIdProfessor(
    long id) const {
    std::vector<Horario> todosHorarios = listByIdProfessor(id);
    std::vector<Horario> horariosDisponiveis;

    for (const auto& horario : todosHorarios) {
        if (horario.isDisponivel()) {
            horariosDisponiveis.push_back(horario);
        }
    }

    return horariosDisponiveis;
}

std::vector<Horario> HorarioService::listByIdProfessor(long id) const {
    auto linhas = connection.selectByColumn(
        HORARIO_TABLE, ID_PROFESSOR_COL_INDEX, std::to_string(id));
    std::vector<Horario> horarios;
    for (const auto& linha : linhas) {
        std::stringstream ss(linha);
        std::string idStr, idProf, inicio, fim, disponivelStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, idProf, ',');
        std::getline(ss, inicio, ',');
        std::getline(ss, fim, ',');
        std::getline(ss, disponivelStr, ',');

        horarios.emplace_back(std::stol(idStr), std::stol(idProf),
                              std::stol(inicio), std::stol(fim),
                              disponivelStr == "1");
    }
    return horarios;
}

// --- MÉTODOS ADICIONADOS PARA SUPORTE AO AGENDAMENTO ---

/**
 * @brief Busca um horário específico pelo seu ID.
 */
Horario HorarioService::getById(long id) const {
    try {
        std::string linha = connection.selectOne(HORARIO_TABLE, id);
        if (linha.empty()) {
            throw std::runtime_error("Horário não encontrado.");
        }

        std::stringstream ss(linha);
        std::string idStr, idProf, inicioStr, fimStr, disponivelStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, idProf, ',');
        std::getline(ss, inicioStr, ',');
        std::getline(ss, fimStr, ',');
        std::getline(ss, disponivelStr, ',');

        return Horario(std::stol(idStr), std::stol(idProf),
                       std::stol(inicioStr), std::stol(fimStr),
                       disponivelStr == "1");

    } catch (const std::runtime_error& e) {
        throw;
    }
}

/**
 * @brief Atualiza o status de um horário para "indisponível" (reservado).
 */
void HorarioService::marcarComoReservado(long id) const {
    try {
        Horario horario = this->getById(id);

        if (!horario.isDisponivel()) {
            return;  // Já está reservado, não faz nada.
        }

        // Formato CSV: idProfessor,inicio,fim,disponivel
        std::stringstream dados;
        dados << horario.getProfessorId() << "," << horario.getInicio() << ","
              << horario.getFim() << ","
              << "0";  // "0" significa não disponível

        connection.update(HORARIO_TABLE, id, dados.str());

        Horario horarioAtualizado(horario.getId(), horario.getProfessorId(),
                                  horario.getInicio(), horario.getFim(), false);

        bus.publish(HorarioUpdatedEvent(horarioAtualizado));

    } catch (const std::runtime_error& e) {
        throw;  // Repassa erros (ex: "Horário não encontrado" vindo do getById)
    }
}

bool HorarioService::isDisponivelById(long id) const {
    try {
        Horario horario = this->getById(id);
        return horario.isDisponivel();
    } catch (const std::runtime_error& e) {
        throw;  // Repassa erros (ex: "Horário não encontrado" vindo do getById)
    }
}