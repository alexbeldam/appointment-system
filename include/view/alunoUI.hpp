#ifndef ALUNO_UI_HPP
#define ALUNO_UI_HPP

#include "controller/agendamentoController.hpp"
#include "controller/alunoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/professorController.hpp"
#include "view/consoleUI.hpp"
#include "event/bus.hpp" // EventBus
#include "service/sessionManager.hpp"

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
    const AlunoController& alunoController;
    const ProfessorController& professorController;
    const HorarioController& horarioController;
    const AgendamentoController& agendamentoController;
    EventBus& bus;
    SessionManager& sessionManager;
    /**
     * @brief Gerencia o fluxo de I/O e delega o agendamento de um horário.
     */
    void agendar_horario() const;
    void atualizar_perfil();

   public:
    AlunoUI(const AlunoController& ac, const ProfessorController& pc,
            const HorarioController& hc, const AgendamentoController& agc,
            EventBus& bus, SessionManager& sm);

    virtual bool show() const override;
};

#endif