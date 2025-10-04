#ifndef ALUNO_DTO_HPP
#define ALUNO_DTO_HPP

#include <string>
#include <vector>

/**
 * @brief Data Transfer Object (DTO) para a entidade Aluno.
 * * Esta classe é utilizada para transferir dados de e para a camada de
 * serviço, contendo apenas os atributos básicos da entidade Aluno.
 */
class AlunoDTO {
   private:
    long id;            ///< Identificador único do Aluno.
    std::string nome;   ///< Nome completo do Aluno.
    std::string email;  ///< Endereço de e-mail único do Aluno.
    std::string senha;  ///< Senha do Aluno.
    long matricula;     ///< Número de matrícula único do Aluno.

   public:
    /**
     * @brief Obtém o identificador único (ID) do Aluno.
     * * @return O ID do Aluno.
     */
    long getId() const;

    /**
     * @brief Define o identificador único (ID) do Aluno.
     * * @param id O novo ID.
     */
    void setId(long id);

    /**
     * @brief Obtém o nome do Aluno.
     * * @return Uma referência constante para a string do nome.
     */
    const std::string& getNome() const;

    /**
     * @brief Define o nome do Aluno.
     * * @param nome O novo nome.
     */
    void setNome(const std::string& nome);

    /**
     * @brief Obtém o e-mail do Aluno.
     * * @return Uma referência constante para a string do e-mail.
     */
    const std::string& getEmail() const;

    /**
     * @brief Define o e-mail do Aluno.
     * * @param email O novo e-mail.
     */
    void setEmail(const std::string& email);

    /**
     * @brief Obtém a senha do Aluno.
     * * @return Uma referência constante para a string da senha.
     */
    const std::string& getSenha() const;

    /**
     * @brief Define a senha do Aluno.
     * * @param senha A nova senha.
     */
    void setSenha(const std::string& senha);

    /**
     * @brief Obtém o número de matrícula do Aluno.
     * * @return O número de matrícula.
     */
    long getMatricula() const;

    /**
     * @brief Define o número de matrícula do Aluno.
     * * @param matricula O novo número de matrícula.
     */
    void setMatricula(long matricula);
};

#endif