#include "controller/horarioController.hpp"

HorarioController::HorarioController(const HorarioService& service)
    : service(service) {}

Horario HorarioController::cadastrarHorario(long idProfessor,
                                            std::time_t inicio,
                                            std::time_t fim) const {
    try {
        return service.save(idProfessor, inicio, fim);
    } catch (const std::exception& e) {
        handle_controller_exception(e, "cadastrar horário");
        throw;
    }
}

std::vector<Horario> HorarioController::listarPorProfessor(
    long idProfessor) const {
    return service.listByIdProfessor(idProfessor);
}

bool HorarioController::excluirTodosPorProfessor(long idProfessor) const {
    return service.deleteByIdProfessor(idProfessor);
}

bool HorarioController::excluirPorId(long idHorario) const {
    try {
        return service.deleteById(idHorario);
    } catch (...) {
        return false;
    }
}
