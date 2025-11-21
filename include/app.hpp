#ifndef APP_COMPOSER_HPP
#define APP_COMPOSER_HPP

#include "view/alunoUI.hpp"
#include "view/authUI.hpp"
#include "view/professorUI.hpp"

/**
 * @brief Classe responsável por centralizar a composição (criação e injeção)
 * de todas as dependências da aplicação.
 */
class App {
   private:
    MockConnection connection;
    EventBus bus;
    EntityManager manager;

    const std::shared_ptr<HorarioService>& horarioService;
    const std::shared_ptr<AgendamentoService>& agendamentoService;
    const std::shared_ptr<AlunoService>& alunoService;
    const std::shared_ptr<ProfessorService>& professorService;
    const std::shared_ptr<SessionService>& sessionService;

    AlunoController alunoController;
    ProfessorController professorController;
    HorarioController horarioController;
    LoginController loginController;
    AgendamentoController agendamentoController;

    AuthUI authUI;
    AlunoUI alunoUI;
    ProfessorUI professorUI;

   public:
    /**
     * @brief Construtor do App.
     * Responsável por inicializar todos os membros na ordem de declaração.
     */
    App();

    /**
     * @brief Inicia a execução da aplicação.
     */
    void run();
};

#endif