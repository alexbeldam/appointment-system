#ifndef ALUNO_MAPPER_HPP
#define ALUNO_MAPPER_HPP

#include "dto/alunoDTO.hpp"
#include "model/aluno.hpp"
#include "service/agendamentoService.hpp"

class AlunoMapper {
   private:
    AgendamentoService service;

   public:
    Aluno map(const AlunoDTO& dto);

    AlunoDTO map(const Aluno& aluno);
};

#endif