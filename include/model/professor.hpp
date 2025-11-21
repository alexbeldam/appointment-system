#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include <vector>

#include "model/usuario.hpp"

class Horario;

/**
 * @brief Representa a entidade Professor, que herda as propriedades básicas de
 * Usuario.
 * * Adiciona atributos específicos de domínio como o número de matrícula e uma
 * lista de horarios associados.
 */
class Professor : public Usuario {
   private:
    std::string disciplina;  ///< A dsiciplina do Professor.
    EntityList<Horario>
        horarios;  ///< Lista de Horarios associados a este Professor.

   public:
    using HorarioList = EntityList<Horario>;
    using HorarioVector = HorarioList::EntityVector;
    using HorariosLoader = ListLoaderFunction<Horario>;

    /**
     * @brief Construtor completo do Professor.
     * * Inicializa os campos herdados (ID, nome, email, disciplina) e os campos
     * específicos (matrícula, horarios).
     * @param id O identificador único.
     * @param nome O nome do Professor.
     * @param email O e-mail do Professor.
     * @param disciplina A disciplina do Professor.
     * @param disciplina A disciplina do Professor.
     * @param horarios Uma lista de Horarios associados.
     */
    Professor(long id, const std::string& nome, const std::string& email,
              const std::string& senha, std::string disciplina,
              const HorariosLoader& loader);

    /**
     * @brief Obtém a disciplina do Usuário.
     * * @return Uma referência constante para a string da disciplina.
     */
    const std::string& getDisciplina() const;

    /**
     * @brief Define a disciplina do Usuário.
     * * @param disciplina A nova disciplina.
     */
    void setDisciplina(const std::string& disciplina);

    HorarioList& getHorarios();

    /**
     * @brief Obtém a lista de Horarios disponíveis associados a este Professor.
     * * @return Uma referência constante para o vetor de Horarios disponíveis.
     */
    HorarioVector getHorariosDisponiveis();

    HorarioVector getHorariosOcupados();

    bool operator<(const Professor& other) const;
};

#endif