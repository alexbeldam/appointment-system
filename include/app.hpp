#ifndef APP_COMPOSER_HPP
#define APP_COMPOSER_HPP

#include "controller/alunoController.hpp"
#include "controller/loginController.hpp"
#include "controller/professorController.hpp"
#include "service/sessionManager.hpp"

/**
 * @brief Classe responsável por centralizar a composição (criação e injeção)
 * de todas as dependências da aplicação.
 */
class App {
   private:
    // 1. Membros de baixo nível
    const MockConnection connection;
    const AlunoMapper alunoMapper;
    const ProfessorMapper professorMapper;
    const EventBus bus;

    // 2. Membros de nível intermediário, que dependem dos anteriores
    const AgendamentoService agendamentoService;
    const HorarioService horarioService;
    const AlunoService alunoService;
    const ProfessorService professorService;
    SessionManager sessionManager;

    // 3. Membros de nível superior
    const AlunoController alunoController;
    const ProfessorController professorController;
    const LoginController loginController;

   public:
    App();

    void run();
};

#endif