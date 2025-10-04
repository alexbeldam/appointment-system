#ifndef ALUNO_SERVICE_HPP
#define ALUNO_SERVICE_HPP

#include <optional>

#include "map/alunoMapper.hpp"
#include "service/agendamentoService.hpp"

class AlunoService {
   private:
    const AlunoMapper mapper;
    const AgendamentoService service;

    Aluno mapAndInjectAgendamentos(const std::string& csv_line) const;

    std::vector<Aluno> getByEmail(const std::string& email) const;

    std::vector<Aluno> getByMatricula(long matricula) const;

    bool existsByEmail(std::string email) const;

    bool existsByEmailAndIdNot(std::string email, long id) const;

    bool existsByMatricula(long matricula) const;

    bool existsByMatriculaAndIdNot(long matricula, long id) const;

   public:
    AlunoService(const AlunoMapper& mapper, const AgendamentoService& service);

    Aluno save(const AlunoDTO& aluno) const;

    std::optional<Aluno> getById(long id) const;

    std::vector<Aluno> listAll() const;

    std::optional<Aluno> updateById(long id, const AlunoDTO& aluno) const;

    bool deleteById(long id) const;
};

#endif