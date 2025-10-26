#include "service/agendamentoService.hpp"

#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "event/events.hpp"

using std::getline;
using std::invalid_argument;
using std::runtime_error;
using std::stol;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---
#define AGENDAMENTO_TABLE "agendamentos"
#define ID_ALUNO_COL_INDEX 1
#define ID_HORARIO_COL_INDEX 2

// --- CONSTRUTOR ---
AgendamentoService::AgendamentoService(const MockConnection& connection,
                                       EventBus& bus,
                                       const HorarioService& horarioService)
    : connection(connection), bus(bus), horarioService(horarioService) {
    bus.subscribe<AlunoDeletedEvent>([this](const AlunoDeletedEvent& event) {
        this->deleteByIdAluno(event.id);
    });

    bus.subscribe<HorarioDeletedEvent>(
        [this](const HorarioDeletedEvent& event) {
            this->deleteByIdHorario(event.id);
        });
}

// --- MÉTODOS CRUD ---

/**
 * @brief Salva um novo agendamento, aplicando as regras de negócio.
 */
Agendamento AgendamentoService::save(long alunoId, long horarioId) const {
    if (!horarioService.isDisponivelById(horarioId)) {
        throw std::runtime_error(
            "Este horário não está aberto para agendamentos.");
    }

    // AC 1 (Registro): Salvar o agendamento (com status "PENDENTE")
    stringstream dados;
    dados << alunoId << "," << horarioId << "," << "PENDENTE";

    long newId = connection.insert(AGENDAMENTO_TABLE, dados.str());

    Agendamento salvo(newId, alunoId, horarioId, "PENDENTE");

    // AC 1 (Notificar)
    this->bus.publish(AgendamentoCreatedEvent(salvo));

    return salvo;
}

std::optional<Agendamento> AgendamentoService::getById(long id) const {
    try {
        string linha = connection.selectOne(AGENDAMENTO_TABLE, id);
        stringstream ss(linha);
        string idStr, alunoIdStr, horarioIdStr, statusStr;
        getline(ss, idStr, ',');
        getline(ss, alunoIdStr, ',');
        getline(ss, horarioIdStr, ',');
        getline(ss, statusStr, ',');

        return Agendamento(stol(idStr), stol(alunoIdStr), stol(horarioIdStr),
                           statusStr);

    } catch (const invalid_argument& e) {
        return std::nullopt;
    } catch (const runtime_error& e) {
        throw;
    }
}

vector<Agendamento> AgendamentoService::listAll() const {
    vector<Agendamento> agendamentos;
    try {
        auto linhas = connection.selectAll(AGENDAMENTO_TABLE);
        for (const auto& linha : linhas) {
            stringstream ss(linha);
            string idStr, alunoIdStr, horarioIdStr, statusStr;
            getline(ss, idStr, ',');
            getline(ss, alunoIdStr, ',');
            getline(ss, horarioIdStr, ',');
            getline(ss, statusStr, ',');

            agendamentos.emplace_back(stol(idStr), stol(alunoIdStr),
                                      stol(horarioIdStr), statusStr);
        }
        return agendamentos;
    } catch (const runtime_error& e) {
        throw;
    }
}

std::optional<Agendamento> AgendamentoService::updateById(
    long id, long alunoId, long horarioId, const std::string& status) const {
    try {
        stringstream dados;
        dados << alunoId << "," << horarioId << "," << status;

        connection.update(AGENDAMENTO_TABLE, id, dados.str());
        Agendamento agendamentoAtualizado(id, alunoId, horarioId, status);
        this->bus.publish(AgendamentoUpdatedEvent(agendamentoAtualizado));
        return agendamentoAtualizado;
    } catch (const invalid_argument& e) {
        return std::nullopt;
    } catch (const runtime_error& e) {
        throw;
    }
}

bool AgendamentoService::deleteById(long id) const {
    try {
        connection.deleteRecord(AGENDAMENTO_TABLE, id);
        bus.publish(AgendamentoDeletedEvent(id));
        return true;
    } catch (...) {
        return false;
    }
}

// --- MÉTODOS EXISTENTES ---

vector<Agendamento> AgendamentoService::listByIdAluno(long id) const {
    vector<Agendamento> agendamentos;
    try {
        auto linhas = connection.selectByColumn(
            AGENDAMENTO_TABLE, ID_ALUNO_COL_INDEX, to_string(id));
        for (const auto& linha : linhas) {
            stringstream ss(linha);
            string idStr, alunoIdStr, horarioIdStr, statusStr;
            getline(ss, idStr, ',');
            getline(ss, alunoIdStr, ',');
            getline(ss, horarioIdStr, ',');
            getline(ss, statusStr, ',');

            agendamentos.emplace_back(stol(idStr), stol(alunoIdStr),
                                      stol(horarioIdStr), statusStr);
        }
        return agendamentos;
    } catch (const runtime_error& e) {
        throw;
    }
}

bool AgendamentoService::deleteByIdAluno(long id) const {
    try {
        vector<Agendamento> agendamentos = listByIdAluno(id);
        if (agendamentos.empty())
            return false;
        connection.deleteByColumn(AGENDAMENTO_TABLE, ID_ALUNO_COL_INDEX,
                                  to_string(id));
        for (const auto& a : agendamentos)
            bus.publish(AgendamentoDeletedEvent(a.getId()));
        return true;
    } catch (const invalid_argument& e) {
        return false;
    } catch (const runtime_error& e) {
        throw;
    }
}

bool AgendamentoService::deleteByIdHorario(long id) const {
    try {
        vector<Agendamento> agendamentos =
            listByIdHorario(this->connection, id);
        if (agendamentos.empty())
            return false;
        connection.deleteByColumn(AGENDAMENTO_TABLE, ID_HORARIO_COL_INDEX,
                                  to_string(id));
        for (const auto& a : agendamentos)
            bus.publish(AgendamentoDeletedEvent(a.getId()));
        return true;
    } catch (const invalid_argument& e) {
        return false;
    } catch (const runtime_error& e) {
        throw;
    }
}

// --- IMPLEMENTAÇÃO DA FUNÇÃO AUXILIAR ---

/**
 * @brief Método auxiliar estático para listar agendamentos por ID_HORARIO.
 * @param connection Conexão com o banco de dados.
 * @param id O id do horario.
 * @return vector<Agendamento>
 */
vector<Agendamento> AgendamentoService::listByIdHorario(
    const MockConnection& connection, long id) const {
    vector<Agendamento> agendamentos;
    try {
        auto linhas = connection.selectByColumn(
            AGENDAMENTO_TABLE, ID_HORARIO_COL_INDEX, to_string(id));
        for (const auto& linha : linhas) {
            stringstream ss(linha);
            string idStr, alunoIdStr, horarioIdStr, statusStr;
            getline(ss, idStr, ',');
            getline(ss, alunoIdStr, ',');
            getline(ss, horarioIdStr, ',');
            getline(ss, statusStr, ',');

            agendamentos.emplace_back(stol(idStr), stol(alunoIdStr),
                                      stol(horarioIdStr), statusStr);
        }
        return agendamentos;
    } catch (const runtime_error& e) {
        throw;
    }
}