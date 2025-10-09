#ifndef AGENDAMENTO_HPP
#define AGENDAMENTO_HPP

/**
 * @brief CLASSE PLACEHOLDER. Representa a entidade Agendamento.
 *
 * Esta definição é incompleta e serve unicamente para resolver dependências de
 * compilação em outras classes (ex: Aluno). Atributos e métodos de domínio
 * devem ser adicionados posteriormente.
 */
class Agendamento {
   public:
    /**
     * @brief Obtém o identificador único (ID) do Agendamento.
     * * @return O ID do Agendamento.
     */
    long getId() const;

    /**
     * @brief Obtém o ID do Aluno associado a este agendamento.
     * @return O ID único do Aluno.
     */
    long getAlunoId() const;

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