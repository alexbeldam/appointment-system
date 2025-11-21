#ifndef ALUNO_UI_HPP
#define ALUNO_UI_HPP

#include "controller/agendamentoController.hpp"
#include "controller/alunoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/professorController.hpp"
#include "event/bus.hpp"  // EventBus
#include "service/sessionService.hpp"
#include "view/consoleUI.hpp"

/**
 * @brief Classe da Interface de Usuário (Presentation Layer) para Alunos via
 * console.
 *
 * * Responsável por gerenciar toda a interação com o usuário (I/O via
 * cin/cout) relacionada às funcionalidades disponíveis para Alunos, como
 * agendamento de horários.
 */
class AlunoUI : public ConsoleUI {
   private:
    AlunoController& alunoController;
    ProfessorController& professorController;
    AgendamentoController& agendamentoController;

    /**
     * @brief Gerencia o fluxo de I/O e delega o agendamento de um horário.
     */
    void agendar_horario();
    void atualizar_perfil();
    void visualizar_agendamentos();
    void deletar_perfil();
    void cancelar_agendamento();

   public:
    AlunoUI(AlunoController& ac, ProfessorController& pc,
            AgendamentoController& agc,
            const std::shared_ptr<SessionService>& ss);

    virtual bool show() override;
};

#endif