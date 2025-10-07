#ifndef AGENDAMENTO_SERVICE_HPP
#define AGENDAMENTO_SERVICE_HPP

#include <vector>

#include "data/mockConnection.hpp"
#include "event/bus.hpp"
#include "model/agendamento.hpp"

/**
 * @brief CLASSE INCOMPLETA: Simula a Camada de Serviço para a entidade
 * Agendamento.
 *
 * * Esta classe foi criada apenas para satisfazer as dependências e permitir a
 * compilação e o desenvolvimento da classe AlunoService e AlunoController.
 * * A implementação atual não interage com a Camada de Acesso a Dados (DAL)
 * de forma funcional.
 */
class AgendamentoService {
   private:
    const MockConnection&
        connection;  ///< Conexão simulada com o banco de dados
    EventBus& bus;   ///< Barramento de eventos;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o Barramento de Eventos.
     */
    AgendamentoService(const MockConnection& connection, EventBus& bus);

    /**
     * @brief Lista todos os Agendamentos associados a um Aluno específico.
     * * * Esta função é utilizada para injetar dados relacionados ao Modelo
     * Aluno.
     * * **NOTA DE IMPLEMENTAÇÃO:** Atualmente, a função não possui lógica de
     * busca e retorna sempre um vetor vazio (std::vector<Agendamento>{}) como
     * placeholder.
     * * @param id O identificador único (ID) do Aluno.
     * @return Um vetor **vazio** de Agendamentos.
     */
    std::vector<Agendamento> listByIdAluno(long id) const;

    /**
     * @brief Deleta todos os agendamentos que tenham a coluna id_aluno igual ao
     * parametro.
     *
     * @param id O id do aluno.
     * @return true se os registros foram encontrados e deletados; false se o
     * registro não foi encontrado.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteByIdAluno(long id) const;

    /**
     * @brief Deleta todos os agendamentos que tenham a coluna id_horario igual
     * ao parametro.
     *
     * @param id O id do horario.
     * @return true se os registros foram encontrados e deletados; false se o
     * registro não foi encontrado.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteByIdHorario(long id) const;
};

#endif