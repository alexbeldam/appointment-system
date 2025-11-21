#ifndef APP_COMPOSER_HPP
#define APP_COMPOSER_HPP

#include "view/alunoUI.hpp"
#include "view/authUI.hpp"
#include "view/professorUI.hpp"

/**
 * @brief Classe principal que compõe e orquestra todas as dependências da
 * aplicação.
 * * Esta classe segue o padrão Composer/Composition Root, sendo responsável
 * por inicializar a infraestrutura (conexão, bus, services) e as camadas de
 * controle e interface, gerenciando o ciclo de vida da aplicação.
 */
class App {
   private:
    // --- Infraestrutura e Persistência ---
    MockConnection connection; /**< A conexão mock de persistência. */
    EventBus bus;              /**< O barramento de eventos centralizado. */
    EntityManager
        manager; /**< O gerenciador de entidades/serviços (Composition Root). */

    // --- Referências aos Serviços (Injetadas pelo EntityManager) ---
    const std::shared_ptr<HorarioService>& horarioService;
    const std::shared_ptr<AgendamentoService>& agendamentoService;
    const std::shared_ptr<AlunoService>& alunoService;
    const std::shared_ptr<ProfessorService>& professorService;
    const std::shared_ptr<SessionService>& sessionService;

    // --- Controllers ---
    AlunoController alunoController;
    ProfessorController professorController;
    HorarioController horarioController;
    LoginController loginController;
    AgendamentoController agendamentoController;

    // --- User Interfaces (Views) ---
    AuthUI authUI;
    AlunoUI alunoUI;
    ProfessorUI professorUI;

   public:
    /**
     * @brief Construtor da classe App.
     * * Responsável por inicializar todas as dependências na ordem correta,
     * garantindo a injeção de dependência e a composição de todo o sistema.
     */
    App();

    /**
     * @brief Inicia o ciclo de execução da aplicação.
     * * Este método contém o loop principal que alterna entre as diferentes
     * interfaces de usuário (AuthUI, AlunoUI, ProfessorUI) com base no estado
     * do SessionService.
     */
    void run();
};

#endif