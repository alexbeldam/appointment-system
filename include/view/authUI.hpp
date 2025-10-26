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
    const AlunoController& alunoController;
    const LoginController& loginController;
    const ProfessorController& professorController;

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação do usuário.
     */
    void fazer_login() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação de um Aluno.
     */
    void login_aluno() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega a autenticação de um Professor.
     */
    void login_professor() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega o cadastro de um Aluno.
     */
    void cadastro_aluno() const;

    /**
     * @brief Gerencia o fluxo de I/O e delega o cadastro de um Professor.
     */
    void cadastro_professor() const;

   public:
    AuthUI(const AlunoController& ac, const ProfessorController& pc,
           const LoginController& lc, SessionManager& sm);

    virtual bool show() const override;
};

#endif