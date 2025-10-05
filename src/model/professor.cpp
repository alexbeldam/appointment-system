#include "model/professor.hpp"
using namespace std;

Professor::Professor() {}

Professor::Professor(long id, const string& nome, const string& email,
                     const string& senha, string disciplina,
                     const vector<Horario>& horarios)
    : Usuario(id, nome, email, senha) {
    setDisciplina(disciplina);
    setHorarios(horarios);
}

const string& Professor::getDisciplina() const {
    return disciplina;
}

void Professor::setDisciplina(const string& disciplina) {
    this->disciplina = disciplina;
}

const vector<Horario>& Professor::getHorarios() const {
    return horarios;
}

void Professor::setHorarios(const vector<Horario>& horarios) {
    this->horarios = horarios;
}