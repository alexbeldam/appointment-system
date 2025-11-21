#ifndef ALUNO_UI_HPP
#define ALUNO_UI_HPP

#include "controller/agendamentoController.hpp"
#include "controller/alunoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/professorController.hpp"
#include "service/sessionService.hpp"
#include "view/consoleUI.hpp"

/**
 * @brief Gerencia a Interface de Usuário (UI) para um Aluno logado.
 * * Herda de ConsoleUI e implementa as funcionalidades específicas disponíveis
 * para um usuário do tipo Aluno (agendar, deletar perfil, etc.).
 */
class AlunoUI : public ConsoleUI {
   private:
    /**
     * @brief Referência ao Controller de Aluno para operações CRUD no perfil.
     */
    AlunoController& alunoController;

    /**
     * @brief Referência ao Controller de Professor para listar professores e
     * horários.
     */
    ProfessorController& professorController;

    /**
     * @brief Referência ao Controller de Agendamento para criar, visualizar e
     * cancelar agendamentos.
     */
    AgendamentoController& agendamentoController;

    /**
     * @brief Permite ao aluno visualizar professores e seus horários
     * disponíveis e criar um novo agendamento.
     */
    void agendar_horario();

    /**
     * @brief Permite ao aluno modificar seus dados cadastrais (nome, email,
     * senha, matrícula).
     */
    void atualizar_perfil();

    /**
     * @brief Exibe a lista de todos os agendamentos do aluno, com seus status.
     */
    void visualizar_agendamentos();

    /**
     * @brief Permite ao aluno excluir permanentemente sua conta do sistema.
     */
    void deletar_perfil();

    /**
     * @brief Permite ao aluno selecionar e cancelar um agendamento pendente ou
     * confirmado.
     */
    void cancelar_agendamento();

   public:
    /**
     * @brief Construtor da classe AlunoUI.
     * * Injeta as dependências dos Controllers e o SessionService para
     * gerenciar o estado do usuário logado.
     * @param ac Referência para o AlunoController.
     * @param pc Referência para o ProfessorController.
     * @param agc Referência para o AgendamentoController.
     * @param ss Ponteiro inteligente para o SessionService.
     */
    AlunoUI(AlunoController& ac, ProfessorController& pc,
            AgendamentoController& agc,
            const std::shared_ptr<SessionService>& ss);

    /**
     * @brief Exibe o menu principal do aluno e processa as escolhas.
     * * Implementação da interface virtual da classe base ConsoleUI.
     * @return bool Retorna true se a UI deve continuar (menu deve ser exibido
     * novamente), false para sair.
     */
    virtual bool show() override;
};

#endif