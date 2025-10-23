#ifndef AGENDAMENTO_HPP
#define AGENDAMENTO_HPP

#include <string>

/**
 * @brief CLASSE PLACEHOLDER. Representa a entidade Agendamento.
 *
 * Esta definição é incompleta e serve unicamente para resolver dependências de
 * compilação em outras classes (ex: Aluno). Atributos e métodos de domínio
 * devem ser adicionados posteriormente.
 */
class Agendamento {
   private:
    long id;
    long alunoId;
    long horarioId;
    std::string status;

   public:
    /**
     * @brief Construtor para criar um novo agendamento (antes de salvar no DB).
     * @param alunoId ID do aluno.
     * @param horarioId ID do horário.
     * @param status O status inicial (ex: "CONFIRMADO").
     */
    Agendamento(long alunoId, long horarioId, const std::string& status)
        : id(0),
          alunoId(alunoId),
          horarioId(horarioId),
          status(status) {}

    /**
     * @brief Construtor para carregar um agendamento do DB (já com ID).
     * @param id ID do agendamento.
     * @param alunoId ID do aluno.
     * @param horarioId ID do horário.
     * @param status O status lido do CSV.
     */
    Agendamento(long id, long alunoId, long horarioId, const std::string& status)
        : id(id),
          alunoId(alunoId),
          horarioId(horarioId),
          status(status) {}

    /**
     * @brief Obtém o identificador único (ID) do Agendamento.
     * @return O ID do Agendamento.
     */
    long getId() const { return this->id; }

    /**
     * @brief Obtém o ID do Aluno associado a este agendamento.
     * @return O ID único do Aluno.
     */
    long getAlunoId() const { return this->alunoId; }

    /**
     * @brief Obtém o ID do Horário associado a este agendamento.
     * @return O ID único do Horário.
     */
    long getHorarioId() const { return this->horarioId; }

    /**
     * @brief Obtém o status deste agendamento (ex: "CONFIRMADO").
     * @return O status.
     */
    std::string getStatus() const { return this->status; }

    // --- NÃO APAGAR ---

    /**
     * @brief Construtor de cópia (copy constructor).
     *
     * Habilitado explicitamente para permitir que objetos Agendamento sejam
     * copiados por valor (necessário para std::vector e eventos).
     */
    Agendamento(const Agendamento& other) = default;
};

#endif