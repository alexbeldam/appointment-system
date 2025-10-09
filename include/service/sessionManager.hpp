#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <memory>
#include <stdexcept>

#include "event/bus.hpp"
#include "model/aluno.hpp"
#include "model/professor.hpp"

/**
 * @brief Gerencia o estado da sessão do usuário logado na aplicação.
 *
 * Armazena o objeto polimórfico do usuário (Aluno ou Professor) e depende do
 * EventBus para reagir a eventos de autenticação e atualização.
 */
class SessionManager {
   private:
    /**
     * @brief Ponteiro compartilhado para o usuário logado.
     *
     * Armazena o usuário autenticado como um `std::shared_ptr<Usuario>`,
     * participando da posse compartilhada do objeto. `nullptr` se não houver
     * usuário logado.
     */
    std::shared_ptr<Usuario> user;

    /**
     * @brief Referência ao EventBus.
     *
     * Usado para inscrever o SessionManager em eventos de login e atualização
     * de usuário.
     */
    EventBus& bus;

   public:
    /**
     * @brief Construtor do SessionManager.
     *
     * Inicializa a classe e utiliza o EventBus fornecido para se inscrever
     * nos eventos necessários (login, update, etc.).
     * @param bus Referência ao EventBus do sistema.
     */
    SessionManager(EventBus& bus);

    /**
     * @brief Encerra a sessão.
     *
     * Reseta o `loggedUser`, liberando a referência do SessionManager ao
     * objeto.
     */
    void logout();

    /**
     * @brief Verifica o estado da sessão.
     * @return true se `loggedUser` não for nulo.
     */
    bool isLogged() const;

    /**
     * @brief Verifica se o usuário logado é um Professor.
     * @return true se o usuário estiver logado e for do tipo Professor.
     */
    bool isProfessor() const;

    /**
     * @brief Verifica se o usuário logado é um Aluno.
     * @return true se o usuário estiver logado e for do tipo Aluno.
     */
    bool isAluno() const;

    /**
     * @brief Retorna uma cópia do objeto Professor logado.
     * @return Cópia completa do objeto Professor.
     * @throws std::runtime_error Se o usuário não for um Professor ou não
     * estiver logado.
     */
    Professor getCurrentProfessor() const;

    /**
     * @brief Retorna uma cópia do objeto Aluno logado.
     * @return Cópia completa do objeto Aluno.
     * @throws std::runtime_error Se o usuário não for um Aluno ou não estiver
     * logado.
     */
    Aluno getCurrentAluno() const;
};

#endif