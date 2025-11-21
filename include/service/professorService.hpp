#ifndef PROFESSOR_SERVICE_HPP
#define PROFESSOR_SERVICE_HPP

#include "model/professor.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

using ProfessorCache = EntityCache<Professor>;

/**
 * @brief Camada de Serviço (Business Logic) para a entidade Professor.
 *
 * * Responsável por orquestrar a validação de regras de negócio (unicidade),
 * gerenciar a persistência de dados através do DAL (mock/arquivo) e montar o
 * Modelo de Domínio completo (injetando Horarios).
 */
class ProfessorService {
   private:
    EntityManager* manager;
    const MockConnection&
        connection;  ///< Conexão simulada com o banco de dados
    EventBus& bus;   ///< Barramento de eventos
    ProfessorCache cache;

    std::shared_ptr<Professor> loadProfessor(const std::string& line);

    std::vector<std::shared_ptr<Professor>> getByEmail(
        const std::string& email);

    bool existsByEmail(std::string email);

    bool existsByEmailAndIdNot(std::string email, long id);

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o EventBus.
     * @param service Referência para o HorarioService.
     */
    ProfessorService(EntityManager* manager, const MockConnection& connection,
                     EventBus& bus);

    ~ProfessorService() = default;

    std::shared_ptr<Professor> save(const std::string& nome,
                                    const std::string& email,
                                    const std::string& senha,
                                    const std::string& disciplina);

    std::shared_ptr<Professor> getById(long id);

    std::shared_ptr<Professor> getOneByEmail(const std::string& email);

    std::vector<std::shared_ptr<Professor>> listAll();

    std::shared_ptr<Professor> updateById(long id, const std::string& nome,
                                          const std::string& email,
                                          const std::string& senha,
                                          const std::string& disciplina);

    /**
     * @brief Deleta um registro de Professor pelo ID.
     * * @param id O ID do Professor a ser deletado.
     * @return true se o registro foi encontrado e deletado; false se o registro
     * não foi encontrado.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteById(long id);
};

#endif