#include "model/horario.hpp"

#include <string>

#include "model/agendamento.hpp"
#include "util/utils.hpp"
using namespace std;

Horario::Horario(long id, long idProfessor, Timestamp inicio, Timestamp fim,
                 bool disponivel, const ProfessorLoader& profLoader,
                 const AgendamentosLoader& agLoader)
    : id(id),
      idProfessor(idProfessor),
      inicio(inicio),
      fim(fim),
      disponivel(disponivel),
      professorLoader(profLoader),
      agendamentos(agLoader, id) {}

long Horario::getId() const {
    return id;
}
long Horario::getProfessorId() const {
    return idProfessor;
}
Timestamp Horario::getInicio() const {
    return inicio;
}
Timestamp Horario::getFim() const {
    return fim;
}
bool Horario::isDisponivel() const {
    return disponivel;
}

string Horario::getInicioStr() const {
    return timestamp_to_string(inicio);
}

string Horario::getFimStr() const {
    return timestamp_to_string(fim);
}

shared_ptr<Professor> Horario::getProfessor() {
    if (professorLoader)
        return professorLoader(idProfessor);
    return nullptr;
}

Horario::AgendamentoVector Horario::getAgendamentosPendentes() {
    AgendamentoVector pendentes(agendamentos.begin(), agendamentos.end());

    pendentes.erase(remove_if(pendentes.begin(), pendentes.end(),
                              [](shared_ptr<Agendamento> ag) {
                                  return ag->getStatus() != Status::PENDENTE;
                              }),
                    pendentes.end());

    return pendentes;
}

Horario::AgendamentoVector Horario::getAgendamentosConfirmados() {
    AgendamentoVector confirmados(agendamentos.begin(), agendamentos.end());

    confirmados.erase(remove_if(confirmados.begin(), confirmados.end(),
                                [](shared_ptr<Agendamento> ag) {
                                    return ag->getStatus() !=
                                           Status::CONFIRMADO;
                                }),
                      confirmados.end());

    return confirmados;
}

void Horario::setId(long id) {
    this->id = id;
}
void Horario::setProfessorId(long idProfessor) {
    this->idProfessor = idProfessor;
}
void Horario::setInicio(Timestamp inicio) {
    this->inicio = inicio;
}
void Horario::setFim(Timestamp fim) {
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