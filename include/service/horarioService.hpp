#ifndef HORARIO_SERVICE_HPP
#define HORARIO_SERVICE_HPP

#include "model/horario.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

/**
 * @brief Alias de tipo para o cache de entidades Horario.
 */
using HorarioCache = EntityCache<Horario>;

/**
 * @brief Serviço de negócio responsável pela lógica e manipulação de Horários.
 * * Esta classe gerencia a criação, atualização e exclusão de Horários de
 * disponibilidade, e provê métodos para consultar a disponibilidade.
 */
class HorarioService {
   private:
    EntityManager* manager; /**< Ponteiro para o gerenciador de entidades (IoC
                               Container). */
    const MockConnection&
        connection;     /**< Referência para a conexão de persistência. */
    EventBus& bus;      /**< Referência para o barramento de eventos. */
    HorarioCache cache; /**< Cache local para entidades Horario. */

    /**
     * @brief Converte uma linha de dados brutos (string) em um objeto Horario.
     * @param line A string contendo os dados do horário.
     * @return std::shared_ptr<Horario> O objeto Horario carregado.
     */
    std::shared_ptr<Horario> loadHorario(const std::string& line);

   public:
    /**
     * @brief Construtor da classe HorarioService.
     * * Recebe suas dependências via injeção.
     * @param manager O EntityManager que fornece acesso a outros serviços e
     * loaders.
     * @param connection A conexão de persistência.
     * @param bus O barramento de eventos.
     */
    HorarioService(EntityManager* manager, const MockConnection& connection,
                   EventBus& bus);

    /**
     * @brief Destrutor padrão.
     */
    ~HorarioService() = default;

    /**
     * @brief Lista todos os Horários associados a um Professor.
     * @param id O ID do Professor.
     * @return std::vector<std::shared_ptr<Horario>> Lista de horários.
     */
    std::vector<std::shared_ptr<Horario>> listByIdProfessor(long id);

    /**
     * @brief Exclui todos os Horários criados por um Professor.
     * * Dispara a exclusão de Agendamentos relacionados.
     * @param id O ID do Professor.
     * @return bool True se um ou mais horários foram excluídos.
     */
    bool deleteByIdProfessor(long id);

    /**
     * @brief Exclui um Horário específico pelo seu ID.
     * * Dispara a exclusão de Agendamentos relacionados.
     * @param id O ID do horário a ser excluído.
     * @return bool True se a exclusão foi bem-sucedida.
     */
    bool deleteById(long id);

    /**
     * @brief Cria e salva um novo Horário, inicialmente marcado como
     * disponível.
     * @param idProfessor O ID do professor proprietário.
     * @param inicio O timestamp de início do horário.
     * @param fim O timestamp de fim do horário.
     * @return std::shared_ptr<Horario> O Horário recém-criado.
     */
    std::shared_ptr<Horario> save(long idProfessor, Timestamp inicio,
                                  Timestamp fim);

    /**
     * @brief Busca um Horário pelo seu ID, utilizando o cache.
     * @param id O ID único do horário.
     * @return std::shared_ptr<Horario> O horário encontrado ou nullptr.
     */
    std::shared_ptr<Horario> getById(long id);

    /**
     * @brief Atualiza todos os campos de um Horário (exceto ID).
     * @param id O ID do horário a ser atualizado.
     * @param idProfessor O novo ID do professor (se alterado).
     * @param inicio O novo timestamp de início.
     * @param fim O novo timestamp de fim.
     * @param disponivel O novo status de disponibilidade.
     * @return std::shared_ptr<Horario> O horário atualizado.
     */
    std::shared_ptr<Horario> updateById(long id, long idProfessor,
                                        Timestamp inicio, Timestamp fim,
                                        bool disponivel);

    /**
     * @brief Atualiza apenas o status de disponibilidade de um Horário.
     * * Essa função é usada para marcar um horário como ocupado/livre,
     * geralmente em resposta a eventos de agendamento/cancelamento.
     * @param id O ID do horário.
     * @param disponivel O novo status (true para disponível, false para
     * ocupado).
     * @return std::shared_ptr<Horario> O horário atualizado.
     */
    std::shared_ptr<Horario> updateDisponivelById(long id, bool disponivel);

    /**
     * @brief Verifica se um Horário específico está marcado como disponível.
     * @param id O ID do horário.
     * @return bool True se o horário estiver disponível, false caso contrário.
     */
    bool isDisponivelById(long id);
};

#endif