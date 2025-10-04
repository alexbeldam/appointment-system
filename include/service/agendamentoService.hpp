#ifndef AGENDAMENTO_SERVICE_HPP
#define AGENDAMENTO_SERVICE_HPP

#include <vector>

#include "model/agendamento.hpp"

class AgendamentoService {
   public:
    std::vector<Agendamento> listByIdAluno(long id) const;
};

#endif