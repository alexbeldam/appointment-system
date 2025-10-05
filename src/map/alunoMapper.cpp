#include "map/alunoMapper.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>
using namespace std;

// Definindo o número mínimo esperado de campos na linha CSV, incluindo o ID.
// ID (0), Nome (1), Email (2), Senha (3), Matrícula (4). Total de 5 campos.
#define MIN_FIELDS 5

// --- Mapeamento DTO <-> Model ---

// Converte um AlunoDTO (dados de entrada) para um objeto Aluno (entidade de
// domínio). NOTE: Este método só lida com campos básicos; Agendamentos são
// injetados pelo Service.
Aluno AlunoMapper::map(const AlunoDTO& dto) const {
    Aluno aluno;

    aluno.setId(dto.getId());
    aluno.setEmail(dto.getEmail());
    aluno.setNome(dto.getNome());
    aluno.setSenha(dto.getSenha());
    aluno.setMatricula(dto.getMatricula());

    return aluno;
}

// Converte um objeto Aluno (entidade de domínio) de volta para um DTO (saída).
// Usado tipicamente quando o Service retorna dados para o Controller.
AlunoDTO AlunoMapper::map(const Aluno& aluno) const {
    AlunoDTO dto;

    dto.setId(aluno.getId());
    dto.setEmail(aluno.getEmail());
    dto.setNome(aluno.getNome());
    dto.setSenha(aluno.getSenha());
    dto.setMatricula(aluno.getMatricula());
    // NOTE: Agendamentos não são mapeados para o DTO, mantendo-o simples.

    return dto;
}

// --- Serialização CSV ---

// Converte o DTO em uma string CSV de dados (sem o ID), para uso no
// DAL/insert/update.
string AlunoMapper::toCsvData(const AlunoDTO& dto) const {
    stringstream ss;
    // Ordem dos campos: Nome, Email, Senha, Matrícula
    ss << dto.getNome() << "," << dto.getEmail() << "," << dto.getSenha() << ","
       << dto.getMatricula();
    return ss.str();
}

// --- Desserialização CSV ---

// Converte uma linha CSV completa (incluindo ID) em um objeto Aluno (Modelo).
// Lida com a tokenização, validação e conversão de tipos.
Aluno AlunoMapper::fromCsvLine(const string& line) const {
    stringstream ss(line);
    string segment;
    vector<string> fields;

    // 1. Tokenização da linha CSV
    while (getline(ss, segment, ',')) {
        // Usa std::move para evitar a cópia da string do segmento para o vetor.
        fields.push_back(move(segment));
    }

    // 2. Validação da Estrutura (Business Logic)
    if (fields.size() < MIN_FIELDS) {
        // Lança exceção se o registro não tiver o número esperado
        // de campos.
        throw invalid_argument("Registro de Aluno malformado: esperados " +
                               to_string(MIN_FIELDS) + " campos, encontrados " +
                               to_string(fields.size()) + ".");
    }

    Aluno aluno;

    // 3. Conversão de Tipos e Mapeamento
    try {
        // Conversão de campos numéricos (mais propensos a lançar exceção).
        // Índice 0: ID
        aluno.setId(stol(fields[0]));
        // Índice 4: Matrícula
        aluno.setMatricula(stol(fields[4]));

        // Mapeamento de strings (usando move semantics para evitar cópia
        // desnecessária)
        aluno.setNome(move(fields[1]));
        aluno.setEmail(move(fields[2]));
        aluno.setSenha(move(fields[3]));

        // Agendamentos ficam vazios (default-initialized) e serão injetados
        // pelo Service.

    } catch (const exception& e) {
        // Captura exceções de stol (invalid_argument/out_of_range) e relança um
        // erro de formato específico.
        throw invalid_argument(
            "Falha ao analisar campos numéricos (ID/Matrícula) na linha CSV: " +
            line);
    }

    return aluno;
}