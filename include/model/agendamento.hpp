#ifndef AGENDAMENTO_HPP
#define AGENDAMENTO_HPP

#include <ctime>
#include <string>

#include "util/entityList.hpp"

using Timestamp = std::time_t;

enum class Status { PENDENTE, CANCELADO, RECUSADO, CONFIRMADO };

Status parseStatus(const std::string& str);

constexpr const std::string_view stringify(const Status& stts) {
    switch (stts) {
        case Status::PENDENTE:
            return "PENDENTE";
        case Status::CANCELADO:
            return "CANCELADO";
        case Status::RECUSADO:
            return "RECUSADO";
        case Status::CONFIRMADO:
            return "CONFIRMADO";
        default:
            return "";
    }
}

constexpr int getStatusPriority(const Status& stts) {
    switch (stts) {
        case Status::CONFIRMADO:
            return 0;
        case Status::PENDENTE:
            return 1;
        case Status::RECUSADO:
            return 2;
        case Status::CANCELADO:
            return 3;
        default:
            return 4;
    }
}

class Aluno;
class Horario;

/**
 * @brief Representa a entidade Agendamento.
 */

class Agendamento {
   private:
    long id;
    long alunoId;
    long horarioId;
    Status status;
    const LoadFunction<Aluno>& alunoLoader;
    const LoadFunction<Horario>& horarioLoader;

    Timestamp horarioInicio;
    Timestamp horarioFim;

   public:
    /**
     * @brief Construtor para carregar um agendamento do DB (já com ID).
     * @param id ID do agendamento.
     * @param alunoId ID do aluno.
     * @param horarioId ID do horário.
     * @param status O status lido do CSV.
     */

    Agendamento(long id, long alunoId, long horarioId, const Status& status,
                const LoadFunction<Aluno>& alunoLoader,
                const LoadFunction<Horario>& horarioLoader,
                Timestamp horarioInicio, Timestamp horarioFim);

    /**
     * @brief Obtém o identificador único (ID) do Agendamento.
     * @return O ID do Agendamento.
     */
    long getId() const;

    /**
     * @brief Obtém o ID do Aluno associado a este agendamento.
     * @return O ID único do Aluno.
     */

    long getAlunoId() const;

    /**
     * @brief Obtém o ID do Horário associado a este agendamento.
     * @return O ID único do Horário.
     */

    long getHorarioId() const;

    /**
     * @brief Obtém o status deste agendamento (ex: "CONFIRMADO").
     * @return O status.
     */

    const Status& getStatus() const;

    const std::string_view getStatusStr() const;

    std::shared_ptr<Aluno> getAluno();

    std::shared_ptr<Horario> getHorario();

    bool operator<(const Agendamento& other) const;
};

#endif