#ifndef LOGIN_CONTROLLER_HPP
#define LOGIN_CONTROLLER_HPP

#include "event/bus.hpp"
#include "service/alunoService.hpp"
#include "service/professorService.hpp"

/**
 * @brief Controller para gerenciamento de autenticação (Login).
 * * Esta classe atua como a camada de controle (Controller) do padrão MVC,
 * responsável por receber requisições de login, delegar a lógica de negócio
 * para as camadas de Service (AlunoService e ProfessorService) e, em caso
 * de sucesso, notificar o sistema via EventBus.
 */
class LoginController {
   private:
    /**
     * @brief Ponteiro inteligente para o serviço de alunos.
     * * Usado para autenticar usuários do tipo Aluno.
     */
    const std::shared_ptr<AlunoService>& alunoService;

    /**
     * @brief Ponteiro inteligente para o serviço de professores.
     * * Usado para autenticar usuários do tipo Professor.
     */
    const std::shared_ptr<ProfessorService>& professorService;

    /**
     * @brief Referência ao barramento de eventos do sistema.
     * * Usado para notificar eventos como login bem-sucedido.
     */
    EventBus& bus;

   public:
    /**
     * @brief Construtor da classe LoginController.
     * * Os serviços e o barramento de eventos são injetados via dependência.
     * * @param alunoService O serviço de alunos.
     * @param professorService O serviço de professores.
     * @param bus O barramento de eventos do sistema.
     */
    LoginController(const std::shared_ptr<AlunoService>& alunoService,
                    const std::shared_ptr<ProfessorService>& professorService,
                    EventBus& bus);

    /**
     * @brief Destrutor padrão.
     */
    ~LoginController() = default;

    /**
     * @brief Tenta realizar o login de um Aluno.
     * * Delega a autenticação para o AlunoService. Em caso de sucesso, notifica
     * o EventBus.
     * * @param email O email do aluno.
     * @param senha A senha do aluno.
     * @return std::shared_ptr<Aluno> O objeto Aluno autenticado se o login for
     * bem-sucedido.
     * @return nullptr Se o login falhar (ex: credenciais inválidas).
     */
    std::shared_ptr<Aluno> loginAluno(std::string email, std::string senha);

    /**
     * @brief Tenta realizar o login de um Professor.
     * * Delega a autenticação para o ProfessorService. Em caso de sucesso,
     * notifica o EventBus.
     * * @param email O email do professor.
     * @param senha A senha do professor.
     * @return std::shared_ptr<Professor> O objeto Professor autenticado se o
     * login for bem-sucedido.
     * @return nullptr Se o login falhar (ex: credenciais inválidas).
     */
    std::shared_ptr<Professor> loginProfessor(std::string email,
                                              std::string senha);
};

#endif