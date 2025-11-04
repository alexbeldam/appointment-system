#include "model/horario.hpp"

#include <string>

#include "util/utils.hpp"
using namespace std;

Horario::Horario(long id, long idProfessor, time_t inicio, time_t fim,
                 bool disponivel)
    : id(id),
      idProfessor(idProfessor),
      inicio(inicio),
      fim(fim),
      disponivel(disponivel) {}

long Horario::getId() const {
    return id;
}
long Horario::getProfessorId() const {
    return idProfessor;
}
time_t Horario::getInicio() const {
    return inicio;
}
time_t Horario::getFim() const {
    return fim;
}
bool Horario::isDisponivel() const {
    return disponivel;
}

std::string Horario::getInicioStr() const {
    return time_to_string(inicio);
}

std::string Horario::getFimStr() const {
    return time_to_string(fim);
}

void Horario::setId(long id) {
    this->id = id;
}
void Horario::setProfessorId(long idProfessor) {
    this->idProfessor = idProfessor;
}
void Horario::setInicio(time_t inicio) {
    this->inicio = inicio;
}
void Horario::setFim(time_t fim) {
    this->fim = fim;
}
void Horario::setDisponivel(bool disponivel) {
    this->disponivel = disponivel;
}

bool Horario::operator<(const Horario& other) const {
    if (this->inicio != other.inicio)
        return this->inicio < other.inicio;
    return this->fim < other.fim;
}