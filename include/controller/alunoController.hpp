#ifndef ALUNO_CONTROLLER_HPP
#define ALUNO_CONTROLLER_HPP

#include "service/alunoService.hpp"

class AlunoController {
   private:
    AlunoService service;

   public:
    Aluno create(const std::string& nome, const std::string& email,
                 const std::string& senha, long matricula);
    Aluno read(long id);
    std::vector<Aluno> list();
    Aluno update(long id, const std::string& nome, const std::string& email,
                 const std::string& senha, long matricula);
    bool destroy(long id);
};

#endif