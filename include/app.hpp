#ifndef APP_COMPOSER_HPP
#define APP_COMPOSER_HPP

// Includes dos Controladores
#include "controller/agendamentoController.hpp"
#include "controller/alunoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/loginController.hpp"
#include "controller/professorController.hpp"

// Includes dos Serviços e Mappers
#include "event/bus.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"
#include "service/agendamentoService.hpp"
#include "service/alunoService.hpp"
#include "service/horarioService.hpp"
#include "service/professorService.hpp"
#include "service/sessionService.hpp"
#include "view/alunoUI.hpp"
#include "view/authUI.hpp"
#include "view/professorUI.hpp"

/**
 * @brief Classe responsável por centralizar a composição (criação e injeção)
 * de todas as dependências da aplicação.
 */
class App {
   private:
    // 1. Membros de baixo nível
    MockConnection connection;
    EventBus bus;
    EntityManager manager;

    // 2. Membros de nível intermediário (Serviços)
    const std::shared_ptr<HorarioService>& horarioService;
    const std::shared_ptr<AgendamentoService>& agendamentoService;
    const std::shared_ptr<AlunoService>& alunoService;
    const std::shared_ptr<ProfessorService>& professorService;
    const std::shared_ptr<SessionService>& sessionService;

    // 3. Membros de nível superior (Controladores)
    AlunoController alunoController;
    ProfessorController professorController;
    HorarioController horarioController;
    LoginController loginController;
    AgendamentoController agendamentoController;

    // 4. Interfaces de Usuário (UIs)
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