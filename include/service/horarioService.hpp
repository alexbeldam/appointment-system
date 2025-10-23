#ifndef HORARIO_SERVICE_HPP
#define HORARIO_SERVICE_HPP

#include <vector>
#include <string>

#include "data/mockConnection.hpp"
#include "event/bus.hpp"
#include "model/horario.hpp"
#include "event/events.hpp"

/**
 * @brief Camada de Serviço (Business Logic) para Horario.
 */
class HorarioService {
private:
    const MockConnection& connection;
    EventBus& bus;

public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o Barramento de Eventos.
     */
    HorarioService(const MockConnection& connection, EventBus& bus);

    /**
     * @brief Lista todos os Horarios disponíveis associados a um Professor.
     */
    std::vector<Horario> listDisponivelByIdProfessor(long id) const;

    /**
     * @brief Lista todos os Horarios associados a um Professor.
     */
    std::vector<Horario> listByIdProfessor(long id) const;

    /**
     * @brief Deleta todos os horários que tenham a coluna id_professor igual ao
     * parametro.
     */
    bool deleteByIdProfessor(long id) const;

    /**
     * @brief Deleta um horário específico pelo seu ID.
     * @param id O ID do horário a ser deletado.
     * @return true se foi deletado, false se não foi encontrado.
     */
    bool deleteById(long id) const;

    /**
     * @brief Salva um novo horário para um professor.
     */
    Horario save(long idProfessor, const std::string& inicio,
                 const std::string& fim);

    // --- MÉTODOS ADICIONADOS PARA SUPORTE AO AGENDAMENTO ---

    /**
     * @brief Busca um horário específico pelo seu ID.
     * Necessário para AgendamentoService verificar a disponibilidade (AC 2).
     *
     * @param id O ID do horário.
     * @return O objeto Horario.
     * @throws std::runtime_error Se o horário não for encontrado.
     */
    Horario getById(long id) const;

    /**
     * @brief Atualiza o status de um horário para "indisponível" (reservado).
     * Necessário para AgendamentoService após criar um agendamento (AC 1).
     *
     * @param id O ID do horário a ser atualizado.
     * @throws std::runtime_error Se o horário não for encontrado.
     */
    void marcarComoReservado(long id) const;
};

#endif