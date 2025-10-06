#include "app.hpp"

#include "consoleUI.hpp"

App::App()
    : connection(),
      alunoMapper(),
      professorMapper(),
      bus(),
      agendamentoService(connection, bus),
      horarioService(connection, bus),
      alunoService(connection, bus, alunoMapper, agendamentoService),
      professorService(connection, bus, professorMapper, horarioService),
      sessionManager(bus),
      alunoController(alunoService),
      professorController(professorService),
      loginController(alunoService, professorService, bus) {}

void App::run() {
    ConsoleUI ui(alunoController, professorController, loginController,
                 sessionManager);

    ui.start();
}