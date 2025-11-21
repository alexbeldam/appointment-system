#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <memory>

#include "event/bus.hpp"
#include "persistence/mockConnection.hpp"
#include "util/entityList.hpp"

class AgendamentoService;
class AlunoService;
class HorarioService;
class ProfessorService;
class SessionService;
class Professor;
class Aluno;
class Agendamento;
class Horario;

#define AGENDAMENTO_TABLE "agendamentos"
#define ALUNO_TABLE "alunos"
#define HORARIO_TABLE "horarios"
#define PROFESSOR_TABLE "professores"

class EntityManager {
   private:
    std::shared_ptr<AgendamentoService> agendamentoService;
    std::shared_ptr<AlunoService> alunoService;
    std::shared_ptr<HorarioService> horarioService;
    std::shared_ptr<ProfessorService> professorService;
    std::shared_ptr<SessionService> sessionService;

    LoadFunction<Aluno> alunoLoader;
    LoadFunction<Professor> professorLoader;
    LoadFunction<Horario> horarioLoader;

    ListLoaderFunction<Agendamento> alunoAgendamentosLoader;
    ListLoaderFunction<Agendamento> horarioAgendamentosLoader;
    ListLoaderFunction<Horario> horarioListLoader;

   public:
    using AlunoLoader = LoadFunction<Aluno>;
    using ProfessorLoader = LoadFunction<Professor>;
    using HorarioLoader = LoadFunction<Horario>;
    using AgendamentoListLoader = ListLoaderFunction<Agendamento>;
    using HorarioListLoader = ListLoaderFunction<Horario>;

    EntityManager(const MockConnection& connection, EventBus& bus);

    ~EntityManager() = default;

    const std::shared_ptr<AgendamentoService>& getAgendamentoService() const;

    const std::shared_ptr<AlunoService>& getAlunoService() const;

    const std::shared_ptr<HorarioService>& getHorarioService() const;

    const std::shared_ptr<ProfessorService>& getProfessorService() const;

    const std::shared_ptr<SessionService>& getSessionService() const;

    const AlunoLoader& getAlunoLoader() const;

    const ProfessorLoader& getProfessorLoader() const;

    const HorarioLoader& getHorarioLoader() const;

    const HorarioListLoader& getHorarioListLoader() const;

    const AgendamentoListLoader& getAlunoAgendamentosListLoader() const;

    const AgendamentoListLoader& getHorarioAgendamentosListLoader() const;
};

#endif