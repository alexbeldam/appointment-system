#include "app.hpp"

#include "consoleUI.hpp"

App::App()
    : connection(),
      alunoMapper(),
      professorMapper(),
      agendamentoService(connection),
      horarioService(connection),
      alunoService(connection, alunoMapper, agendamentoService),
      professorService(connection, professorMapper, horarioService),
      sessionManager(),
      alunoController(alunoService),
      professorController(professorService),
      loginController(alunoService, professorService, sessionManager) {}

void App::run() {
    ConsoleUI ui(alunoController, professorController, loginController,
                 sessionManager);

    ui.start();
}