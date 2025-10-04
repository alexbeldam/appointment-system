#ifndef ALUNO_DTO_HPP
#define ALUNO_DTO_HPP

#include <string>
#include <vector>

class AlunoDTO {
   private:
    long id;
    std::string nome;
    std::string email;
    std::string senha;
    long matricula;

   public:
    long getId() const;

    void setId(long id);

    const std::string& getNome() const;

    void setNome(const std::string& nome);

    const std::string& getEmail() const;

    void setEmail(const std::string& email);

    const std::string& getSenha() const;

    void setSenha(const std::string& senha);

    long getMatricula() const;

    void setMatricula(long matricula);
};

#endif