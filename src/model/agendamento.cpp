#include "model/agendamento.hpp"

using std::shared_ptr;
using std::string;
using std::string_view;

Status parseStatus(const string& str) {
    if (str == "CANCELADO")
        return Status::CANCELADO;
    if (str == "RECUSADO")
        return Status::RECUSADO;
    if (str == "CONFIRMADO")
        return Status::CONFIRMADO;

    return Status::PENDENTE;
}

/**
 * @brief Construtor para carregar um agendamento do DB (já com ID).
 */

Agendamento::Agendamento(long id, long alunoId, long horarioId,
                         const Status& status,
                         const LoadFunction<Aluno>& alunoLoader,
                         const LoadFunction<Horario>& horarioLoader,
                         Timestamp horarioInicio, Timestamp horarioFim)
    : id(id),
      alunoId(alunoId),
      horarioId(horarioId),
      status(status),
      alunoLoader(alunoLoader),
      horarioLoader(horarioLoader),
      horarioInicio(horarioInicio),
      horarioFim(horarioFim) {}

/**
 * @brief Obtém o identificador único (ID) do Agendamento.
 */

long Agendamento::getId() const {
    return this->id;
}

/**
 * @brief Obtém o ID do Aluno associado a este agendamento.
 */

long Agendamento::getAlunoId() const {
    return this->alunoId;
}

shared_ptr<Aluno> Agendamento::getAluno() {
    if (alunoLoader)
        return alunoLoader(alunoId);
    return nullptr;
}

shared_ptr<Horario> Agendamento::getHorario() {
    if (horarioLoader)
        return horarioLoader(horarioId);
    return nullptr;
}

/**
 * @brief Obtém o ID do Horário associado a este agendamento.
 */

long Agendamento::getHorarioId() const {
    return this->horarioId;
}

/**
 * @brief Obtém o status deste agendamento.
 */

const Status& Agendamento::getStatus() const {
    return this->status;
}

const string_view Agendamento::getStatusStr() const {
    return stringify(status);
}

bool Agendamento::operator<(const Agendamento& other) const {
    int priorityA = getStatusPriority(this->status);
    int priorityB = getStatusPriority(other.status);

    if (priorityA != priorityB) {
        return priorityA < priorityB;
    }

    if (this->horarioInicio != other.horarioInicio) {
        return this->horarioInicio > other.horarioInicio;
    }

    if (this->horarioFim != other.horarioFim) {
        return this->horarioFim > other.horarioFim;
    }

    return this->id > other.id;
}