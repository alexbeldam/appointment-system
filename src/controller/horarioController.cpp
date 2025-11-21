#include "controller/horarioController.hpp"

using std::exception;
using std::shared_ptr;

HorarioController::HorarioController(const shared_ptr<HorarioService>& service)
    : service(service) {}

shared_ptr<Horario> HorarioController::cadastrarHorario(long idProfessor,
                                                        Timestamp inicio,
                                                        Timestamp fim) {
    try {
        return service->save(idProfessor, inicio, fim);
    } catch (const exception& e) {
        handle_controller_exception(e, "cadastrar horário");
        throw;
    }
}

bool HorarioController::excluirTodosPorProfessor(long idProfessor) {
    return service->deleteByIdProfessor(idProfessor);
}

bool HorarioController::excluirPorId(long idHorario) {
    return service->deleteById(idHorario);
}