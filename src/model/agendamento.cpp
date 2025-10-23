#include "model/agendamento.hpp"

/**
 * @brief Obtém o identificador único (ID) do Agendamento.
 * @return O ID do Agendamento.
 */
long Agendamento::getId() const {
    return this->id;
}

/**
 * @brief Obtém o ID do Aluno associado a este agendamento.
 * @return O ID único do Aluno.
 */
long Agendamento::getAlunoId() const {
    return this->alunoId;
}

/**
 * @brief Obtém o ID do Horário associado a este agendamento.
 * @return O ID único do Horário.
 */
long Agendamento::getHorarioId() const {
    return this->horarioId;
}

/**
 * @brief Obtém o status deste agendamento.
 * @return O status.
 */
std::string Agendamento::getStatus() const {
    return this->status;
}