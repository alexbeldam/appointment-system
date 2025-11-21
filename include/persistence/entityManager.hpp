#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <memory>

#include "event/bus.hpp"
#include "persistence/mockConnection.hpp"
#include "util/entityList.hpp"

// Declarações forward de classes de serviço e entidade
class AgendamentoService;
class AlunoService;
class HorarioService;
class ProfessorService;
class SessionService;
class Professor;
class Aluno;
class Agendamento;
class Horario;

/**
 * @brief Definições das constantes para nomes de tabelas de dados.
 */
#define AGENDAMENTO_TABLE "agendamentos"
#define ALUNO_TABLE "alunos"
#define HORARIO_TABLE "horarios"
#define PROFESSOR_TABLE "professores"

/**
 * @brief Classe central que gerencia a inicialização e o acesso a todos os
 * serviços de negócio (Services) e às funções de carregamento (Loaders) de
 * entidades.
 * * Atua como um Contêiner de Inversão de Controle (IoC) ou Factory para a
 * camada de serviços, garantindo que as dependências sejam configuradas
 * corretamente.
 */
class EntityManager {
   private:
    // Serviços de negócio
    std::shared_ptr<AgendamentoService> agendamentoService;
    std::shared_ptr<AlunoService> alunoService;
    std::shared_ptr<HorarioService> horarioService;
    std::shared_ptr<ProfessorService> professorService;
    std::shared_ptr<SessionService> sessionService;

    // Funções de carregamento (Loaders) de entidades individuais (Lazy Load)
    LoadFunction<Aluno> alunoLoader;
    LoadFunction<Professor> professorLoader;
    LoadFunction<Horario> horarioLoader;

    // Funções de carregamento (Loaders) de listas de entidades
    ListLoaderFunction<Agendamento> alunoAgendamentosLoader;
    ListLoaderFunction<Agendamento> horarioAgendamentosLoader;
    ListLoaderFunction<Horario> horarioListLoader;

   public:
    /**
     * @brief Alias para a função de carregamento de um único Aluno.
     */
    using AlunoLoader = LoadFunction<Aluno>;

    /**
     * @brief Alias para a função de carregamento de um único Professor.
     */
    using ProfessorLoader = LoadFunction<Professor>;

    /**
     * @brief Alias para a função de carregamento de um único Horario.
     */
    using HorarioLoader = LoadFunction<Horario>;

    /**
     * @brief Alias para a função de carregamento de uma lista de Agendamentos.
     */
    using AgendamentoListLoader = ListLoaderFunction<Agendamento>;

    /**
     * @brief Alias para a função de carregamento de uma lista de Horarios.
     */
    using HorarioListLoader = ListLoaderFunction<Horario>;

    /**
     * @brief Construtor da classe EntityManager.
     * * Inicializa todos os serviços de negócio, injetando as dependências
     * necessárias (como a MockConnection e o EventBus) e configurando as
     * funções de carregamento.
     * @param connection Uma referência constante à conexão de persistência
     * (MockConnection).
     * @param bus Uma referência ao barramento de eventos do sistema (EventBus).
     */
    EntityManager(const MockConnection& connection, EventBus& bus);

    /**
     * @brief Destrutor padrão.
     */
    ~EntityManager() = default;

    /**
     * @brief Retorna o serviço de gerenciamento de Agendamentos.
     * @return const std::shared_ptr<AgendamentoService>& O serviço de
     * Agendamento.
     */
    const std::shared_ptr<AgendamentoService>& getAgendamentoService() const;

    /**
     * @brief Retorna o serviço de gerenciamento de Alunos.
     * @return const std::shared_ptr<AlunoService>& O serviço de Aluno.
     */
    const std::shared_ptr<AlunoService>& getAlunoService() const;

    /**
     * @brief Retorna o serviço de gerenciamento de Horários.
     * @return const std::shared_ptr<HorarioService>& O serviço de Horário.
     */
    const std::shared_ptr<HorarioService>& getHorarioService() const;

    /**
     * @brief Retorna o serviço de gerenciamento de Professores.
     * @return const std::shared_ptr<ProfessorService>& O serviço de Professor.
     */
    const std::shared_ptr<ProfessorService>& getProfessorService() const;

    /**
     * @brief Retorna o serviço de gerenciamento de Sessão de Usuário.
     * @return const std::shared_ptr<SessionService>& O serviço de Sessão.
     */
    const std::shared_ptr<SessionService>& getSessionService() const;

    /**
     * @brief Retorna a função de carregamento para um único Aluno.
     * * Usada por outras entidades (ex: Agendamento) para Lazy Load.
     * @return const AlunoLoader& A função de carregamento.
     */
    const AlunoLoader& getAlunoLoader() const;

    /**
     * @brief Retorna a função de carregamento para um único Professor.
     * * Usada por outras entidades (ex: Horario) para Lazy Load.
     * @return const ProfessorLoader& A função de carregamento.
     */
    const ProfessorLoader& getProfessorLoader() const;

    /**
     * @brief Retorna a função de carregamento para um único Horario.
     * * Usada por outras entidades (ex: Agendamento) para Lazy Load.
     * @return const HorarioLoader& A função de carregamento.
     */
    const HorarioLoader& getHorarioLoader() const;

    /**
     * @brief Retorna a função de carregamento para uma lista de Horários.
     * @return const HorarioListLoader& A função de carregamento de lista.
     */
    const HorarioListLoader& getHorarioListLoader() const;

    /**
     * @brief Retorna a função de carregamento para a lista de Agendamentos de
     * um Aluno.
     * * Usada pela entidade Aluno para Lazy Load de seus agendamentos.
     * @return const AgendamentoListLoader& A função de carregamento de lista.
     */
    const AgendamentoListLoader& getAlunoAgendamentosListLoader() const;

    /**
     * @brief Retorna a função de carregamento para a lista de Agendamentos de
     * um Horário.
     * * Usada pela entidade Horario para Lazy Load de seus agendamentos.
     * @return const AgendamentoListLoader& A função de carregamento de lista.
     */
    const AgendamentoListLoader& getHorarioAgendamentosListLoader() const;
};

#endif