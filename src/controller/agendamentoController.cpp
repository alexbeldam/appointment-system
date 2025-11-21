#include "controller/agendamentoController.hpp"

#include <stdexcept>

#include "util/utils.hpp"

using std::invalid_argument;
using std::runtime_error;
using std::shared_ptr;
using std::to_string;

AgendamentoController::AgendamentoController(
    const shared_ptr<AgendamentoService>& service)
    : agendamentoService(service) {}

shared_ptr<Agendamento> AgendamentoController::agendarHorario(long alunoID,
                                                              long horarioId) {
    try {
        if (alunoID <= 0) {
            throw invalid_argument("ID do aluno inválido ou não fornecido.");
        }
        if (horarioId <= 0) {
            throw invalid_argument("ID do horário inválido ou não fornecido.");
        }

        auto agendamento = agendamentoService->save(alunoID, horarioId);

        return agendamento;
    } catch (const invalid_argument& e) {
        handle_controller_exception(e, "validar dados do novo agendamento");

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "criar agendamento no serviço");

        throw;
    }
}

void AgendamentoController::cancelar(long agendamentoId) {
    try {
        agendamentoService->updateStatusById(agendamentoId, Status::CANCELADO);
    } catch (const runtime_error& e) {
        handle_controller_exception(
            e, "cancelar agendamento com ID " + to_string(agendamentoId));

        throw;
    }
}

void AgendamentoController::confirmar(long agendamentoId) {
    try {
        agendamentoService->updateStatusById(agendamentoId, Status::CONFIRMADO);
    } catch (const runtime_error& e) {
        handle_controller_exception(
            e, "confirmar agendamento com ID " + to_string(agendamentoId));
        throw;
    }
}

void AgendamentoController::recusar(long agendamentoId) {
    try {
        agendamentoService->updateStatusById(agendamentoId, Status::RECUSADO);
    } catch (const runtime_error& e) {
        handle_controller_exception(
            e, "recusar agendamento com ID " + to_string(agendamentoId));
        throw;
    }
}