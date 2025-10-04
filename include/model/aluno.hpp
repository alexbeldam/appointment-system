#ifndef ALUNO_HPP
#define ALUNO_HPP

#include <vector>

#include "model/agendamento.hpp"
#include "model/usuario.hpp"

class Aluno : public Usuario {
   private:
    long matricula;
    std::vector<Agendamento> agendametos;

   public:
    Aluno();
    Aluno(long id, const std::string& nome, const std::string& email,
          const std::string& senha, long matricula,
          const std::vector<Agendamento>& agendamentos);

    long getMatricula() const;

    void setMatricula(long matricula);

    const std::vector<Agendamento>& getAgendamentos() const;

    void setAgendamentos(const std::vector<Agendamento>& agendamentos);
};

#endif