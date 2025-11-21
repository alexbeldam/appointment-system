#ifndef HORARIO_HPP
#define HORARIO_HPP

#include <ctime>
#include <string>

#include "util/entityList.hpp"

using Timestamp = std::time_t;

class Professor;
class Agendamento;

/**
 * @brief Representa a entidade Horário de disponibilidade de um professor.
 */
class Horario {
   private:
    long id;
    long idProfessor;
    Timestamp inicio;
    Timestamp fim;
    bool disponivel;
    const LoadFunction<Professor>& professorLoader;
    EntityList<Agendamento> agendamentos;

   public:
    using ProfessorLoader = LoadFunction<Professor>;
    using AgendamentosLoader = ListLoaderFunction<Agendamento>;
    using AgendamentoList = EntityList<Agendamento>;
    using AgendamentoVector = AgendamentoList::EntityVector;

    Horario(long id, long idProf, Timestamp inicio, Timestamp fim, bool disp,
            const ProfessorLoader& profLoader,
            const AgendamentosLoader& agLoader);

    long getId() const;
    long getProfessorId() const;
    Timestamp getInicio() const;
    Timestamp getFim() const;
    bool isDisponivel() const;

    std::string getInicioStr() const;
    std::string getFimStr() const;

    std::shared_ptr<Professor> getProfessor();
    AgendamentoVector getAgendamentosPendentes();
    AgendamentoVector getAgendamentosConfirmados();

    void setId(long id);
    void setProfessorId(long idProfessor);
    void setInicio(Timestamp inicio);
    void setFim(Timestamp fim);
    void setDisponivel(bool disponivel);

    bool operator<(const Horario& other) const;
};

#endif
