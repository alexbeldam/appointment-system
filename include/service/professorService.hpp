#ifndef PROFESSOR_SERVICE_HPP
#define PROFESSOR_SERVICE_HPP

#include <optional>

#include "event/bus.hpp"
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
    EventBus& bus;                  ///< Barramento de eventos
    const HorarioService& service;  ///< Referência constante para o Serviço de
                                    ///< Horarios (injeção de dependência).

    Professor mapAndInjectHorarios(const std::string& csv_line) const;

    std::vector<Professor> getByEmail(const std::string& email) const;

    bool existsByEmail(std::string email) const;

    bool existsByEmailAndIdNot(std::string email, long id) const;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param connection Referência para a conexão com o banco de dados.
     * @param bus Referência para o EventBus.
     * @param service Referência para o HorarioService.
     */
    ProfessorService(const MockConnection& connection, EventBus& bus,
                     const HorarioService& service);

    Professor save(const std::string& nome, const std::string& email,
                   const std::string& senha,
                   const std::string& disciplina) const;

    std::optional<Professor> getById(long id) const;

    std::optional<Professor> getOneByEmail(const std::string& email) const;

    std::vector<Professor> listAll() const;

    std::optional<Professor> updateById(long id, const std::string& nome,
                                        const std::string& email,
                                        const std::string& senha,
                                        const std::string& disciplina) const;

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