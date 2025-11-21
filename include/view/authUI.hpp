#ifndef AUTH_UI_HPP
#define AUTH_UI_HPP

#include "controller/alunoController.hpp"
#include "controller/loginController.hpp"
#include "controller/professorController.hpp"
#include "view/consoleUI.hpp"

/**
 * @brief Gerencia a Interface de Usuário (UI) para autenticação e cadastro de
 * usuários.
 * * Esta é a tela inicial do sistema, onde o usuário pode escolher entre login
 * ou cadastro, e especificar se é um Aluno ou Professor.
 */
class AuthUI : public ConsoleUI {
   private:
    /**
     * @brief Referência ao Controller de Aluno para cadastrar novos alunos.
     */
    AlunoController& alunoController;

    /**
     * @brief Referência ao Controller de Login para processar tentativas de
     * login.
     */
    LoginController& loginController;

    /**
     * @brief Referência ao Controller de Professor para cadastrar novos
     * professores.
     */
    ProfessorController& professorController;

    /**
     * @brief Exibe o menu de escolha entre login de Aluno e login de Professor.
     */
    void fazer_login();

    /**
     * @brief Exibe o menu de escolha entre cadastro de Aluno e cadastro de
     * Professor.
     */
    void fazer_cadastro();

    /**
     * @brief Processa a entrada de credenciais para login de um Aluno.
     */
    void login_aluno();

    /**
     * @brief Processa a entrada de credenciais para login de um Professor.
     */
    void login_professor();

    /**
     * @brief Coleta os dados e realiza o cadastro de um novo Aluno.
     */
    void cadastro_aluno();

    /**
     * @brief Coleta os dados e realiza o cadastro de um novo Professor.
     */
    void cadastro_professor();

   public:
    /**
     * @brief Construtor da classe AuthUI.
     * * Injeta as dependências dos Controllers e o SessionService.
     * @param ac Referência para o AlunoController.
     * @param pc Referência para o ProfessorController.
     * @param lc Referência para o LoginController.
     * @param ss Ponteiro inteligente para o SessionService (usado na classe
     * base).
     */
    AuthUI(AlunoController& ac, ProfessorController& pc, LoginController& lc,
           const std::shared_ptr<SessionService>& ss);

    /**
     * @brief Exibe o menu principal de autenticação (Login ou Cadastro) e
     * processa as escolhas.
     * * Implementação da interface virtual da classe base ConsoleUI.
     * @return bool Retorna true se a UI deve continuar (menu deve ser exibido
     * novamente), false para encerrar o sistema.
     */
    virtual bool show() override;
};

#endif