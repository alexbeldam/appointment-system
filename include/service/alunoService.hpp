#ifndef ALUNO_SERVICE_HPP
#define ALUNO_SERVICE_HPP

#include "model/aluno.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

using AlunoCache = EntityCache<Aluno>;

/**
 * @brief Camada de Serviço (Business Logic) para a entidade Aluno.
 *
 * * Responsável por orquestrar a validação de regras de negócio (unicidade),
 * gerenciar a persistência de dados através do DAL (mock/arquivo) e montar o
 * Modelo de Domínio completo (injetando Agendamentos).
 */
class AlunoService {
   private:
    EntityManager* manager;
    const MockConnection&
        connection;  ///< Conexão simulada com o banco de dados
    EventBus& bus;   ///< Barramento de eventos;
    AlunoCache cache;

    std::shared_ptr<Aluno> loadAluno(const std::string& line);

    std::vector<std::shared_ptr<Aluno>> getByEmail(const std::string& email);

    std::vector<std::shared_ptr<Aluno>> getByMatricula(long matricula);

    bool existsByEmail(std::string email);

    bool existsByEmailAndIdNot(std::string email, long id);

    bool existsByMatricula(long matricula);

    bool existsByMatriculaAndIdNot(long matricula, long id);

   public:
    AlunoService(EntityManager* manager, const MockConnection& connection,
                 EventBus& bus);

    ~AlunoService() = default;

    std::shared_ptr<Aluno> save(const std::string& nome,
                                const std::string& email,
                                const std::string& senha, long matricula);

    std::shared_ptr<Aluno> getById(long id);

    std::shared_ptr<Aluno> getOneByEmail(const std::string& email);

    std::shared_ptr<Aluno> listAll();

    std::shared_ptr<Aluno> updateById(long id, const std::string& nome,
                                      const std::string& email,
                                      const std::string& senha, long matricula);

    bool deleteById(long id);
};

#endif