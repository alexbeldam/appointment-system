#include "model/aluno.hpp"

#include <algorithm>
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
    return agendamentos;
}

void Aluno::setAgendamentos(const vector<Agendamento>& agendamentos) {
    this->agendamentos = agendamentos;
}

void Aluno::addAgendamento(const Agendamento& agendamento) {
    this->agendamentos.push_back(agendamento);
}

void Aluno::updateAgendamento(const Agendamento& agendamento) {
    for (Agendamento& a : this->agendamentos) {
        if (a.getId() == agendamento.getId()) {
            a = agendamento;
            return;
        }
    }
}

void Aluno::removeAgendamento(long id) {
    this->agendamentos.erase(
        remove_if(this->agendamentos.begin(), this->agendamentos.end(),
                  [id](const Agendamento& a) { return a.getId() == id; }),
        this->agendamentos.end());
}