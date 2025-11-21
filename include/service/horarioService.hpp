#ifndef HORARIO_SERVICE_HPP
#define HORARIO_SERVICE_HPP

#include "model/horario.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

using HorarioCache = EntityCache<Horario>;

/**
 * @brief Camada de Serviço (Business Logic) para Horario.
 */
class HorarioService {
   private:
    EntityManager* manager;
    const MockConnection& connection;
    EventBus& bus;
    HorarioCache cache;

    std::shared_ptr<Horario> loadHorario(const std::string& line);

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o Barramento de Eventos.
     */
    HorarioService(EntityManager* manager, const MockConnection& connection,
                   EventBus& bus);

    ~HorarioService() = default;

    /**
     * @brief Lista todos os Horarios associados a um Professor.
     */
    std::vector<std::shared_ptr<Horario>> listByIdProfessor(long id);

    /**
     * @brief Deleta todos os horários que tenham a coluna id_professor igual ao
     * parametro.
     */
    bool deleteByIdProfessor(long id);

    /**
     * @brief Deleta um horário específico pelo seu ID.
     * @param id O ID do horário a ser deletado.
     * @return true se foi deletado, false se não foi encontrado.
     */
    bool deleteById(long id);

    /**
     * @brief Salva um novo horário para um professor.
     */
    std::shared_ptr<Horario> save(long idProfessor, Timestamp inicio,
                                  Timestamp fim);

    // --- MÉTODOS ADICIONADOS PARA SUPORTE AO AGENDAMENTO ---

    /**
     * @brief Busca um horário específico pelo seu ID.
     * Necessário para AgendamentoService verificar a disponibilidade (AC 2).
     *
     * @param id O ID do horário.
     * @return O objeto Horario.
     * @throws std::runtime_error Se o horário não for encontrado.
     */
    std::shared_ptr<Horario> getById(long id);

    std::shared_ptr<Horario> updateById(long id, long idProfessor,
                                        Timestamp inicio, Timestamp fim,
                                        bool disponivel);

    std::shared_ptr<Horario> updateDisponivelById(long id, bool disponivel);

    /**
     * @brief Verifica se um horário está disponível pelo seu ID.
     * @param id O ID do horário.
     * @return true se estiver disponível, false caso contrário.
     */
    bool isDisponivelById(long id);
};

#endif