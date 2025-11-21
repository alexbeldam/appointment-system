#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

#include "util/entityList.hpp"

/**
 * @brief Representa a classe base abstrata para todos os
 * usuários do sistema (ex: Aluno, Professor).
 * * Contém os atributos básicos de identificação e autenticação.
 */
class Usuario {
   private:
    long id;           /**< O identificador único do usuário. */
    std::string nome;  /**< O nome completo do usuário. */
    std::string email; /**< O endereço de email, usado como login. */
    std::string senha; /**< A senha do usuário. */

   public:
    /**
     * @brief Construtor da classe Usuario.
     * @param id O identificador único.
     * @param nome O nome do usuário.
     * @param email O email do usuário.
     * @param senha A senha do usuário.
     */
    Usuario(long id, const std::string& nome, const std::string& email,
            const std::string& senha);

    /**
     * @brief Destrutor virtual padrão.
     */
    virtual ~Usuario();

    /**
     * @brief Retorna o ID único do usuário.
     * @return long O ID.
     */
    long getId() const;

    /**
     * @brief Define um novo ID para o usuário.
     * @param id O novo ID.
     */
    void setId(long id);

    /**
     * @brief Retorna o nome do usuário.
     * @return const std::string& O nome.
     */
    const std::string& getNome() const;

    /**
     * @brief Define um novo nome para o usuário.
     * @param nome O novo nome.
     */
    void setNome(const std::string& nome);

    /**
     * @brief Retorna o email do usuário.
     * @return const std::string& O email.
     */
    const std::string& getEmail() const;

    /**
     * @brief Define um novo email para o usuário.
     * @param email O novo email.
     */
    void setEmail(const std::string& email);

    /**
     * @brief Retorna a senha do usuário.
     * @return const std::string& A senha.
     */
    const std::string& getSenha() const;

    /**
     * @brief Define uma nova senha para o usuário.
     * @param senha A nova senha.
     */
    void setSenha(const std::string& senha);
};

#endif