#ifndef APP_COMPOSER_HPP
#define APP_COMPOSER_HPP

#include "controller/alunoController.hpp"
#include "controller/loginController.hpp"
#include "controller/professorController.hpp"
#include "service/sessionManager.hpp"
#include "controller/horarioController.hpp"


/**
 * @brief Classe responsável por centralizar a composição (criação e injeção)
 * de todas as dependências da aplicação.
 */
class App {
   private:
    // 1. Membros de baixo nível
     MockConnection connection;
     AlunoMapper alunoMapper;
     ProfessorMapper professorMapper;
    EventBus bus;

    // 2. Membros de nível intermediário, que dependem dos anteriores
     AgendamentoService agendamentoService;
     HorarioService horarioService;
     AlunoService alunoService;
     ProfessorService professorService;
    HorarioController horarioController;
    SessionManager sessionManager;


    // 3. Membros de nível superior
     AlunoController alunoController;
     ProfessorController professorController;
     LoginController loginController;

   public:
    App();

    void run();
};

#endif