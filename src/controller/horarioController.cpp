#include "controller/horarioController.hpp"

HorarioController::HorarioController(HorarioService& service)
    : service(service) {}

Horario HorarioController::cadastrarHorario(long idProfessor,
                                            const std::string& inicio,
                                            const std::string& fim) {
  try {
    return service.save(idProfessor, inicio, fim);
  } catch (const std::exception& e) {
    handle_controller_exception(e, "cadastrar horário");
    throw;
  }
}

std::vector<Horario> HorarioController::listarPorProfessor(long idProfessor) const {
  return service.listByIdProfessor(idProfessor);
}
