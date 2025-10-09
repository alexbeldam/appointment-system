#ifndef HORARIO_HPP
#define HORARIO_HPP

/**
 * @brief CLASSE PLACEHOLDER. Representa a entidade Horário.
 *
 * Esta definição é incompleta e serve unicamente para resolver dependências de
 * compilação em outras classes (ex: Professor). Atributos e métodos de domínio
 * devem ser adicionados posteriormente.
 */
class Horario {
   public:
    /**
     * @brief Obtém o identificador único (ID) do Horário.
     * * @return O ID do Horário.
     */
    long getId() const;

    /**
     * @brief Obtém o ID do Professor associado a este horário.
     * @return O ID único do Professor.
     */
    long getProfessorId() const;

    // --- NÃO APAGAR ---

    /**
     * @brief Construtor de cópia (copy constructor).
     *
     * Habilitado explicitamente para permitir que objetos Horario sejam
     * copiados por valor (necessário para std::vector e eventos).
     */
    Horario(const Horario& other) = default;
};

#endif