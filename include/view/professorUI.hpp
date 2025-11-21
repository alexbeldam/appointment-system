#ifndef PROFESSOR_UI_HPP
#define PROFESSOR_UI_HPP

#include "controller/agendamentoController.hpp"
#include "controller/alunoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/professorController.hpp"
#include "service/sessionService.hpp"
#include "view/consoleUI.hpp"

/**
 * @brief Gerencia a Interface de Usuário (UI) para um Professor logado.
 * * Herda de ConsoleUI e implementa as funcionalidades específicas disponíveis
 * para um usuário do tipo Professor (gerenciar horários, avaliar agendamentos,
 * etc.).
 */
class ProfessorUI : public ConsoleUI {
   private:
    /**
     * @brief Referência ao Controller de Professor para operações CRUD no
     * perfil.
     */
    ProfessorController& professorController;

    /**
     * @brief Referência ao Controller de Horário para gerenciar a
     * disponibilidade do professor.
     */
    HorarioController& horarioController;

    /**
     * @brief Referência ao Controller de Agendamento para listar e avaliar
     * agendamentos pendentes.
     */
    AgendamentoController& agendamentoController;

    /**
     * @brief Permite ao professor cadastrar um novo horário de disponibilidade.
     */
    void cadastro_horario();

    /**
     * @brief Lista todos os horários de disponibilidade cadastrados pelo
     * professor.
     */
    void listar_horarios();

    /**
     * @brief Permite ao professor selecionar e excluir um único horário.
     */
    void excluir_horario();

    /**
     * @brief Permite ao professor excluir todos os seus horários de
     * disponibilidade de uma vez.
     */
    void excluir_todos_horarios();

    /**
     * @brief Permite ao professor modificar seus dados cadastrais (nome, email,
     * senha, disciplina).
     */
    void atualizar_perfil();

    /**
     * @brief Permite ao professor excluir permanentemente sua conta do sistema.
     */
    void deletar_perfil();

    /**
     * @brief Exibe o menu de agendamentos pendentes e verifica se há
     * agendamentos para avaliar.
     * @return bool Retorna true para continuar avaliando e false para voltar ao
     * menu pincipal.
     */
    bool avaliar_agendamentos();

    /**
     * @brief Processa a avaliação (confirmação/recusa) dos agendamentos
     * pendentes.
     */
    void fazer_avaliacoes();

    /**
     * @brief Permite ao professor selecionar e cancelar um agendamento
     * confirmado.
     */
    void cancelar_agendamento();

   public:
    /**
     * @brief Construtor da classe ProfessorUI.
     * * Injeta as dependências dos Controllers e o SessionService para
     * gerenciar o estado do usuário logado.
     * @param pc Referência para o ProfessorController.
     * @param hc Referência para o HorarioController.
     * @param ac Referência para o AgendamentoController.
     * @param ss Ponteiro inteligente para o SessionService.
     */
    ProfessorUI(ProfessorController& pc, HorarioController& hc,
                AgendamentoController& ac,
                const std::shared_ptr<SessionService>& ss);

    /**
     * @brief Exibe o menu principal do professor e processa as escolhas.
     * * Implementação da interface virtual da classe base ConsoleUI.
     * @return bool Retorna true se a UI deve continuar, false para sair.
     */
    virtual bool show() override;
};

#endif