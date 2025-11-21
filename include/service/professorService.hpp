#ifndef PROFESSOR_SERVICE_HPP
#define PROFESSOR_SERVICE_HPP

#include "model/professor.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

/**
 * @brief Alias de tipo para o cache de entidades Professor.
 */
using ProfessorCache = EntityCache<Professor>;

/**
 * @brief Serviço de negócio responsável pela lógica e manipulação de
 * Professores.
 * * Esta classe gerencia operações CRUD e validações de unicidade para a
 * entidade Professor, utilizando cache e notificação de eventos.
 */
class ProfessorService {
   private:
    EntityManager* manager; /**< Ponteiro para o gerenciador de entidades (IoC
                               Container). */
    const MockConnection&
        connection;       /**< Referência para a conexão de persistência. */
    EventBus& bus;        /**< Referência para o barramento de eventos. */
    ProfessorCache cache; /**< Cache local para entidades Professor. */

    /**
     * @brief Converte uma linha de dados brutos (string) em um objeto
     * Professor.
     * @param line A string contendo os dados do professor.
     * @return std::shared_ptr<Professor> O objeto Professor carregado.
     */
    std::shared_ptr<Professor> loadProfessor(const std::string& line);

    /**
     * @brief Busca todos os professores que correspondem ao email fornecido.
     * * Usado para validação e busca de login.
     * @param email O email a ser buscado.
     * @return std::vector<std::shared_ptr<Professor>> Lista de professores
     * encontrados.
     */
    std::vector<std::shared_ptr<Professor>> getByEmail(
        const std::string& email);

    /**
     * @brief Verifica a existência de um professor com o email fornecido.
     * @param email O email a ser verificado.
     * @return bool True se existir um professor com o email.
     */
    bool existsByEmail(std::string email);

    /**
     * @brief Verifica a existência de um professor com o email fornecido,
     * excluindo o ID.
     * * Útil para validação de unicidade durante atualizações.
     * @param email O email a ser verificado.
     * @param id O ID do professor a ser ignorado na busca.
     * @return bool True se existir outro professor com o mesmo email.
     */
    bool existsByEmailAndIdNot(std::string email, long id);

   public:
    /**
     * @brief Construtor da classe ProfessorService.
     * * Recebe suas dependências via injeção.
     * @param manager O EntityManager que fornece acesso a outros serviços e
     * loaders.
     * @param connection A conexão de persistência.
     * @param bus O barramento de eventos.
     */
    ProfessorService(EntityManager* manager, const MockConnection& connection,
                     EventBus& bus);

    /**
     * @brief Destrutor padrão.
     */
    ~ProfessorService() = default;

    /**
     * @brief Cria e salva um novo Professor.
     * * Executa validações de unicidade para email.
     * @param nome O nome do professor.
     * @param email O email único.
     * @param senha A senha.
     * @param disciplina A disciplina lecionada.
     * @return std::shared_ptr<Professor> O Professor recém-criado.
     */
    std::shared_ptr<Professor> save(const std::string& nome,
                                    const std::string& email,
                                    const std::string& senha,
                                    const std::string& disciplina);

    /**
     * @brief Busca um Professor pelo seu ID, utilizando o cache.
     * @param id O ID único do professor.
     * @return std::shared_ptr<Professor> O professor encontrado ou nullptr.
     */
    std::shared_ptr<Professor> getById(long id);

    /**
     * @brief Busca um único Professor pelo seu email (usado principalmente para
     * login).
     * @param email O email do professor.
     * @return std::shared_ptr<Professor> O professor encontrado ou nullptr.
     */
    std::shared_ptr<Professor> getOneByEmail(const std::string& email);

    /**
     * @brief Lista todos os Professores cadastrados no sistema.
     * @return std::vector<std::shared_ptr<Professor>> Uma lista de todos os
     * professores.
     */
    std::vector<std::shared_ptr<Professor>> listAll();

    /**
     * @brief Atualiza as informações de um Professor existente.
     * * Executa validações de unicidade para email, excluindo o próprio ID.
     * @param id O ID do professor a ser atualizado.
     * @param nome O novo nome.
     * @param email O novo email.
     * @param senha A nova senha.
     * @param disciplina A nova disciplina.
     * @return std::shared_ptr<Professor> O professor atualizado ou nullptr se o
     * ID não for encontrado.
     */
    std::shared_ptr<Professor> updateById(long id, const std::string& nome,
                                          const std::string& email,
                                          const std::string& senha,
                                          const std::string& disciplina);

    /**
     * @brief Exclui um Professor pelo seu ID.
     * * Dispara a exclusão de todos os Horários e Agendamentos associados e
     * notifica via EventBus (ProfessorDeletedEvent).
     * @param id O ID do professor a ser excluído.
     * @return bool True se a exclusão foi bem-sucedida.
     */
    bool deleteById(long id);
};

#endif