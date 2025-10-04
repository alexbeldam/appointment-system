#include "map/alunoMapper.hpp"

Aluno AlunoMapper::map(const AlunoDTO& dto) {
    Aluno aluno;

    aluno.setId(dto.getId());
    aluno.setEmail(dto.getEmail());
    aluno.setNome(dto.getNome());
    aluno.setSenha(dto.getSenha());
    aluno.setMatricula(dto.getMatricula());
    aluno.setAgendamentos(service.listByIdAluno(dto.getId()));

    return aluno;
}

AlunoDTO AlunoMapper::map(const Aluno& aluno) {
    AlunoDTO dto;

    dto.setId(aluno.getId());
    dto.setEmail(aluno.getEmail());
    dto.setNome(aluno.getNome());
    dto.setSenha(aluno.getSenha());
    dto.setMatricula(aluno.getMatricula());

    return dto;
}