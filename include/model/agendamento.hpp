#ifndef AGENDAMENTO_HPP
#define AGENDAMENTO_HPP

#include <ctime>
#include <string>

#include "util/entityList.hpp"

/**
 * @brief Alias de tipo para representar um ponto no tempo..
 */
using Timestamp = std::time_t;

/**
 * @brief Enumeração que define os possíveis status de um Agendamento.
 */
enum class Status {
    PENDENTE,  /**< O agendamento foi solicitado e aguarda aprovação. */
    CANCELADO, /**< O agendamento foi cancelado pelo aluno ou professor. */
    RECUSADO,  /**< O agendamento foi recusado pelo professor. */
    CONFIRMADO /**< O agendamento foi confirmado pelo professor. */
};

/**
 * @brief Converte uma string para o valor correspondente da enumeração Status.
 * * Não lança exceção, mas pode retornar um status padrão se a string for
 * inválida.
 * @param str A string de status (ex: "CONFIRMADO").
 * @return Status O valor da enumeração Status.
 */
Status parseStatus(const std::string& str);

/**
 * @brief Converte um valor da enumeração Status para sua representação em
 * string.
 * * @param stts O status a ser convertido.
 * @return std::string_view A representação em string do status.
 */
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

/**
 * @brief Atribui uma prioridade numérica aos status para fins de ordenação.
 * * Status de maior prioridade (ex: CONFIRMADO) terão menor valor inteiro.
 * @param stts O status.
 * @return int O valor de prioridade (0 = mais importante).
 */
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

// Declarações forward para evitar inclusões circulares
class Aluno;
class Horario;

/**
 * @brief Representa a entidade Agendamento.
 * * Esta entidade modela uma reserva de um Horário por um Aluno.
 * * Utiliza a técnica de "Lazy Loading" para as entidades Aluno e Horario.
 */
class Agendamento {
   private:
    long id;        /**< O identificador único do agendamento. */
    long alunoId;   /**< O ID do aluno que fez o agendamento. */
    long horarioId; /**< O ID do horário reservado. */
    Status status;  /**< O status atual do agendamento (PENDENTE, CONFIRMADO,
                       etc.). */

    /**
     * @brief Função para carregar o objeto Aluno sob demanda (Lazy Loading).
     */
    const LoadFunction<Aluno>& alunoLoader;

    /**
     * @brief Função para carregar o objeto Horario sob demanda (Lazy Loading).
     */
    const LoadFunction<Horario>& horarioLoader;

    Timestamp horarioInicio; /**< O timestamp de início do horário agendado. */
    Timestamp horarioFim;    /**< O timestamp de fim do horário agendado. */

   public:
    /**
     * @brief Construtor da classe Agendamento.
     * * Recebe funções de carregamento para as entidades associadas (Aluno e
     * Horario).
     * @param id O ID único do agendamento.
     * @param alunoId O ID do aluno.
     * @param horarioId O ID do horário.
     * @param status O status inicial.
     * @param alunoLoader Função de callback para carregar o objeto Aluno.
     * @param horarioLoader Função de callback para carregar o objeto Horario.
     * @param horarioInicio O timestamp de início do horário.
     * @param horarioFim O timestamp de fim do horário.
     */
    Agendamento(long id, long alunoId, long horarioId, const Status& status,
                const LoadFunction<Aluno>& alunoLoader,
                const LoadFunction<Horario>& horarioLoader,
                Timestamp horarioInicio, Timestamp horarioFim);

    /**
     * @brief Retorna o ID único do agendamento.
     * @return long O ID do agendamento.
     */
    long getId() const;

    /**
     * @brief Retorna o ID do aluno associado.
     * @return long O ID do aluno.
     */
    long getAlunoId() const;

    /**
     * @brief Retorna o ID do horário associado.
     * @return long O ID do horário.
     */
    long getHorarioId() const;

    /**
     * @brief Retorna o status do agendamento.
     * @return const Status& O status atual.
     */
    const Status& getStatus() const;

    /**
     * @brief Retorna o status do agendamento como string.
     * @return const std::string_view A representação em string do status.
     */
    const std::string_view getStatusStr() const;

    /**
     * @brief Carrega e retorna o objeto Aluno associado (Lazy Load).
     * @return std::shared_ptr<Aluno> O objeto Aluno.
     */
    std::shared_ptr<Aluno> getAluno();

    /**
     * @brief Carrega e retorna o objeto Horario associado (Lazy Load).
     * @return std::shared_ptr<Horario> O objeto Horario.
     */
    std::shared_ptr<Horario> getHorario();

    /**
     * @brief Operador de comparação para ordenação.
     * * Útil para ordenar agendamentos com base em critérios como prioridade de
     * status e horário.
     * @param other O outro objeto Agendamento a ser comparado.
     * @return true Se este agendamento for considerado "menor" (deve vir antes)
     * que o outro.
     */
    bool operator<(const Agendamento& other) const;
};

#endif