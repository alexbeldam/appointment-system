#ifndef PROFESSOR_SERVICE_HPP
#define PROFESSOR_SERVICE_HPP

#include <optional>

#include "event/bus.hpp"
#include "map/professorMapper.hpp"
#include "service/horarioService.hpp"

/**
 * @brief Camada de Serviço (Business Logic) para a entidade Professor.
 *
 * * Responsável por orquestrar a validação de regras de negócio (unicidade),
 * gerenciar a persistência de dados através do DAL (mock/arquivo) e montar o
 * Modelo de Domínio completo (injetando Horarios).
 */
class ProfessorService {
   private:
    const MockConnection&
        connection;                 ///< Conexão simulada com o banco de dados
    const EventBus& bus;            ///< Barramento de eventos
    const ProfessorMapper& mapper;  ///< Referência constante para o Mapper
                                    ///< (conversão DTO<->Model).
    const HorarioService& service;  ///< Referência constante para o Serviço de
                                    ///< Horarios (injeção de dependência).

    /**
     * @brief Converte uma linha CSV em um objeto Professor completo e injeta
     * seus Horarios.
     * * Método privado que encapsula a lógica de mapeamento e a busca por dados
     * relacionados.
     * * @param csv_line A linha CSV do registro de Professor.
     * @return O objeto Professor (Modelo de Domínio) completo.
     * @throws std::runtime_error Se houver falha na conversão de tipos ou na
     * leitura de I/O.
     */
    Professor mapAndInjectHorarios(const std::string& csv_line) const;

    // --- Métodos Privados de Busca e Validação ---

    /**
     * @brief Busca todos os registros de Professor que possuem o email
     * especificado.
     * * @return Um vetor contendo os Professors encontrados (pode ser vazio).
     */
    std::vector<Professor> getByEmail(const std::string& email) const;

    /**
     * @brief Verifica se o email já existe no sistema.
     * * @param email O email a ser verificado.
     * @return true se o email já estiver em uso.
     */
    bool existsByEmail(std::string email) const;

    /**
     * @brief Verifica se o email existe para **outro** Professor, excluindo o
     * ID fornecido.
     * * Usado para garantir unicidade em operações de atualização (update).
     * * @param email O email a ser verificado.
     * @param id O ID do Professor a ser excluído da verificação.
     * @return true se o email for encontrado em outro registro.
     */
    bool existsByEmailAndIdNot(std::string email, long id) const;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o EventBus.
     * @param mapper Referência para o ProfessorMapper.
     * @param service Referência para o HorarioService.
     */
    ProfessorService(const MockConnection& connection, const EventBus& bus,
                     const ProfessorMapper& mapper,
                     const HorarioService& service);

    // --- Métodos Públicos (CRUD) ---

    /**
     * @brief Realiza a validação, verifica a unicidade e persiste um novo
     * registro de Professor.
     * * @param professor O DTO com os dados do novo Professor.
     * @return O objeto Professor (Modelo) criado e completo, incluindo o novo
     * ID.
     * @throws std::invalid_argument Se houver violação de regra de negócio
     * (unicidade).
     * @throws std::runtime_error Em caso de falha de I/O na persistência.
     */
    Professor save(const ProfessorDTO& professor) const;

    /**
     * @brief Busca um Professor pelo seu ID.
     * * @param id O ID do Professor.
     * @return std::optional<Professor> Se encontrado, retorna o objeto
     * Professor completo; caso contrário, retorna std::nullopt.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    std::optional<Professor> getById(long id) const;

    /**
     * @brief Busca um Professor pelo seu email.
     *
     * @param email O email do Professor
     * @return std::optional<Professor> Se encontrado, retorna o objeto
     * Professor completo; caso contrário, retorna std::nullopt.
     * @throws std::runtime_error Em caso de falha crítica de I/O ou de
     * integridade dos dados.
     */
    std::optional<Professor> getOneByEmail(const std::string& email) const;

    /**
     * @brief Lista todos os Professors existentes no sistema.
     * * @return Um vetor contendo todos os Professors (pode ser vazio).
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    std::vector<Professor> listAll() const;

    /**
     * @brief Atualiza um registro de Professor existente.
     * * Verifica unicidade, aplica o patch (ignora campos vazios) e persiste a
     * mudança.
     * * @param id O ID do Professor a ser atualizado.
     * @param professor O DTO com os dados atualizados.
     * @return std::optional<Professor> O Professor atualizado se o ID for
     * encontrado; caso contrário, retorna std::nullopt.
     * @throws std::invalid_argument Se houver violação de regra de negócio
     * (unicidade).
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    std::optional<Professor> updateById(long id,
                                        const ProfessorDTO& professor) const;

    /**
     * @brief Deleta um registro de Professor pelo ID.
     * * @param id O ID do Professor a ser deletado.
     * @return true se o registro foi encontrado e deletado; false se o registro
     * não foi encontrado.
     * @throws std::runtime_error Em caso de falha crítica de I/O.
     */
    bool deleteById(long id) const;
};

#endif