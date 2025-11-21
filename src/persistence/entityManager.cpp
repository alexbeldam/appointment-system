#include "persistence/entityManager.hpp"

#include "service/agendamentoService.hpp"
#include "service/alunoService.hpp"
#include "service/horarioService.hpp"
#include "service/professorService.hpp"
#include "service/sessionService.hpp"

using namespace std;

EntityManager::EntityManager(const MockConnection& conn, EventBus& bus) {
    alunoService = make_shared<AlunoService>(this, conn, bus);
    professorService = make_shared<ProfessorService>(this, conn, bus);
    horarioService = make_shared<HorarioService>(this, conn, bus);
    agendamentoService = make_shared<AgendamentoService>(this, conn, bus);
    sessionService = make_shared<SessionService>(this, bus);

    alunoLoader = [this](long id) { return alunoService->getById(id); };
    professorLoader = [this](long id) { return professorService->getById(id); };
    horarioLoader = [this](long id) { return horarioService->getById(id); };

    horarioListLoader = [this](long professorId) {
        return horarioService->listByIdProfessor(professorId);
    };

    alunoAgendamentosLoader = [this](long alunoId) {
        return agendamentoService->listByIdAluno(alunoId);
    };

    horarioAgendamentosLoader = [this](long horarioId) {
        return agendamentoService->listByIdHorario(horarioId);
    };
}

const shared_ptr<AgendamentoService>& EntityManager::getAgendamentoService()
    const {
    return agendamentoService;
}

const shared_ptr<AlunoService>& EntityManager::getAlunoService() const {
    return alunoService;
}

const shared_ptr<ProfessorService>& EntityManager::getProfessorService() const {
    return professorService;
}

const shared_ptr<HorarioService>& EntityManager::getHorarioService() const {
    return horarioService;
}

const shared_ptr<SessionService>& EntityManager::getSessionService() const {
    return sessionService;
}

const EntityManager::AlunoLoader& EntityManager::getAlunoLoader() const {
    return alunoLoader;
}

const EntityManager::ProfessorLoader& EntityManager::getProfessorLoader()
    const {
    return professorLoader;
}

const EntityManager::HorarioLoader& EntityManager::getHorarioLoader() const {
    return horarioLoader;
}

const EntityManager::HorarioListLoader& EntityManager::getHorarioListLoader()
    const {
    return horarioListLoader;
}

const EntityManager::AgendamentoListLoader&
EntityManager::getAlunoAgendamentosListLoader() const {
    return alunoAgendamentosLoader;
}

const EntityManager::AgendamentoListLoader&
EntityManager::getHorarioAgendamentosListLoader() const {
    return horarioAgendamentosLoader;
}