#ifndef HORARIO_SERVICE_HPP
#define HORARIO_SERVICE_HPP

#include <vector>

#include "data/mockConnection.hpp"
#include "event/bus.hpp"
#include "model/horario.hpp"

/**
 * @brief CLASSE INCOMPLETA: Camada de Serviço (Business Logic) para Horario.
 *
 * Classe placeholder criada para satisfazer dependências de injeção e
 * compilação. Não possui lógica funcional de domínio ou persistência
 * implementada.
 */
class HorarioService {
   private:
    const MockConnection&
        connection;       ///< Conexão simulada com o banco de dados
    const EventBus& bus;  ///< Barramento de eventos.

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o Barramento de Eventos. // << NOVO PARÂMETRO
     */
    HorarioService(const MockConnection& connection, const EventBus& bus);

    /**
     * @brief Lista todos os Horarios disponíveis associados a um Professor.
     *
     * @note **PLACEHOLDER:** Atualmente, retorna sempre um vetor vazio.
     * @param id O identificador único (ID) do Professor.
     * @return Um vetor de Horarios disponíveis (atualmente vazio).
     */
    std::vector<Horario> listDisponivelByIdProfessor(long id) const;

    /**
     * @brief Deleta todos os horários que tenham a coluna id_professor igual ao
     * parametro.
     *
     * @param id O id do professor.
     */
    void deleteByIdProfessor(long id) const;
};

#endif