#ifndef AGENDAMENTO_SERVICE_HPP
#define AGENDAMENTO_SERVICE_HPP

#include "model/agendamento.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

/**
 * @brief Alias de tipo para o cache de entidades Agendamento.
 */
using AgendamentoCache = EntityCache<Agendamento>;

/**
 * @brief Serviço de negócio responsável pela lógica e manipulação de
 * Agendamentos.
 * * Esta classe atua como a camada de Serviço (Service) do padrão MVC,
 * gerenciando o acesso aos dados via MockConnection, utilizando um cache
 * (AgendamentoCache) e notificando o sistema sobre mudanças via EventBus.
 */
class AgendamentoService {
   private:
    EntityManager* manager; /**< Ponteiro para o gerenciador de entidades (IoC
                               Container). */
    const MockConnection&
        connection;         /**< Referência para a conexão de persistência. */
    EventBus& bus;          /**< Referência para o barramento de eventos. */
    AgendamentoCache cache; /**< Cache local para entidades Agendamento. */

    /**
     * @brief Converte uma linha de dados brutos (string) em um objeto
     * Agendamento.
     * @param line A string contendo os dados do agendamento.
     * @return std::shared_ptr<Agendamento> O objeto Agendamento carregado.
     */
    std::shared_ptr<Agendamento> loadAgendamento(const std::string& line);

   public:
    /**
     * @brief Construtor da classe AgendamentoService.
     * * Recebe suas dependências via injeção.
     * @param manager O EntityManager que fornece acesso a outros serviços e
     * loaders.
     * @param connection A conexão de persistência.
     * @param bus O barramento de eventos.
     */
    AgendamentoService(EntityManager* manager, const MockConnection& connection,
                       EventBus& bus);

    /**
     * @brief Destrutor padrão.
     */
    ~AgendamentoService() = default;

    /**
     * @brief Cria e salva um novo Agendamento no estado PENDENTE.
     * @param alunoId O ID do aluno que está agendando.
     * @param horarioId O ID do horário que se quer reservar.
     * @return std::shared_ptr<Agendamento> O Agendamento recém-criado.
     */
    std::shared_ptr<Agendamento> save(long alunoId, long horarioId);

    /**
     * @brief Busca um Agendamento pelo seu ID, utilizando o cache.
     * @param id O ID único do agendamento.
     * @return std::shared_ptr<Agendamento> O agendamento encontrado ou nullptr.
     */
    std::shared_ptr<Agendamento> getById(long id);

    /**
     * @brief Atualiza todos os campos de um Agendamento (exceto ID).
     * @param id O ID do agendamento a ser atualizado.
     * @param alunoId O novo ID do aluno.
     * @param horarioId O novo ID do horário.
     * @param status O novo status do agendamento.
     * @return std::shared_ptr<Agendamento> O agendamento atualizado.
     */
    std::shared_ptr<Agendamento> updateById(long id, long alunoId,
                                            long horarioId,
                                            const Status& status);

    /**
     * @brief Atualiza apenas o status de um Agendamento.
     * * Envia notificação de evento (HorarioOcupadoEvent/HorarioLiberadoEvent)
     * se a mudança de status afetar a disponibilidade do horário.
     * @param id O ID do agendamento.
     * @param status O novo status (CONFIRMADO, CANCELADO, RECUSADO).
     * @return std::shared_ptr<Agendamento> O agendamento atualizado.
     */
    std::shared_ptr<Agendamento> updateStatusById(long id,
                                                  const Status& status);

    /**
     * @brief Lista todos os Agendamentos associados a um Horário.
     * @param id O ID do Horário.
     * @return std::vector<std::shared_ptr<Agendamento>> Lista de agendamentos.
     */
    std::vector<std::shared_ptr<Agendamento>> listByIdHorario(long id);

    /**
     * @brief Exclui um Agendamento pelo seu ID.
     * * Envia notificação de evento (HorarioLiberadoEvent) se a exclusão
     * estava ocupando um horário.
     * @param id O ID do agendamento a ser excluído.
     * @return bool True se a exclusão foi bem-sucedida.
     */
    bool deleteById(long id);

    /**
     * @brief Lista todos os Agendamentos associados a um Aluno.
     * @param id O ID do Aluno.
     * @return std::vector<std::shared_ptr<Agendamento>> Lista de agendamentos.
     */
    std::vector<std::shared_ptr<Agendamento>> listByIdAluno(long id);

    /**
     * @brief Exclui todos os Agendamentos feitos por um Aluno.
     * * Usado principalmente ao excluir um Aluno.
     * @param id O ID do Aluno.
     * @return bool True se um ou mais agendamentos foram excluídos.
     */
    bool deleteByIdAluno(long id);

    /**
     * @brief Exclui todos os Agendamentos associados a um Horário.
     * * Usado principalmente ao excluir um Horário.
     * @param id O ID do Horário.
     * @return bool True se um ou mais agendamentos foram excluídos.
     */
    bool deleteByIdHorario(long id);
};

#endif