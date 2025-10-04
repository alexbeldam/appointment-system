#ifndef ALUNO_SERVICE_HPP
#define ALUNO_SERVICE_HPP

#include "map/alunoMapper.hpp"

class AlunoService {
   private:
    AlunoMapper mapper;

   public:
    Aluno save(const AlunoDTO& aluno);

    Aluno getById(long id);

    std::vector<Aluno> listAll();

    Aluno updateById(long id, const AlunoDTO& aluno);

    bool deleteById(long id);
};

#endif