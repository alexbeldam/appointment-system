#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

class Usuario {
   private:
    long id;
    std::string nome;
    std::string email;
    std::string senha;

   public:
    Usuario();
    Usuario(long id, const std::string& nome, const std::string& email,
            const std::string& senha);

    long getId() const;

    void setId(long id);

    const std::string& getNome() const;

    void setNome(const std::string& nome);

    const std::string& getEmail() const;

    void setEmail(const std::string& email);

    const std::string& getSenha() const;

    void setSenha(const std::string& senha);
};

#endif