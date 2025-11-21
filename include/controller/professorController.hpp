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
    const std::shared_ptr<ProfessorService>& service;

   public:
    ProfessorController(const std::shared_ptr<ProfessorService>& service);

    ~ProfessorController() = default;

    /**
     * @brief Cria um novo registro na tabela, após validações de formato.
     * * @return O professor criado
     */
    std::shared_ptr<Professor> create(const std::string& nome,
                                      const std::string& email,
                                      const std::string& senha,
                                      const std::string& disciplina);

    /**
     * @brief Lê o registro com o id passado.
     * * @return O professor com o id parametro
     */
    std::shared_ptr<Professor> read(long id);

    /**
     * @brief Lista todos os professors.
     * * @return Um vetor com todos os professors
     */
    std::vector<std::shared_ptr<Professor>> list();

    /**
     * @brief Atualiza o registro com o id parametro. Implementa lógica de
     * 'patch': campos vazios ou inválidos são ignorados e o valor existente é
     * mantido.
     * * @return O professor atualizado
     */
    std::shared_ptr<Professor> update(long id, const std::string& nome,
                                      const std::string& email,
                                      const std::string& senha,
                                      const std::string& disciplina);

    /**
     * @brief Deleta o registro de id parametro.
     * * @return true se a operação suceder e o registro for
     * encontrado/deletado.
     * @return false se a operação falhar (registro não encontrado).
     */
    bool destroy(long id);
};

#endif