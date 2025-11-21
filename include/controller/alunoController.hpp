#ifndef ALUNO_CONTROLLER_HPP
#define ALUNO_CONTROLLER_HPP

#include "service/alunoService.hpp"

/**
 * @brief Controller para gerenciamento de Alunos.
 * * Esta classe atua como a camada de controle (Controller) do padrão MVC,
 * responsável por receber requisições, delegar a lógica de negócio
 * para a camada de Service e retornar o resultado.
 */
class AlunoController {
   private:
    /**
     * @brief Ponteiro inteligente para o serviço de alunos.
     * * Responsável por isolar a lógica de negócio e o acesso aos dados.
     */
    const std::shared_ptr<AlunoService>& service;

   public:
    /**
     * @brief Construtor da classe AlunoController.
     * * @param service O serviço de alunos injetado via dependência.
     */
    AlunoController(const std::shared_ptr<AlunoService>& service);

    /**
     * @brief Destrutor padrão.
     */
    ~AlunoController() = default;

    /**
     * @brief Cria um novo recurso Aluno (Requisição POST).
     * * Validações de formato e integridade são aplicadas antes da criação.
     * * @param nome O nome completo do aluno.
     * @param email O email único do aluno.
     * @param senha A senha para acesso.
     * @param matricula O número de matrícula único do aluno.
     * @return std::shared_ptr<Aluno> O objeto Aluno criado.
     */
    std::shared_ptr<Aluno> create(const std::string& nome,
                                  const std::string& email,
                                  const std::string& senha, long matricula);

    /**
     * @brief Recupera um recurso Aluno pelo seu ID (Requisição GET).
     * * @param id O identificador único do aluno.
     * @return std::shared_ptr<Aluno> O aluno com o ID especificado.
     */
    std::shared_ptr<Aluno> read(long id);

    /**
     * @brief Atualiza parcialmente um recurso Aluno (Requisição PATCH).
     * * Implementa a lógica de atualização parcial (PATCH): campos vazios ou
     * nulos no corpo da requisição são ignorados, e o valor existente é
     * mantido.
     * * @param id O identificador único do aluno a ser atualizado.
     * @param nome O novo nome (opcional).
     * @param email O novo email (opcional).
     * @param senha A nova senha (opcional).
     * @param matricula O novo número de matrícula (opcional).
     * @return std::shared_ptr<Aluno> O objeto Aluno atualizado.
     */
    std::shared_ptr<Aluno> update(long id, const std::string& nome,
                                  const std::string& email,
                                  const std::string& senha, long matricula);

    /**
     * @brief Remove um recurso Aluno pelo seu ID (Requisição DELETE).
     * * @param id O identificador único do aluno a ser deletado.
     * @return true Se o aluno for encontrado e deletado com sucesso.
     * @return false Se a operação falhar (ex: aluno não encontrado).
     */
    bool destroy(long id);
};

#endif