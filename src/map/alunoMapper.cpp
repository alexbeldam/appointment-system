#include "map/alunoMapper.hpp"

#include <sstream>
using namespace std;

#define MIN_FIELDS 5

Aluno AlunoMapper::map(const AlunoDTO& dto) const {
    Aluno aluno;

    aluno.setId(dto.getId());
    aluno.setEmail(dto.getEmail());
    aluno.setNome(dto.getNome());
    aluno.setSenha(dto.getSenha());
    aluno.setMatricula(dto.getMatricula());

    return aluno;
}

AlunoDTO AlunoMapper::map(const Aluno& aluno) const {
    AlunoDTO dto;

    dto.setId(aluno.getId());
    dto.setEmail(aluno.getEmail());
    dto.setNome(aluno.getNome());
    dto.setSenha(aluno.getSenha());
    dto.setMatricula(aluno.getMatricula());

    return dto;
}

string AlunoMapper::toCsvData(const AlunoDTO& dto) const {
    stringstream ss;
    ss << dto.getNome() << "," << dto.getEmail() << "," << dto.getSenha() << ","
       << dto.getMatricula();
    return ss.str();
}

Aluno AlunoMapper::fromCsvLine(const string& line) const {
    stringstream ss(line);
    string segment;
    vector<string> fields;

    while (getline(ss, segment, ',')) {
        fields.push_back(std::move(segment));
    }

    if (fields.size() < MIN_FIELDS) {
        throw invalid_argument("Registro de Aluno malformado: esperados " +
                               to_string(MIN_FIELDS) + " campos, encontrados " +
                               to_string(fields.size()) + ".");
    }

    Aluno aluno;

    try {
        aluno.setId(stol(fields[0]));
        aluno.setMatricula(stol(fields[4]));
        aluno.setNome(std::move(fields[1]));
        aluno.setEmail(std::move(fields[2]));
        aluno.setSenha(std::move(fields[3]));
    } catch (const exception& e) {
        throw invalid_argument(
            "Falha ao analisar campos numéricos (ID/Matrícula) na linha CSV: " +
            line);
    }

    return aluno;
}