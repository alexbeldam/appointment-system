#include "dto/alunoDTO.hpp"
using namespace std;

long AlunoDTO::getId() const {
    return id;
}

void AlunoDTO::setId(long id) {
    this->id = id;
}

const string& AlunoDTO::getNome() const {
    return nome;
}

void AlunoDTO::setNome(const string& nome) {
    this->nome = nome;
}

const string& AlunoDTO::getEmail() const {
    return email;
}

void AlunoDTO::setEmail(const string& email) {
    this->email = email;
}

const string& AlunoDTO::getSenha() const {
    return senha;
}

void AlunoDTO::setSenha(const string& senha) {
    this->senha = senha;
}

long AlunoDTO::getMatricula() const {
    return matricula;
}

void AlunoDTO::setMatricula(long matricula) {
    this->matricula = matricula;
}