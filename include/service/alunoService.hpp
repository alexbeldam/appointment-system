#ifndef ALUNO_SERVICE_HPP
#define ALUNO_SERVICE_HPP

#include "model/aluno.hpp"
#include "persistence/entityCache.hpp"
#include "persistence/entityManager.hpp"
#include "persistence/mockConnection.hpp"

/**
 * @brief Alias de tipo para o cache de entidades Aluno.
 */
using AlunoCache = EntityCache<Aluno>;

/**
 * @brief Serviço de negócio responsável pela lógica e manipulação de Alunos.
 * * Esta classe gerencia operações CRUD e validações de unicidade para a
 * entidade Aluno, utilizando cache e notificação de eventos.
 */
class AlunoService {
   private:
    EntityManager* manager; /**< Ponteiro para o gerenciador de entidades (IoC
                               Container). */
    const MockConnection&
        connection;   /**< Referência para a conexão de persistência. */
    EventBus& bus;    /**< Referência para o barramento de eventos. */
    AlunoCache cache; /**< Cache local para entidades Aluno. */

    /**
     * @brief Converte uma linha de dados brutos (string) em um objeto Aluno.
     * @param line A string contendo os dados do aluno.
     * @return std::shared_ptr<Aluno> O objeto Aluno carregado.
     */
    std::shared_ptr<Aluno> loadAluno(const std::string& line);

    /**
     * @brief Busca todos os alunos que correspondem ao email fornecido.
     * * Usado para validação e busca de login.
     * @param email O email a ser buscado.
     * @return std::vector<std::shared_ptr<Aluno>> Lista de alunos encontrados.
     */
    std::vector<std::shared_ptr<Aluno>> getByEmail(const std::string& email);

    /**
     * @brief Busca todos os alunos que correspondem à matrícula fornecida.
     * @param matricula A matrícula a ser buscada.
     * @return std::vector<std::shared_ptr<Aluno>> Lista de alunos encontrados.
     */
    std::vector<std::shared_ptr<Aluno>> getByMatricula(long matricula);

    /**
     * @brief Verifica a existência de um aluno com o email fornecido.
     * @param email O email a ser verificado.
     * @return bool True se existir um aluno com o email.
     */
    bool existsByEmail(std::string email);

    /**
     * @brief Verifica a existência de um aluno com o email fornecido, excluindo
     * o ID.
     * * Útil para validação de unicidade durante atualizações.
     * @param email O email a ser verificado.
     * @param id O ID do aluno a ser ignorado na busca.
     * @return bool True se existir outro aluno com o mesmo email.
     */
    bool existsByEmailAndIdNot(std::string email, long id);

    /**
     * @brief Verifica a existência de um aluno com a matrícula fornecida.
     * @param matricula A matrícula a ser verificada.
     * @return bool True se existir um aluno com a matrícula.
     */
    bool existsByMatricula(long matricula);

    /**
     * @brief Verifica a existência de um aluno com a matrícula fornecida,
     * excluindo o ID.
     * * Útil para validação de unicidade durante atualizações.
     * @param matricula A matrícula a ser verificada.
     * @param id O ID do aluno a ser ignorado na busca.
     * @return bool True se existir outro aluno com a mesma matrícula.
     */
    bool existsByMatriculaAndIdNot(long matricula, long id);

   public:
    /**
     * @brief Construtor da classe AlunoService.
     * * Recebe suas dependências via injeção.
     * @param manager O EntityManager que fornece acesso a outros serviços e
     * loaders.
     * @param connection A conexão de persistência.
     * @param bus O barramento de eventos.
     */
    AlunoService(EntityManager* manager, const MockConnection& connection,
                 EventBus& bus);

    /**
     * @brief Destrutor padrão.
     */
    ~AlunoService() = default;

    /**
     * @brief Cria e salva um novo Aluno.
     * * Executa validações de unicidade para email e matrícula.
     * @param nome O nome do aluno.
     * @param email O email único.
     * @param senha A senha.
     * @param matricula O número de matrícula único.
     * @return std::shared_ptr<Aluno> O Aluno recém-criado.
     * @throws std::runtime_error Se o email ou a matrícula já existirem.
     */
    std::shared_ptr<Aluno> save(const std::string& nome,
                                const std::string& email,
                                const std::string& senha, long matricula);

    /**
     * @brief Busca um Aluno pelo seu ID, utilizando o cache.
     * @param id O ID único do aluno.
     * @return std::shared_ptr<Aluno> O aluno encontrado ou nullptr.
     */
    std::shared_ptr<Aluno> getById(long id);

    /**
     * @brief Busca um único Aluno pelo seu email (usado principalmente para
     * login).
     * @param email O email do aluno.
     * @return std::shared_ptr<Aluno> O aluno encontrado ou nullptr.
     */
    std::shared_ptr<Aluno> getOneByEmail(const std::string& email);

    /**
     * @brief Atualiza as informações de um Aluno existente.
     * * Executa validações de unicidade para email e matrícula, excluindo o
     * próprio ID.
     * @param id O ID do aluno a ser atualizado.
     * @param nome O novo nome.
     * @param email O novo email.
     * @param senha A nova senha.
     * @param matricula A nova matrícula.
     * @return std::shared_ptr<Aluno> O aluno atualizado ou nullptr se o ID não
     * for encontrado.
     * @throws std::runtime_error Se o novo email ou matrícula já existirem em
     * outro aluno.
     */
    std::shared_ptr<Aluno> updateById(long id, const std::string& nome,
                                      const std::string& email,
                                      const std::string& senha, long matricula);

    /**
     * @brief Exclui um Aluno pelo seu ID.
     * * Dispara a exclusão de todos os Agendamentos associados e notifica via
     * EventBus (AlunoDeletedEvent).
     * @param id O ID do aluno a ser excluído.
     * @return bool True se a exclusão foi bem-sucedida.
     */
    bool deleteById(long id);
};

#endif