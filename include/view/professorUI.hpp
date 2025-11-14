#ifndef PROFESSOR_UI_HPP
#define PROFESSOR_UI_HPP

#include "controller/agendamentoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/professorController.hpp"
#include "controller/alunoController.hpp"
#include "event/bus.hpp"
#include "service/sessionManager.hpp"
#include "view/consoleUI.hpp"

/**
 * @brief Classe da Interface de Usuário (Presentation Layer) para Professores
 * via console.
 *
 * * Responsável por gerenciar toda a interação com o usuário (I/O via
 * cin/cout) relacionada às funcionalidades disponíveis para Professores,
 * como cadastro e gerenciamento de horários.
 */
class ProfessorUI : public ConsoleUI {
   private:
    const ProfessorController& professorController;
    const HorarioController& horarioController;
    const AgendamentoController& agendamentoController;
    const AlunoController& alunoController;

    SessionManager& sessionManager;

    /**
     * @brief Gerencia o fluxo de I/O e delega o cadastro de um novo horário
     * disponível.
     */
    void cadastro_horario() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega a listagem de horários.
     */
    void listar_horarios() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega a exclusão de um horário.
     */
    void excluir_horario() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega a exclusão de todos os horários
     * do professor logado.
     */
    void excluir_todos_horarios() const;

    /**
     * @brief Atualiza o perfil do professor logado.
     */
    void atualizar_perfil() const;

    void deletar_perfil() const;

    void avaliar_agendamentos() const;

   public:
    ProfessorUI(const ProfessorController& pc, const HorarioController& hc,
                const AgendamentoController& ac, const AlunoController& alc, SessionManager& sm);

    virtual bool show() const override;
};

#endif