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
      sessionManager(bus),
      horarioService(connection, bus),
      agendamentoService(connection, bus, horarioService),
      alunoService(connection, bus, agendamentoService),
      professorService(connection, bus, horarioService),
      alunoController(alunoService),
      professorController(professorService),
      horarioController(horarioService),
      loginController(alunoService, professorService, bus),
      agendamentoController(agendamentoService, horarioService),
      authUI(alunoController, professorController, loginController,
             sessionManager),
      alunoUI(alunoController, professorController, horarioController,
              agendamentoController, sessionManager),
      professorUI(professorController, horarioController, agendamentoController,
                  alunoController, sessionManager) {}

void App::run() {
    bool keepRunning = true;

    while (keepRunning) {
        if (sessionManager.isProfessor())
            keepRunning = professorUI.show();
        else if (sessionManager.isAluno())
            keepRunning = alunoUI.show();
        else
            keepRunning = authUI.show();
    }

    cout << "\n>> Saindo do programa\n";
}