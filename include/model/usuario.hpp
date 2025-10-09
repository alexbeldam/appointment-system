#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

/**
 * @brief Classe base que representa um usuário genérico no sistema.
 * * * Contém os atributos fundamentais de identificação e autenticação comuns
 * a todas as entidades derivadas, como ID, nome, email e senha.
 */
class Usuario {
   private:
    long id;            ///< Identificador único do Usuário.
    std::string nome;   ///< Nome do Usuário.
    std::string email;  ///< Endereço de e-mail do Usuário.
    std::string senha;  ///< Senha do Usuário.

   public:
    /**
     * @brief Construtor padrão. Inicializa o objeto com valores default.
     */
    Usuario();

    /**
     * @brief Construtor completo.
     * * @param id O identificador único.
     * @param nome O nome do Usuário.
     * @param email O e-mail do Usuário.
     * @param senha A senha do Usuário.
     */
    Usuario(long id, const std::string& nome, const std::string& email,
            const std::string& senha);

    /**
     * @brief Destrutor virtual.
     */
    virtual ~Usuario();

    /**
     * @brief Obtém o identificador único (ID) do Usuário.
     * * @return O ID do Usuário.
     */
    long getId() const;

    /**
     * @brief Define o identificador único (ID) do Usuário.
     * * @param id O novo ID.
     */
    void setId(long id);

    /**
     * @brief Obtém o nome do Usuário.
     * * @return Uma referência constante para a string do nome.
     */
    const std::string& getNome() const;

    /**
     * @brief Define o nome do Usuário.
     * * @param nome O novo nome.
     */
    void setNome(const std::string& nome);

    /**
     * @brief Obtém o e-mail do Usuário.
     * * @return Uma referência constante para a string do e-mail.
     */
    const std::string& getEmail() const;

    /**
     * @brief Define o e-mail do Usuário.
     * * @param email O novo e-mail.
     */
    void setEmail(const std::string& email);

    /**
     * @brief Obtém a senha do Usuário.
     * * @return Uma referência constante para a string da senha.
     */
    const std::string& getSenha() const;

    /**
     * @brief Define a senha do Usuário.
     * * @param senha A nova senha.
     */
    void setSenha(const std::string& senha);
};

#endif