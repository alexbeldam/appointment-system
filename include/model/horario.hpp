#ifndef HORARIO_HPP
#define HORARIO_HPP

#include <ctime>
#include <string>

#include "util/entityList.hpp"

/**
 * @brief Alias de tipo para representar um ponto no tempo.
 */
using Timestamp = std::time_t;

// Declarações forward para evitar inclusões circulares
class Professor;
class Agendamento;

/**
 * @brief Representa a entidade Horário de disponibilidade de um professor.
 * * Define um intervalo de tempo que pode ser reservado por alunos
 * (Agendamento).
 * * Utiliza a técnica de "Lazy Loading" para as entidades Professor e
 * Agendamento.
 */
class Horario {
   private:
    long id;          /**< O identificador único do horário. */
    long idProfessor; /**< O ID do professor proprietário deste horário. */
    Timestamp inicio; /**< O timestamp de início do horário. */
    Timestamp fim;    /**< O timestamp de fim do horário. */
    bool disponivel;  /**< Indica se o horário está atualmente disponível para
                         agendamento. */

    /**
     * @brief Função de callback para carregar o objeto Professor proprietário
     * (Lazy Loading).
     */
    const LoadFunction<Professor>& professorLoader;

    /**
     * @brief Lista de agendamentos associados a este horário.
     * * Armazenada como EntityList para suportar carregamento preguiçoso.
     */
    EntityList<Agendamento> agendamentos;

   public:
    /**
     * @brief Alias para a função de carregamento do Professor.
     */
    using ProfessorLoader = LoadFunction<Professor>;

    /**
     * @brief Alias para a função de carregamento da lista de Agendamentos.
     */
    using AgendamentosLoader = ListLoaderFunction<Agendamento>;

    /**
     * @brief Alias para o tipo EntityList de Agendamento.
     */
    using AgendamentoList = EntityList<Agendamento>;

    /**
     * @brief Alias para o vetor de ponteiros inteligentes de Agendamento.
     */
    using AgendamentoVector = AgendamentoList::EntityVector;

    /**
     * @brief Construtor da classe Horario.
     * * Inicializa os atributos e injeta as funções de carregamento para as
     * entidades associadas.
     * @param id O ID único do horário.
     * @param idProf O ID do professor.
     * @param inicio O timestamp de início.
     * @param fim O timestamp de fim.
     * @param disp O status inicial de disponibilidade.
     * @param profLoader Função de callback para carregar o objeto Professor.
     * @param agLoader Função de callback para carregar a lista de Agendamentos.
     */
    Horario(long id, long idProf, Timestamp inicio, Timestamp fim, bool disp,
            const ProfessorLoader& profLoader,
            const AgendamentosLoader& agLoader);

    /**
     * @brief Retorna o ID único do horário.
     * @return long O ID do horário.
     */
    long getId() const;

    /**
     * @brief Retorna o ID do professor proprietário.
     * @return long O ID do professor.
     */
    long getProfessorId() const;

    /**
     * @brief Retorna o timestamp de início do horário.
     * @return Timestamp O tempo de início.
     */
    Timestamp getInicio() const;

    /**
     * @brief Retorna o timestamp de fim do horário.
     * @return Timestamp O tempo de fim.
     */
    Timestamp getFim() const;

    /**
     * @brief Verifica se o horário está disponível.
     * @return bool True se estiver disponível, false caso contrário.
     */
    bool isDisponivel() const;

    /**
     * @brief Retorna o timestamp de início formatado como string.
     * @return std::string A string formatada do início.
     */
    std::string getInicioStr() const;

    /**
     * @brief Retorna o timestamp de fim formatado como string.
     * @return std::string A string formatada do fim.
     */
    std::string getFimStr() const;

    /**
     * @brief Carrega e retorna o objeto Professor proprietário (Lazy Load).
     * @return std::shared_ptr<Professor> O objeto Professor.
     */
    std::shared_ptr<Professor> getProfessor();

    /**
     * @brief Retorna uma lista filtrada de Agendamentos com status PENDENTE.
     * * Carrega a lista completa de agendamentos e aplica a filtragem.
     * @return AgendamentoVector Um vetor contendo agendamentos pendentes.
     */
    AgendamentoVector getAgendamentosPendentes();

    /**
     * @brief Retorna uma lista filtrada de Agendamentos com status CONFIRMADO.
     * * Carrega a lista completa de agendamentos e aplica a filtragem.
     * @return AgendamentoVector Um vetor contendo agendamentos confirmados.
     */
    AgendamentoVector getAgendamentosConfirmados();

    /**
     * @brief Define um novo ID para o horário.
     * @param id O novo ID.
     */
    void setId(long id);

    /**
     * @brief Define um novo ID de professor proprietário.
     * @param idProfessor O novo ID do professor.
     */
    void setProfessorId(long idProfessor);

    /**
     * @brief Define um novo timestamp de início.
     * @param inicio O novo tempo de início.
     */
    void setInicio(Timestamp inicio);

    /**
     * @brief Define um novo timestamp de fim.
     * @param fim O novo tempo de fim.
     */
    void setFim(Timestamp fim);

    /**
     * @brief Define o status de disponibilidade do horário.
     * @param disponivel O novo status (true/false).
     */
    void setDisponivel(bool disponivel);

    /**
     * @brief Operador de comparação para ordenação.
     * * Útil para ordenar horários com base no tempo de início.
     * @param other O outro objeto Horario a ser comparado.
     * @return true Se este horário começar antes do outro.
     */
    bool operator<(const Horario& other) const;
};

#endif