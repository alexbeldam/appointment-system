#include "app.hpp"

#include <iostream>

#include "view/alunoUI.hpp"
#include "view/authUI.hpp"
#include "view/professorUI.hpp"
using std::cout;

/**
 * @brief Construtor do App.
 * Inicializa todos os membros na ordem exata em que foram declarados no
 * app.hpp.
 */
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