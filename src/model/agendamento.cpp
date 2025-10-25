#include "model/agendamento.hpp"

/**
 * @brief Construtor para criar um novo agendamento (antes de salvar no DB).
 */

Agendamento::Agendamento(long alunoId, long horarioId, const std::string& status)
    : id(0),
      alunoId(alunoId),
      horarioId(horarioId),
      status(status) {}

/**
 * @brief Construtor para carregar um agendamento do DB (já com ID).
 */

Agendamento::Agendamento(long id, long alunoId, long horarioId, const std::string& status)
    : id(id),
      alunoId(alunoId),
      horarioId(horarioId),
      status(status) {}

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

/**
 * @brief Obtém o ID do Horário associado a este agendamento.
 */

long Agendamento::getHorarioId() const {
    return this->horarioId;
}

/**
 * @brief Obtém o status deste agendamento.
 */

std::string Agendamento::getStatus() const {
    return this->status;
}