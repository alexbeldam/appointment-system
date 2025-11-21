#ifndef AUTH_UI_HPP
#define AUTH_UI_HPP

#include "controller/alunoController.hpp"
#include "controller/loginController.hpp"
#include "controller/professorController.hpp"
#include "view/consoleUI.hpp"

/**
 * @brief Classe da Interface de Usuário (Presentation Layer) para autenticação
 * via console.
 *
 * * Responsável por gerenciar toda a interação com o usuário (I/O via
 * cin/cout) relacionada à autenticação, como login e logout.
 */
class AuthUI : public ConsoleUI {
   private:
    AlunoController& alunoController;
    LoginController& loginController;
    ProfessorController& professorController;

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação do usuário.
     */
    void fazer_login();

    /**
     * @brief Gerencia o fluxo de I/O e delega o cadastro de um usuário.
     */
    void fazer_cadastro();

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação de um Aluno.
     */
    void login_aluno();

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação de um Professor.
     */
    void login_professor();

    /**
     * @brief Gerencia o fluxo de I/O e delega o cadastro de um Aluno.
     */
    void cadastro_aluno();

    /**
     * @brief Gerencia o fluxo de I/O e delega o cadastro de um Professor.
     */
    void cadastro_professor();

   public:
    AuthUI(AlunoController& ac, ProfessorController& pc, LoginController& lc,
           const std::shared_ptr<SessionService>& ss);

    virtual bool show() override;
};

#endif