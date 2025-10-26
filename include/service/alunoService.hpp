#ifndef ALUNO_SERVICE_HPP
#define ALUNO_SERVICE_HPP

#include <optional>

#include "event/bus.hpp"
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
        connection;  ///< Conexão simulada com o banco de dados
    EventBus& bus;   ///< Barramento de eventos;
                     ///< (conversão DTO<->Model).
    const AgendamentoService&
        service;  ///< Referência constante para o Serviço de Agendamentos
                  ///< (injeção de dependência).

    Aluno mapAndInjectAgendamentos(const std::string& csv_line) const;

    std::vector<Aluno> getByEmail(const std::string& email) const;

    std::vector<Aluno> getByMatricula(long matricula) const;

    bool existsByEmail(std::string email) const;

    bool existsByEmailAndIdNot(std::string email, long id) const;

    bool existsByMatricula(long matricula) const;

    bool existsByMatriculaAndIdNot(long matricula, long id) const;

   public:
    AlunoService(const MockConnection& connection, EventBus& bus,
                 const AgendamentoService& service);

    Aluno save(const std::string& nome, const std::string& email,
               const std::string& senha, long matricula) const;

    std::optional<Aluno> getById(long id) const;

    std::optional<Aluno> getOneByEmail(const std::string& email) const;

    std::vector<Aluno> listAll() const;

    std::optional<Aluno> updateById(long id, const std::string& nome,
                                    const std::string& email,
                                    const std::string& senha,
                                    long matricula) const;

    bool deleteById(long id) const;
};

#endif