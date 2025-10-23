#include "app.hpp"
#include "consoleUI.hpp"

/**
 * @brief Construtor do App.
 * Inicializa todos os membros na ordem exata em que foram declarados no app.hpp.
 */
App::App()
    : connection(),
      alunoMapper(),
      professorMapper(),
      bus(),
      sessionManager(bus),
      horarioService(connection, bus),
      agendamentoService(connection, bus, horarioService),
      alunoService(connection, bus, alunoMapper, agendamentoService),
      professorService(connection, bus, professorMapper, horarioService),
      alunoController(alunoService),
      professorController(professorService),
      horarioController(horarioService),
      loginController(alunoService, professorService, bus),
      agendamentoController(agendamentoService)
{
}


void App::run() {
    ConsoleUI ui(
        alunoController,
        professorController,
        loginController,
        horarioController,
        sessionManager,
        agendamentoController
    );

    ui.start();
}