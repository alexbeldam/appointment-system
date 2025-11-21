#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include <vector>

#include "model/usuario.hpp"

class Horario;  // Declaração forward

/**
 * @brief Representa a entidade Professor, que é um tipo de Usuário.
 * * Um Professor leciona uma disciplina e possui uma lista de Horários de
 * disponibilidade para agendamento.
 * * A lista de horários é carregada sob demanda (Lazy Loading) usando
 * EntityList.
 */
class Professor : public Usuario {
   private:
    std::string
        disciplina; /**< A disciplina principal lecionada pelo professor. */

    /**
     * @brief Lista de horários de disponibilidade associados a este professor.
     * * Armazenada como EntityList para suportar carregamento preguiçoso.
     */
    EntityList<Horario> horarios;

   public:
    /**
     * @brief Alias para o tipo EntityList de Horario.
     */
    using HorarioList = EntityList<Horario>;

    /**
     * @brief Alias para o vetor de ponteiros inteligentes de Horario.
     */
    using HorarioVector = HorarioList::EntityVector;

    /**
     * @brief Alias para a função de carregamento que fornece a lista de
     * horários.
     */
    using HorariosLoader = ListLoaderFunction<Horario>;

    /**
     * @brief Construtor da classe Professor.
     * * Inicializa os atributos do Usuário e do Professor, e injeta a função de
     * carregamento dos horários.
     * @param id O identificador único do usuário.
     * @param nome O nome do professor.
     * @param email O email do professor.
     * @param senha A senha do professor.
     * @param disciplina A disciplina lecionada.
     * @param loader A função de carregamento para os horários do professor.
     */
    Professor(long id, const std::string& nome, const std::string& email,
              const std::string& senha, std::string disciplina,
              const HorariosLoader& loader);

    /**
     * @brief Retorna a disciplina lecionada pelo professor.
     * @return const std::string& A string contendo o nome da disciplina.
     */
    const std::string& getDisciplina() const;

    /**
     * @brief Define uma nova disciplina para o professor.
     * @param disciplina A nova disciplina.
     */
    void setDisciplina(const std::string& disciplina);

    /**
     * @brief Retorna a lista de Horários de disponibilidade do professor.
     * * O carregamento dos horários é disparado na primeira chamada a esta
     * função ou se o EntityList for acessado.
     * @return HorarioList& A referência para a lista de horários.
     */
    HorarioList& getHorarios();

    /**
     * @brief Retorna uma lista filtrada de Horários que estão disponíveis.
     * * Carrega a lista completa de horários e aplica a lógica de filtragem.
     * @return HorarioVector Um vetor contendo apenas os horários disponíveis.
     */
    HorarioVector getHorariosDisponiveis();

    /**
     * @brief Retorna uma lista filtrada de Horários que estão ocupados.
     * * Carrega a lista completa de horários e aplica a lógica de filtragem.
     * @return HorarioVector Um vetor contendo apenas os horários ocupados.
     */
    HorarioVector getHorariosOcupados();

    /**
     * @brief Operador de comparação para ordenação.
     * * Útil para ordenar professores com base no nome.
     * @param other O outro objeto Professor a ser comparado.
     * @return true Se este professor for considerado "menor" (deve vir antes)
     * que o outro.
     */
    bool operator<(const Professor& other) const;
};

#endif