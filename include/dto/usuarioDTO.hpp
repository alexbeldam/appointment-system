#ifndef USUARIO_DTO_HPP
#define USUARIO_DTO_HPP

#include <string>

/**
 * @brief Classe Data Transfer Object (DTO) genérica para os usuários do
 * sistema.
 * * Contém os atributos básicos dos usuários
 */
class UsuarioDTO {
   private:
    long id;            ///< Identificador único do Usuario.
    std::string nome;   ///< Nome completo do Usuario.
    std::string email;  ///< Endereço de e-mail único do Usuario.
    std::string senha;  ///< Senha do Usuario.

   public:
    /**
     * @brief Destrutor virtual.
     */
    virtual ~UsuarioDTO();

    /**
     * @brief Obtém o identificador único (ID) do Usuario.
     * * @return O ID do Usuario.
     */
    long getId() const;

    /**
     * @brief Define o identificador único (ID) do Usuario.
     * * @param id O novo ID.
     */
    void setId(long id);

    /**
     * @brief Obtém o nome do Usuario.
     * * @return Uma referência constante para a string do nome.
     */
    const std::string& getNome() const;

    /**
     * @brief Define o nome do Usuario.
     * * @param nome O novo nome.
     */
    void setNome(const std::string& nome);

    /**
     * @brief Obtém o e-mail do Usuario.
     * * @return Uma referência constante para a string do e-mail.
     */
    const std::string& getEmail() const;

    /**
     * @brief Define o e-mail do Usuario.
     * * @param email O novo e-mail.
     */
    void setEmail(const std::string& email);

    /**
     * @brief Obtém a senha do Usuario.
     * * @return Uma referência constante para a string da senha.
     */
    const std::string& getSenha() const;

    /**
     * @brief Define a senha do Usuario.
     * * @param senha A nova senha.
     */
    void setSenha(const std::string& senha);
};

#endif