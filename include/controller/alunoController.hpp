#ifndef ALUNO_CONTROLLER_HPP
#define ALUNO_CONTROLLER_HPP

#include "service/alunoService.hpp"

class AlunoController {
   private:
    const AlunoService service;

   public:
    AlunoController(const AlunoService& service);

    Aluno create(const std::string& nome, const std::string& email,
                 const std::string& senha, long matricula) const;

    Aluno read(long id) const;

    std::vector<Aluno> list() const;

    Aluno update(long id, const std::string& nome, const std::string& email,
                 const std::string& senha, long matricula) const;

    bool destroy(long id) const;
};

#endif