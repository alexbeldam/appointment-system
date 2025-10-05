#ifndef PROFESSOR_CONTROLLER_HPP
#define PROFESSOR_CONTROLLER_HPP

#include "service/professorService.hpp"

/**
 * @brief Classe responsável por controlar o fluxo de dados entre a camada de
 * apresentação (I/O) e a camada de serviço (lógica de negócio).
 *
 */
class ProfessorController {
   private:
    const ProfessorService& service;

   public:
    ProfessorController(const ProfessorService& service);

    /**
     * @brief Cria um novo registro na tabela, após validações de formato.
     * * @return O professor criado
     */
    Professor create(const std::string& nome, const std::string& email,
                     const std::string& senha,
                     const std::string& disciplina) const;

    /**
     * @brief Lê o registro com o id passado.
     * * @return O professor com o id parametro
     */
    Professor read(long id) const;

    /**
     * @brief Lista todos os professors.
     * * @return Um vetor com todos os professors
     */
    std::vector<Professor> list() const;

    /**
     * @brief Atualiza o registro com o id parametro. Implementa lógica de
     * 'patch': campos vazios ou inválidos são ignorados e o valor existente é
     * mantido.
     * * @return O professor atualizado
     */
    Professor update(long id, const std::string& nome, const std::string& email,
                     const std::string& senha, const string& disciplina) const;

    /**
     * @brief Deleta o registro de id parametro.
     * * @return true se a operação suceder e o registro for
     * encontrado/deletado.
     * @return false se a operação falhar (registro não encontrado).
     */
    bool destroy(long id) const;
};

#endif