#ifndef AGENDAMENTO_HPP
#define AGENDAMENTO_HPP

#include <string>

/**
 * @brief Representa a entidade Agendamento.
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

    Agendamento(long alunoId, long horarioId, const std::string& status);

    /**
     * @brief Construtor para carregar um agendamento do DB (já com ID).
     * @param id ID do agendamento.
     * @param alunoId ID do aluno.
     * @param horarioId ID do horário.
     * @param status O status lido do CSV.
     */

    Agendamento(long id, long alunoId, long horarioId, const std::string& status);

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

    std::string getStatus() const;

    // --- NÃO APAGAR ---

    /**
     * @brief Construtor de cópia (copy constructor).
     */
    
    Agendamento(const Agendamento& other) = default;
};

#endif