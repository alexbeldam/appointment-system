#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include "model/aluno.hpp"
#include "model/professor.hpp"
#include "persistence/entityManager.hpp"

/**
 * @brief Enumeração que define o tipo de usuário logado na sessão atual.
 */
enum class UserType {
    NONE,     /**< Ninguém logado. */
    ALUNO,    /**< Um Aluno está logado. */
    PROFESSOR /**< Um Professor está logado. */
};

/**
 * @brief Serviço de negócio responsável por gerenciar o estado da sessão de
 * usuário.
 * * Lida com o login, logout, verificação do estado e carregamento da entidade
 * do usuário logado sob demanda (Lazy Loading).
 */
class SessionService {
   private:
    EntityManager* manager; /**< Ponteiro para o gerenciador de entidades (IoC
                               Container). */
    EventBus& bus;          /**< Referência para o barramento de eventos. */
    long userId;   /**< ID do usuário logado (0 se ninguém estiver logado). */
    UserType type; /**< O tipo do usuário logado (NONE, ALUNO, PROFESSOR). */

    /**
     * @brief Função de carregamento (Loader) para a entidade Aluno.
     */
    const LoadFunction<Aluno>& alunoLoader;

    /**
     * @brief Função de carregamento (Loader) para a entidade Professor.
     */
    const LoadFunction<Professor>& professorLoader;

   public:
    /**
     * @brief Construtor da classe SessionService.
     * * Inicializa o estado da sessão como deslogado e injeta as dependências.
     * @param manager O EntityManager, usado para acessar funções de
     * carregamento.
     * @param bus O barramento de eventos.
     */
    SessionService(EntityManager* manager, EventBus& bus);

    /**
     * @brief Destrutor padrão.
     */
    ~SessionService() = default;

    /**
     * @brief Encerra a sessão atual (logout).
     * * Redefine o userId para 0 e o tipo para NONE.
     */
    void logout();

    /**
     * @brief Verifica se há algum usuário logado.
     * @return bool True se userId > 0 e type != NONE.
     */
    bool isLogged() const;

    /**
     * @brief Verifica se o usuário logado é um Professor.
     * @return bool True se o UserType for PROFESSOR.
     */
    bool isProfessor() const;

    /**
     * @brief Verifica se o usuário logado é um Aluno.
     * @return bool True se o UserType for ALUNO.
     */
    bool isAluno() const;

    /**
     * @brief Retorna o objeto Professor logado.
     * @return std::shared_ptr<Professor> O objeto Professor, ou nullptr se não
     * for um Professor logado.
     */
    std::shared_ptr<Professor> getProfessor();

    /**
     * @brief Retorna o objeto Aluno logado.
     * @return std::shared_ptr<Aluno> O objeto Aluno, ou nullptr se não for um
     * Aluno logado.
     */
    std::shared_ptr<Aluno> getAluno();
};

#endif