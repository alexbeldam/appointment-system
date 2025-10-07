#ifndef ALUNO_SERVICE_HPP
#define ALUNO_SERVICE_HPP

#include <optional>

#include "event/bus.hpp"
#include "map/alunoMapper.hpp"
#include "service/agendamentoService.hpp"

/**
 * @brief Camada de Serviço (Business Logic) para a entidade Aluno.
 *
 * * Responsável por orquestrar a validação de regras de negócio (unicidade),
 * gerenciar a persistência de dados através do DAL (mock/arquivo) e montar o
 * Modelo de Domínio completo (injetando Agendamentos).
 */
class AlunoService {
   private:
    const MockConnection&
        connection;             ///< Conexão simulada com o banco de dados
    const EventBus& bus;        ///< Barramento de eventos;
    const AlunoMapper& mapper;  ///< Referência constante para o Mapper
                                ///< (conversão DTO<->Model).
    const AgendamentoService&
        service;  ///< Referência constante para o Serviço de Agendamentos
                  ///< (injeção de dependência).

    /**
     * @brief Converte uma linha CSV em um objeto Aluno completo e injeta seus
     * Agendamentos.
     * * Método privado que encapsula a lógica de mapeamento e a busca por dados
     * relacionados.
     * * @param csv_line A linha CSV do registro de Aluno.
     * @return O objeto Aluno (Modelo de Domínio) completo.
     * @throws std::runtime_error Se houver falha na conversão de tipos ou na
     * leitura de I/O.
     */
    Aluno mapAndInjectAgendamentos(const std::string& csv_line) const;

    // --- Métodos Privados de Busca e Validação ---

    /**
     * @brief Busca todos os registros de Aluno que possuem o email
     * especificado.
     * * @return Um vetor contendo os Alunos encontrados (pode ser vazio).
     */
    std::vector<Aluno> getByEmail(const std::string& email) const;

    /**
     * @brief Busca todos os registros de Aluno que possuem a matrícula
     * especificada.
     * * @return Um vetor contendo os Alunos encontrados (pode ser vazio).
     */
    std::vector<Aluno> getByMatricula(long matricula) const;

    /**
     * @brief Verifica se o email já existe no sistema.
     * * @param email O email a ser verificado.
     * @return true se o email já estiver em uso.
     */
    bool existsByEmail(std::string email) const;

    /**
     * @brief Verifica se o email existe para **outro** Aluno, excluindo o ID
     * fornecido.
     * * Usado para garantir unicidade em operações de atualização (update).
     * * @param email O email a ser verificado.
     * @param id O ID do Aluno a ser excluído da verificação.
     * @return true se o email for encontrado em outro registro.
     */
    bool existsByEmailAndIdNot(std::string email, long id) const;

    /**
     * @brief Verifica se a matrícula já existe no sistema.
     * * @param matricula A matrícula a ser verificada.
     * @return true se a matrícula já estiver em uso.
     */
    bool existsByMatricula(long matricula) const;

    /**
     * @brief Verifica se a matrícula existe para **outro** Aluno, excluindo o
     * ID fornecido.
     * * Usado para garantir unicidade em operações de atualização (update).
     * * @param matricula A matrícula a ser verificada.
     * @param id O ID do Aluno a ser excluído da verificação.
     * @return true se a matrícula for encontrada em outro registro.
     */
    bool existsByMatriculaAndIdNot(long matricula, long id) const;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o EventBus. <--- PARÂMETRO ADICIONADO
     * @param mapper Referência para o AlunoMapper.
     * @param service Referência para o AgendamentoService.
     */
    AlunoService(const MockConnection& connection, const EventBus& bus,
                 const AlunoMapper& mapper, const AgendamentoService& service);

    // --- Métodos Públicos (CRUD) ---

    /**
     * @brief Realiza a validação, verifica a unicidade e persiste um novo
     * registro de Aluno.
     * * @param aluno O DTO com os dados do novo Aluno.
     * @return O objeto Aluno (Modelo) criado e completo, incluindo o novo ID.
     * @throws std::invalid_argument Se houver violação de regra de negócio
     * (unicidade).
     * @throws std::runtime_error Em caso de falha de I/O na persistência.
     */
    Aluno save(const AlunoDTO& aluno) const;

    /**
     * @brief Busca um Aluno pelo seu ID.
     * * @param id O ID do Aluno.
     * @return std::optional<Aluno> Se encontrado, retorna o objeto Aluno
     * completo; caso contrário, retorna std::nullopt.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    std::optional<Aluno> getById(long id) const;

    /**
     * @brief Busca um Aluno pelo seu email.
     *
     * @param email O email do Aluno
     * @return std::optional<Aluno> Se encontrado, retorna o objeto Aluno
     * completo; caso contrário, retorna std::nullopt.
     * @throws std::runtime_error Em caso de falha crítica de I/O ou de
     * integridade dos dados.
     */
    std::optional<Aluno> getOneByEmail(const std::string& email) const;

    /**
     * @brief Lista todos os Alunos existentes no sistema.
     * * @return Um vetor contendo todos os Alunos (pode ser vazio).
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    std::vector<Aluno> listAll() const;

    /**
     * @brief Atualiza um registro de Aluno existente.
     * * Verifica unicidade, aplica o patch (ignora campos vazios) e persiste a
     * mudança.
     * * @param id O ID do Aluno a ser atualizado.
     * @param aluno O DTO com os dados atualizados.
     * @return std::optional<Aluno> O Aluno atualizado se o ID for encontrado;
     * caso contrário, retorna std::nullopt.
     * @throws std::invalid_argument Se houver violação de regra de negócio
     * (unicidade).
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    std::optional<Aluno> updateById(long id, const AlunoDTO& aluno) const;

    /**
     * @brief Deleta um registro de Aluno pelo ID.
     * * @param id O ID do Aluno a ser deletado.
     * @return true se o registro foi encontrado e deletado; false se o registro
     * não foi encontrado.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteById(long id) const;
};

#endif