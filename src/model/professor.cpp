#include "model/professor.hpp"

#include <algorithm>
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

const vector<Horario>& Professor::getHorariosDisponiveis() const {
    static vector<Horario> disponiveis;
    disponiveis.clear();
    for (const Horario& h : this->horarios) {
        if (h.isDisponivel()) {
            disponiveis.push_back(h);
        }
    }
    return disponiveis;
}

void Professor::setHorarios(const vector<Horario>& horarios) {
    this->horarios = horarios;
}

void Professor::addHorario(const Horario& horario) {
    this->horarios.push_back(horario);
}

void Professor::updateHorario(const Horario& horario) {
    for (Horario& h : this->horarios) {
        if (h.getId() == horario.getId()) {
            h = horario;
            return;
        }
    }
}

void Professor::removeHorario(long id) {
    this->horarios.erase(
        remove_if(this->horarios.begin(), this->horarios.end(),
                  [id](const Horario& h) { return h.getId() == id; }),
        this->horarios.end());
}