#include "model/aluno.hpp"
using namespace std;

Aluno::Aluno() {}

Aluno::Aluno(long id, const string& nome, const string& email,
             const string& senha, long matricula,
             const vector<Agendamento>& agendamentos)
    : Usuario(id, nome, email, senha) {
    setMatricula(matricula);
    setAgendamentos(agendamentos);
}

long Aluno::getMatricula() const {
    return matricula;
}

void Aluno::setMatricula(long matricula) {
    this->matricula = matricula;
}

const vector<Agendamento>& Aluno::getAgendamentos() const {
    return agendametos;
}

void Aluno::setAgendamentos(const vector<Agendamento>& agendamentos) {
    this->agendametos = agendamentos;
}