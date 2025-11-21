#ifndef AGENDAMENTO_SERVICE_HPP
#define AGENDAMENTO_SERVICE_HPP

#include "model/agendamento.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

using AgendamentoCache = EntityCache<Agendamento>;

/**
 * @brief Simula a Camada de Serviço para a entidade Agendamento.
 */
class AgendamentoService {
   private:
    EntityManager* manager;
    const MockConnection& connection;
    EventBus& bus;
    AgendamentoCache cache;

    std::shared_ptr<Agendamento> loadAgendamento(const std::string& line);

   public:
    AgendamentoService(EntityManager* manager, const MockConnection& connection,
                       EventBus& bus);

    ~AgendamentoService() = default;

    /**
     * @brief Salva um novo agendamento, aplicando as regras de negócio.
     *
     * Implementa os Critérios de Aceitação 1 e 2 do caso de uso "Agendar
     * Horário".
     *
     * @param alunoId O ID do aluno.
     * @param horarioId O ID do horário.
     * @return O objeto Agendamento salvo.
     * @throws std::runtime_error Se o horário não estiver disponível (AC 2).
     */
    std::shared_ptr<Agendamento> save(long alunoId, long horarioId);

    /**
     * @brief Busca um agendamento pelo seu ID único.
     * @param id O ID do agendamento.
     * @return O objeto Agendamento.
     * @throws std::runtime_error Se o agendamento não for encontrado.
     */
    std::shared_ptr<Agendamento> getById(long id);

    /**
     * @brief Atualiza os dados de um agendamento existente.
     * @param id O ID do agendamento a ser atualizado.
     * @param alunoId O ID do aluno.
     * @param horarioId O ID do horário.
     * @param status O novo status do agendamento.
     * @return std::optional<Agendamento>
     */
    std::shared_ptr<Agendamento> updateById(long id, long alunoId,
                                            long horarioId,
                                            const Status& status);

    std::shared_ptr<Agendamento> updateStatusById(long id,
                                                  const Status& status);

    std::vector<std::shared_ptr<Agendamento>> listByIdHorario(long id);

    /**
     * @brief Deleta um agendamento pelo seu ID.
     * @param id O ID do agendamento a ser deletado.
     * @return true se foi deletado, false se não foi encontrado.
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    bool deleteById(long id);

    /**
     * @brief Lista todos os Agendamentos associados a um Aluno específico.
     * @param id O identificador único (ID) do Aluno.
     * @return Um vetor de Agendamentos.
     */
    std::vector<std::shared_ptr<Agendamento>> listByIdAluno(long id);

    /**
     * @brief Deleta todos os agendamentos que tenham a coluna id_aluno igual ao
     * parametro.
     * @param id O id do aluno.
     * @return true se os registros foram encontrados e deletados.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteByIdAluno(long id);

    /**
     * @brief Deleta todos os agendamentos que tenham a coluna id_horario igual
     * ao parametro.
     * @param id O id do horario.
     * @return true se os registros foram encontrados e deletados.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteByIdHorario(long id);
};

#endif