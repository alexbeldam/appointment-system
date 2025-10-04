#ifndef ALUNO_MAPPER_HPP
#define ALUNO_MAPPER_HPP

#include "dto/alunoDTO.hpp"
#include "model/aluno.hpp"

class AlunoMapper {
   public:
    Aluno map(const AlunoDTO& dto) const;

    AlunoDTO map(const Aluno& aluno) const;

    std::string toCsvData(const AlunoDTO& aluno) const;

    Aluno fromCsvLine(const std::string& line) const;
};

#endif