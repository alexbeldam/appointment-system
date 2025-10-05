#ifndef APP_COMPOSER_HPP
#define APP_COMPOSER_HPP

#include "controller/alunoController.hpp"
#include "controller/professorController.hpp"

/**
 * @brief Classe responsável por centralizar a composição (criação e injeção)
 * de todas as dependências da aplicação.
 */
class AppComposer {
   private:
    // 1. Membros de baixo nível
    const MockConnection connection;
    const AlunoMapper alunoMapper;
    const ProfessorMapper professorMapper;

    // 2. Membros de nível intermediário, que dependem dos anteriores
    const AgendamentoService agendamentoService;
    const HorarioService horarioService;
    const AlunoService alunoService;
    const ProfessorService professorService;

    // 3. Membros de nível superior
    const AlunoController alunoController;
    const ProfessorController professorController;

   public:
    AppComposer();

    // Métodos para expor os Controllers prontos
    const AlunoController& getAlunoController() const;

    const ProfessorController& getProfessorController() const;
};

#endif