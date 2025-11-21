#ifndef ALUNO_CONTROLLER_HPP
#define ALUNO_CONTROLLER_HPP

#include "service/alunoService.hpp"

/**
 * @brief Classe responsável por controlar o fluxo de dados entre a camada de
 * apresentação (I/O) e a camada de serviço (lógica de negócio).
 *
 */
class AlunoController {
   private:
    const std::shared_ptr<AlunoService>& service;

   public:
    AlunoController(const std::shared_ptr<AlunoService>& service);

    ~AlunoController() = default;

    /**
     * @brief Cria um novo registro na tabela, após validações de formato.
     * * @return O aluno criado
     */
    std::shared_ptr<Aluno> create(const std::string& nome,
                                  const std::string& email,
                                  const std::string& senha, long matricula);

    /**
     * @brief Lê o registro com o id passado.
     * * @return O aluno com o id parametro
     */
    std::shared_ptr<Aluno> read(long id);

    /**
     * @brief Atualiza o registro com o id parametro. Implementa lógica de
     * 'patch': campos vazios ou inválidos são ignorados e o valor existente é
     * mantido.
     * * @return O aluno atualizado
     */
    std::shared_ptr<Aluno> update(long id, const std::string& nome,
                                  const std::string& email,
                                  const std::string& senha, long matricula);

    /**
     * @brief Deleta o registro de id parametro.
     * * @return true se a operação suceder e o registro for
     * encontrado/deletado.
     * @return false se a operação falhar (registro não encontrado).
     */
    bool destroy(long id);
};

#endif