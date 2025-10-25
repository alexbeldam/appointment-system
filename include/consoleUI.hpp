#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "controller/agendamentoController.hpp"  // Adicionado
#include "controller/alunoController.hpp"
#include "controller/horarioController.hpp"
#include "controller/loginController.hpp"
#include "controller/professorController.hpp"
#include "service/sessionManager.hpp"

/**
 * @brief Classe da Interface de Usuário (Presentation Layer) para aplicações de
 * console.
 *
 * * Responsável por gerenciar toda a interação com o usuário (I/O via
 * cin/cout), apresentar menus dinâmicos e coordenar o fluxo principal de
 * execução com base no estado da sessão.
 */
class ConsoleUI {
   private:
    // --- DEPENDÊNCIAS DE CAMADA ---

    /**
     * @brief Referência constante para o Controller de Aluno.
     * * Usada para delegar operações CRUD e a criação de novos Alunos.
     */
    const AlunoController& alunoController;

    /**
     * @brief Referência constante para o Controller de Professor.
     * * Usada para delegar operações CRUD e a criação de novos Professores.
     */
    const ProfessorController& professorController;

    /**
     * @brief Referência constante para o Controller de Login.
     * * Usada para delegar a autenticação de usuários.
     */
    const LoginController& loginController;

    /**
     * @brief Referência constante para o Controller de Horário.
     * * Usada para delegar o cadastro e listagem de horários disponíveis.
     */
    const HorarioController& horarioController;

    /**
     * @brief Referência para o Gerenciador de Sessão.
     * * Utilizada para checar o status de login (isLogged()) e manipular o
     * estado de sessão (logout).
     */
    SessionManager& sessionManager;

    /**
     * @brief Referência constante para o Controller de Agendamento.
     * * Usada para delegar a ação de agendar um horário.
     */
    const AgendamentoController& agendamentoController;

    // --- MÉTODOS DE FLUXO DE CRIAÇÃO ---

    /**
     * @brief Gerencia a interação de I/O para o cadastro de um novo Aluno.
     */
    void criar_aluno() const;

    /**
     * @brief Gerencia a interação de I/O para o cadastro de um novo Professor.
     */
    void criar_professor() const;

    /**
     * @brief Gerencia a interação de I/O para o Login (Aluno ou Professor).
     * * Delega a autenticação ao LoginController e, em caso de sucesso,
     * registra o login na sessão.
     */
    void realizar_login() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação de um Aluno.
     */
    void login_aluno() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação de um Professor.
     */
    void login_professor() const;

    /**
     * @brief Gerencia a interação de I/O para o Logout (Aluno ou Professor).
     * * Remove o usuário da sessão.
     *
     */
    void realizar_logout() const;

    // --- MÉTODOS DE CASO DE USO (ALUNO) ---

    /**
     * @brief Gerencia a interação de I/O para o caso de uso "Agendar Horário".
     */
    void handleAgendarHorario() const;

    // --- MÉTODOS DO LOOP PRINCIPAL BASEADOS NO ESTADO ---

    /**
     * @brief Executa o laço principal de interação quando o usuário NÃO está
     * autenticado.
     * * Apresenta o menu de login/cadastro.
     * @param opcao Referência à opção compartilhada entre os laços
     */
    void loop_login(int& opcao) const;

    /**
     * @brief Executa o laço de interação APÓS um Aluno ser autenticado.
     * * Apresenta o menu de opções do Aluno.
     * @param opcao Referência à opção compartilhada entre os laços
     */
    void loop_aluno(int& opcao) const;

    /**
     * @brief Executa o laço de interação APÓS um Professor ser autenticado.
     * * Apresenta o menu de opções do Professor.
     * @param opcao Referência à opção compartilhada entre os laços
     */
    void loop_professor(int& opcao) const;

   public:
    /**
     * @brief Construtor da Interface. Recebe e inicializa todas as dependências
     * da aplicação.
     * @param ac O Controller de Aluno.
     * @param pc O Controller de Professor.
     * @param lc O Controller de Login.
     * @param hc O Controller de Horário.
     * @param sm O Gerenciador de Sessão (referência mutável).
     * @param agc O Controller de Agendamento.
     */
    ConsoleUI(const AlunoController& ac, const ProfessorController& pc,
              const LoginController& lc, const HorarioController& hc,
              SessionManager& sm,
              const AgendamentoController& agc);  // Adicionado

    /**
     * @brief Inicia o motor da aplicação, gerando o loop de execução principal.
     * * O loop é dinâmico e alterna entre os estados (login, aluno, professor)
     * com base no SessionManager.
     */
    void start() const;
};

#endif