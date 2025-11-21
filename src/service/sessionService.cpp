#include "service/sessionService.hpp"

#include <stdexcept>

#include "event/events.hpp"

using std::runtime_error;
using std::shared_ptr;

SessionService::SessionService(EntityManager* manager, EventBus& bus)
    : manager(manager),
      bus(bus),
      userId(0),
      type(UserType::NONE),
      alunoLoader(manager->getAlunoLoader()),
      professorLoader(manager->getProfessorLoader()) {
    bus.subscribe<AlunoLoggedInEvent>([this](const AlunoLoggedInEvent& event) {
        type = UserType::ALUNO;
        userId = event.alunoId;
    });

    bus.subscribe<ProfessorLoggedInEvent>(
        [this](const ProfessorLoggedInEvent& event) {
            type = UserType::PROFESSOR;
            userId = event.professorId;
        });

    bus.subscribe<AlunoDeletedEvent>([this](const AlunoDeletedEvent& event) {
        if (type == UserType::ALUNO && userId == event.alunoId)
            logout();
    });

    bus.subscribe<ProfessorDeletedEvent>(
        [this](const ProfessorDeletedEvent& event) {
            if (type == UserType::PROFESSOR && userId == event.professorId)
                logout();
        });
}

void SessionService::logout() {
    type = UserType::NONE;
    userId = 0;
}

bool SessionService::isLogged() const {
    return type != UserType::NONE && userId > 0;
}

bool SessionService::isAluno() const {
    return isLogged() && type == UserType::ALUNO;
}

bool SessionService::isProfessor() const {
    return isLogged() && type == UserType::PROFESSOR;
}

shared_ptr<Professor> SessionService::getProfessor() {
    if (!isLogged())
        throw runtime_error("Nenhum usuário logado");
    if (!isProfessor())
        throw runtime_error("O usuário logado não é Professor");

    return professorLoader(userId);
}

shared_ptr<Aluno> SessionService::getAluno() {
    if (!isLogged())
        throw runtime_error("Nenhum usuário logado");
    if (!isAluno())
        throw runtime_error("O usuário logado não é Aluno");

    return alunoLoader(userId);
}