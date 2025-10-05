#include "dto/usuarioDTO.hpp"
using namespace std;

UsuarioDTO::~UsuarioDTO() {}

long UsuarioDTO::getId() const {
    return id;
}

void UsuarioDTO::setId(long id) {
    this->id = id;
}

const string& UsuarioDTO::getNome() const {
    return nome;
}

void UsuarioDTO::setNome(const string& nome) {
    this->nome = nome;
}

const string& UsuarioDTO::getEmail() const {
    return email;
}

void UsuarioDTO::setEmail(const string& email) {
    this->email = email;
}

const string& UsuarioDTO::getSenha() const {
    return senha;
}

void UsuarioDTO::setSenha(const string& senha) {
    this->senha = senha;
}