#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <memory>

#include "model/aluno.hpp"
#include "model/professor.hpp"

/**
 * @brief Gerencia o estado da sessão do usuário logado na aplicação.
 * * Usado para armazenar o objeto Usuario (Aluno ou Professor) que está
 * autenticado, mantendo sua identidade polimórfica e gerenciando sua vida útil
 * através de um ponteiro exclusivo.
 */
class SessionManager {
   private:
    /**
     * @brief Ponteiro inteligente que armazena o usuário logado.
     * * Usa **std::unique_ptr<Usuario>** para garantir que o SessionManager
     * tenha a **propriedade exclusiva** do objeto polimórfico (Aluno ou
     * Professor) e garantir que ele permaneça válido enquanto a sessão estiver
     * ativa. Um valor nullptr indica que nenhum usuário está logado.
     */
    std::unique_ptr<Usuario> loggedUser;

   public:
    /**
     * @brief Define o usuário que acabou de fazer login.
     * * O SessionManager assume a **propriedade exclusiva** do objeto, que deve
     * ser alocado dinamicamente.
     * @param user O **std::unique_ptr** para o objeto Usuario (Aluno ou
     * Professor) que foi autenticado.
     */
    void login(std::unique_ptr<Usuario> user);

    /**
     * @brief Encerra a sessão, limpando o usuário armazenado.
     * * O ponteiro é resetado (loggedUser.reset()), destruindo o objeto do
     * usuário.
     */
    void logout();

    /**
     * @brief Verifica se há um usuário ativo na sessão.
     * @return true se um usuário estiver logado (loggedUser não é nullptr),
     * false caso contrário.
     */
    bool isLogged() const;

    /**
     * @brief Verifica se há um professor ativo na sessão.
     * * Utiliza dynamic_cast para verificar o tipo de runtime.
     * @return true se um usuário estiver logado e for Professor, false caso
     * contrário.
     */
    bool isProfessor() const;

    /**
     * @brief Verifica se há um aluno ativo na sessão.
     * * Utiliza dynamic_cast para verificar o tipo de runtime.
     * @return true se um usuário estiver logado e for Aluno, false caso
     * contrário.
     */
    bool isAluno() const;

    /**
     * @brief Retorna uma cópia do objeto Professor logado.
     * * O método realiza um downcast seguro e retorna uma cópia do valor,
     * garantindo que não haja slicing e que o tipo exato seja retornado.
     * @return Uma cópia completa do objeto Professor.
     * @throws std::runtime_error Se o usuário não estiver logado **OU** se
     * o usuário logado não for um Professor.
     */
    Professor getCurrentProfessor() const;

    /**
     * @brief Retorna uma cópia do objeto Aluno logado.
     * * O método realiza um downcast seguro e retorna uma cópia do valor,
     * garantindo que não haja slicing e que o tipo exato seja retornado.
     * @return Uma cópia completa do objeto Aluno.
     * @throws std::runtime_error Se o usuário não estiver logado **OU** se
     * o usuário logado não for um Aluno.
     */
    Aluno getCurrentAluno() const;
};

#endif