#ifndef PROFESSOR_UI_HPP
#define PROFESSOR_UI_HPP

#include "controller/agendamentoController.hpp"
#include "controller/alunoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/professorController.hpp"
#include "event/bus.hpp"
#include "service/sessionService.hpp"
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
    ProfessorController& professorController;
    HorarioController& horarioController;
    AgendamentoController& agendamentoController;

    /**
     * @brief Gerencia o fluxo de I/O e delega o cadastro de um novo horário
     * disponível.
     */
    void cadastro_horario();

    /**
     * @brief Gerencia o fluxo de I/O e delega a listagem de horários.
     */
    void listar_horarios();

    /**
     * @brief Gerencia o fluxo de I/O e delega a exclusão de um horário.
     */
    void excluir_horario();

    /**
     * @brief Gerencia o fluxo de I/O e delega a exclusão de todos os horários
     * do professor logado.
     */
    void excluir_todos_horarios();

    /**
     * @brief Atualiza o perfil do professor logado.
     */
    void atualizar_perfil();

    void deletar_perfil();

    bool avaliar_agendamentos();

    void fazer_avaliacoes();

   public:
    ProfessorUI(ProfessorController& pc, HorarioController& hc,
                AgendamentoController& ac,
                const std::shared_ptr<SessionService>& ss);

    virtual bool show() override;
};

#endif