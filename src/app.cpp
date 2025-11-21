#include "app.hpp"

#include <iostream>

using std::cout;

App::App()
    : connection(),
      bus(),
      manager(connection, bus),
      horarioService(manager.getHorarioService()),
      agendamentoService(manager.getAgendamentoService()),
      alunoService(manager.getAlunoService()),
      professorService(manager.getProfessorService()),
      sessionService(manager.getSessionService()),
      alunoController(alunoService),
      professorController(professorService),
      horarioController(horarioService),
      loginController(alunoService, professorService, bus),
      agendamentoController(agendamentoService),
      authUI(alunoController, professorController, loginController,
             sessionService),
      alunoUI(alunoController, professorController, agendamentoController,
              sessionService),
      professorUI(professorController, horarioController, agendamentoController,
                  sessionService) {}

void App::run() {
    bool keepRunning = true;

    while (keepRunning) {
        if (sessionService->isProfessor())
            keepRunning = professorUI.show();
        else if (sessionService->isAluno())
            keepRunning = alunoUI.show();
        else
            keepRunning = authUI.show();
    }

    cout << "\n>> Saindo do programa\n";
}