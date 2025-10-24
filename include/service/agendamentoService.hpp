#ifndef AGENDAMENTO_SERVICE_HPP
#define AGENDAMENTO_SERVICE_HPP

#include <vector>

#include "data/mockConnection.hpp"
#include "event/bus.hpp"
#include "model/agendamento.hpp"
#include "service/horarioService.hpp"

/**
 * @brief Simula a Camada de Serviço para a entidade Agendamento.
 */
class AgendamentoService {
   private:
    const MockConnection& connection;
    EventBus& bus;
    const HorarioService& horarioService;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o Barramento de Eventos.
     * @param horarioService Referência para o Serviço de Horários.
     */
    AgendamentoService(const MockConnection& connection, EventBus& bus,
                       const HorarioService& horarioService);

    /**
     * @brief Salva um novo agendamento, aplicando as regras de negócio.
     *
     * Implementa os Critérios de Aceitação 1 e 2 do caso de uso "Agendar Horário".
     *
     * @param agendamento O objeto Agendamento (com id=0) a ser salvo.
     * @return O objeto Agendamento salvo (com o ID real definido pelo DB).
     * @throws std::runtime_error Se o horário não estiver disponível (AC 2).
     */
    Agendamento save(const Agendamento& agendamento) const;

    /**
     * @brief Busca um agendamento pelo seu ID único.
     * @param id O ID do agendamento.
     * @return O objeto Agendamento.
     * @throws std::runtime_error Se o agendamento não for encontrado.
     */
    std::optional<Agendamento> getById(long id) const;

    /**
     * @brief Lista todos os agendamentos cadastrados.
     * @return Um vetor de Agendamentos.
     */
    std::vector<Agendamento> listAll() const;

    /**
     * @brief Atualiza os dados de um agendamento existente.
     * @param id O ID do agendamento a ser atualizado.
     * @param agendamento O objeto Agendamento com os dados atualizados.
     * @return O objeto Agendamento após a atualização.
     * @throws std::runtime_error Se o agendamento não for encontrado.
     */
    std::optional<Agendamento> updateById(long id, const Agendamento& agendamento) const;

    /**
     * @brief Deleta um agendamento pelo seu ID.
     * @param id O ID do agendamento a ser deletado.
     * @return true se foi deletado, false se não foi encontrado.
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    bool deleteById(long id) const;

    /**
     * @brief Lista todos os Agendamentos associados a um Aluno específico.
     * @param id O identificador único (ID) do Aluno.
     * @return Um vetor de Agendamentos.
     */
    std::vector<Agendamento> listByIdAluno(long id) const;

    /**
     * @brief Deleta todos os agendamentos que tenham a coluna id_aluno igual ao
     * parametro.
     * @param id O id do aluno.
     * @return true se os registros foram encontrados e deletados.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteByIdAluno(long id) const;

    /**
     * @brief Deleta todos os agendamentos que tenham a coluna id_horario igual
     * ao parametro.
     * @param id O id do horario.
     * @return true se os registros foram encontrados e deletados.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteByIdHorario(long id) const;
};

#endif