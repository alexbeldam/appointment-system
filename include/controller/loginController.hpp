#ifndef LOGIN_CONTROLLER_HPP
#define LOGIN_CONTROLLER_HPP

#include "event/bus.hpp"
#include "service/alunoService.hpp"
#include "service/professorService.hpp"

/**
 * @brief Classe responsável por controlar o fluxo de autenticação (login) no
 * sistema.
 *
 * O LoginController recebe as credenciais brutas do usuário e delega a
 * validação do e-mail e senha para as camadas de serviço apropriadas
 * (AlunoService ou ProfessorService). É responsável por **criar uma cópia
 * dinâmica** do objeto autenticado e passá-lo para o SessionManager.
 */
class LoginController {
   private:
    /**
     * @brief Referência constante para o Serviço de Aluno.
     * * Usado para validar credenciais de login de Alunos.
     */
    const AlunoService& alunoService;

    /**
     * @brief Referência constante para o Serviço de Professor.
     * * Usado para validar credenciais de login de Professores.
     */
    const ProfessorService& professorService;

    /**
     * @brief Referência ao EventBus.
     *
     * Usado para publicar o evento de login do usuário.
     */
    EventBus& bus;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * * Recebe e inicializa os serviços necessários para autenticação.
     * @param alunoService Referência para o AlunoService.
     * @param professorService Referência para o ProfessorService.
     * @param bus Referência ao EventBus do sistema.
     */
    LoginController(const AlunoService& alunoService,
                    const ProfessorService& professorService, EventBus& bus);

    /**
     * @brief Tenta autenticar um usuário como Aluno.
     *
     * O método busca o Aluno, verifica a senha e **cria uma cópia dinâmica**
     * do objeto, compartilhando a posse com o SessionManager.
     * @param email O e-mail fornecido pelo usuário.
     * @param senha A senha fornecida em texto simples.
     * @return Um **std::shared_ptr** para o objeto Aluno autenticado (objeto no
     * heap).
     * @throws std::invalid_argument Se o e-mail não for encontrado ou a senha
     * for inválida.
     * @throws std::runtime_error Em caso de falha crítica na camada de
     * Serviço/DAL.
     */
    Aluno loginAluno(std::string email, std::string senha) const;

    /**
     * @brief Tenta autenticar um usuário como Professor.
     *
     * O método busca o Professor, verifica a senha e **cria uma cópia
     * dinâmica** do objeto, compartilhando a posse com o SessionManager.
     * @param email O e-mail fornecido pelo usuário.
     * @param senha A senha fornecida em texto simples.
     * @return Um **std::shared_ptr** para o objeto Professor autenticado
     * (objeto no heap).
     * @throws std::invalid_argument Se o e-mail não for encontrado ou a senha
     * for inválida.
     * @throws std::runtime_error Em caso de falha crítica na camada de
     * Serviço/DAL.
     */
    Professor loginProfessor(std::string email, std::string senha) const;
};

#endif