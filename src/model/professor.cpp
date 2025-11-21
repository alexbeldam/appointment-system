#include "model/professor.hpp"

#include <algorithm>

#include "model/horario.hpp"

using std::remove_if;
using std::shared_ptr;
using std::string;

Professor::Professor(long id, const string& nome, const string& email,
                     const string& senha, string disciplina,
                     const HorariosLoader& loader)
    : Usuario(id, nome, email, senha),
      disciplina(disciplina),
      horarios(loader, id) {}

const string& Professor::getDisciplina() const {
    return disciplina;
}

void Professor::setDisciplina(const string& disciplina) {
    this->disciplina = disciplina;
}

Professor::HorarioList& Professor::getHorarios() {
    return horarios;
}

Professor::HorarioVector Professor::getHorariosDisponiveis() {
    HorarioVector disponiveis(horarios.begin(), horarios.end());

    disponiveis.erase(remove_if(disponiveis.begin(), disponiveis.end(),
                                [](shared_ptr<Horario> horario) {
                                    return !horario->isDisponivel();
                                }),
                      disponiveis.end());

    return disponiveis;
}

Professor::HorarioVector Professor::getHorariosOcupados() {
    HorarioVector ocupados(horarios.begin(), horarios.end());

    ocupados.erase(remove_if(ocupados.begin(), ocupados.end(),
                             [](shared_ptr<Horario> horario) {
                                 return horario->isDisponivel();
                             }),
                   ocupados.end());

    return ocupados;
}

bool Professor::operator<(const Professor& other) const {
    return this->getNome() < other.getNome();
}