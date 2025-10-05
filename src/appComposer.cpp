#include "appComposer.hpp"

AppComposer::AppComposer()
    : connection(),
      alunoMapper(),
      professorMapper(),
      agendamentoService(connection),
      horarioService(connection),
      alunoService(connection, alunoMapper, agendamentoService),
      professorService(connection, professorMapper, horarioService),
      alunoController(alunoService),
      professorController(professorService) {}

const AlunoController& AppComposer::getAlunoController() const {
    return alunoController;
}

const ProfessorController& AppComposer::getProfessorController() const {
    return professorController;
}