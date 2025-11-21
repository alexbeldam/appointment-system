#ifndef PROFESSOR_CONTROLLER_HPP
#define PROFESSOR_CONTROLLER_HPP

#include "service/professorService.hpp"

/**
 * @brief Controller para gerenciamento de entidades Professor.
 * * Esta classe atua como a camada de controle (Controller) do padrão MVC,
 * responsável por receber requisições, delegar a lógica de negócio
 * para a camada de ProfessorService e retornar o resultado.
 */
class ProfessorController {
   private:
    /**
     * @brief Ponteiro inteligente para o serviço de professores.
     * * Responsável por isolar a lógica de negócio e o acesso aos dados
     * relacionados a Professor.
     */
    const std::shared_ptr<ProfessorService>& service;

   public:
    /**
     * @brief Construtor da classe ProfessorController.
     * * Realiza a injeção de dependência do ProfessorService.
     * * @param service O serviço de professores injetado.
     */
    ProfessorController(const std::shared_ptr<ProfessorService>& service);

    /**
     * @brief Destrutor padrão.
     */
    ~ProfessorController() = default;

    /**
     * @brief Cria e cadastra um novo Professor (Requisição POST).
     * * Delega a criação do novo recurso ao ProfessorService.
     * * @param nome O nome completo do professor.
     * @param email O email único do professor, usado para login.
     * @param senha A senha do professor.
     * @param disciplina A disciplina principal lecionada pelo professor.
     * @return std::shared_ptr<Professor> O objeto Professor recém-criado.
     */
    std::shared_ptr<Professor> create(const std::string& nome,
                                      const std::string& email,
                                      const std::string& senha,
                                      const std::string& disciplina);

    /**
     * @brief Busca um Professor pelo seu ID (Requisição GET).
     * * @param id O identificador único do professor.
     * @return std::shared_ptr<Professor> O objeto Professor encontrado.
     * @return nullptr Se nenhum professor for encontrado com o ID fornecido.
     */
    std::shared_ptr<Professor> read(long id);

    /**
     * @brief Lista todos os Professores cadastrados (Requisição GET).
     * * @return std::vector<std::shared_ptr<Professor>> Uma lista contendo
     * todos os objetos Professor.
     */
    std::vector<std::shared_ptr<Professor>> list();

    /**
     * @brief Atualiza as informações de um Professor (Requisição PUT).
     * * @param id O identificador único do professor a ser atualizado.
     * @param nome O novo nome.
     * @param email O novo email.
     * @param senha A nova senha.
     * @param disciplina A nova disciplina.
     * @return std::shared_ptr<Professor> O objeto Professor atualizado.
     * @return nullptr Se o professor não for encontrado ou a atualização
     * falhar.
     */
    std::shared_ptr<Professor> update(long id, const std::string& nome,
                                      const std::string& email,
                                      const std::string& senha,
                                      const std::string& disciplina);

    /**
     * @brief Exclui um Professor pelo seu ID (Requisição DELETE).
     * * @param id O identificador único do professor a ser excluído.
     * @return true Se o professor foi encontrado e deletado com sucesso.
     * @return false Se a operação falhar (ex: professor não encontrado).
     */
    bool destroy(long id);
};

#endif