#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include "model/aluno.hpp"
#include "model/professor.hpp"
#include "persistence/entityManager.hpp"

enum class UserType { NONE, ALUNO, PROFESSOR };

/**
 * @brief Gerencia o estado da sessão do usuário logado na aplicação.
 *
 * Armazena o objeto polimórfico do usuário (Aluno ou Professor) e depende do
 * EventBus para reagir a eventos de autenticação e atualização.
 */
class SessionService {
   private:
    EntityManager* manager;
    EventBus& bus;
    long userId;
    UserType type;

    const LoadFunction<Aluno>& alunoLoader;
    const LoadFunction<Professor>& professorLoader;

   public:
    SessionService(EntityManager* manager, EventBus& bus);

    ~SessionService() = default;

    void logout();

    bool isLogged() const;

    bool isProfessor() const;

    bool isAluno() const;

    std::shared_ptr<Professor> getProfessor();

    std::shared_ptr<Aluno> getAluno();
};

#endif