#include "model/usuario.hpp"

using std::string;

Usuario::Usuario() {}

Usuario::Usuario(long id, const string& nome, const string& email,
                 const string& senha) {
    setId(id);
    setNome(nome);
    setEmail(email);
    setSenha(senha);
}

Usuario::~Usuario() {}

long Usuario::getId() const {
    return id;
}

void Usuario::setId(long id) {
    this->id = id;
}

const string& Usuario::getNome() const {
    return nome;
}

void Usuario::setNome(const string& nome) {
    this->nome = nome;
}

const string& Usuario::getEmail() const {
    return email;
}

void Usuario::setEmail(const string& email) {
    this->email = email;
}

const string& Usuario::getSenha() const {
    return senha;
}

void Usuario::setSenha(const string& senha) {
    this->senha = senha;
}