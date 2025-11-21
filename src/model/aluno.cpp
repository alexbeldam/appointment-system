#include "model/aluno.hpp"

#include <algorithm>

#include "model/agendamento.hpp"
using namespace std;

Aluno::Aluno(long id, const string& nome, const string& email,
             const string& senha, long matricula,
             const AgendamentosLoader& loader)
    : Usuario(id, nome, email, senha),
      matricula(matricula),
      agendamentos(loader, id) {}

long Aluno::getMatricula() const {
    return matricula;
}

void Aluno::setMatricula(long matricula) {
    this->matricula = matricula;
}

Aluno::AgendamentoList& Aluno::getAgendamentos() {
    return agendamentos;
}

Aluno::AgendamentoVector Aluno::getAgendamentosCancelaveis() {
    AgendamentoVector cancelaveis(agendamentos.begin(), agendamentos.end());

    cancelaveis.erase(remove_if(cancelaveis.begin(), cancelaveis.end(),
                                [](shared_ptr<Agendamento> agendamento) {
                                    auto stts = agendamento->getStatus();

                                    return stts != Status::PENDENTE &&
                                           stts != Status::CONFIRMADO;
                                }),
                      cancelaveis.end());

    return cancelaveis;
}