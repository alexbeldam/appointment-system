#ifndef APP_COMPOSER_HPP
#define APP_COMPOSER_HPP

// Includes dos Controladores
#include "controller/alunoController.hpp"
#include "controller/loginController.hpp"
#include "controller/professorController.hpp"
#include "controller/horarioController.hpp"
#include "controller/agendamentoController.hpp"

// Includes dos Serviços e Mappers
#include "service/sessionManager.hpp"
#include "service/agendamentoService.hpp"
#include "service/horarioService.hpp"
#include "service/alunoService.hpp"
#include "service/professorService.hpp"
#include "data/mockConnection.hpp"
#include "event/bus.hpp"
#include "map/alunoMapper.hpp"
#include "map/professorMapper.hpp"

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
     SessionManager sessionManager;

    // 2. Membros de nível intermediário (Serviços)
     HorarioService horarioService;
     AgendamentoService agendamentoService;
     AlunoService alunoService;
     ProfessorService professorService;
    
    // 3. Membros de nível superior (Controladores)
     AlunoController alunoController;
     ProfessorController professorController;
     HorarioController horarioController;
     LoginController loginController;
     AgendamentoController agendamentoController;

   public:
    /**
     * @brief Construtor do App.
     * Responsável por inicializar todos os membros na ordem de declaração.
     */
    App();

    /**
     * @brief Inicia a execução da aplicação.
     * Cria a camada de UI (Console) e injeta os controladores nela.
     */
    void run();
};

#endif