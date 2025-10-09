#include "map/professorMapper.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>
using namespace std;

// Definindo o número mínimo esperado de campos na linha CSV, incluindo o ID.
// ID (0), Nome (1), Email (2), Senha (3), Disciplina (4). Total de 5 campos.
#define MIN_FIELDS 5

// --- Mapeamento DTO <-> Model ---

// Converte um ProfessorDTO (dados de entrada) para um objeto Professor
// (entidade de domínio). NOTE: Este método só lida com campos básicos;
// Horarios são injetados pelo Service.
Professor ProfessorMapper::map(const ProfessorDTO& dto) const {
    Professor professor;

    professor.setId(dto.getId());
    professor.setEmail(dto.getEmail());
    professor.setNome(dto.getNome());
    professor.setSenha(dto.getSenha());
    professor.setDisciplina(dto.getDisciplina());

    return professor;
}

// Converte um objeto Professor (entidade de domínio) de volta para um DTO
// (saída). Usado tipicamente quando o Service retorna dados para o Controller.
ProfessorDTO ProfessorMapper::map(const Professor& professor) const {
    ProfessorDTO dto;

    dto.setId(professor.getId());
    dto.setEmail(professor.getEmail());
    dto.setNome(professor.getNome());
    dto.setSenha(professor.getSenha());
    dto.setDisciplina(professor.getDisciplina());
    // NOTE: Horarios não são mapeados para o DTO, mantendo-o simples.

    return dto;
}

// --- Serialização CSV ---

// Converte o DTO em uma string CSV de dados (sem o ID), para uso no
// DAL/insert/update.
string ProfessorMapper::toCsvData(const ProfessorDTO& dto) const {
    stringstream ss;
    // Ordem dos campos: Nome, Email, Senha, Matrícula
    ss << dto.getNome() << "," << dto.getEmail() << "," << dto.getSenha() << ","
       << dto.getDisciplina();
    return ss.str();
}

// --- Desserialização CSV ---

// Converte uma linha CSV completa (incluindo ID) em um objeto Professor
// (Modelo). Lida com a tokenização, validação e conversão de tipos.
Professor ProfessorMapper::fromCsvLine(const string& line) const {
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
        throw invalid_argument("Registro de Professor malformado: esperados " +
                               to_string(MIN_FIELDS) + " campos, encontrados " +
                               to_string(fields.size()) + ".");
    }

    Professor professor;

    // 3. Conversão de Tipos e Mapeamento
    try {
        // Conversão de campos numéricos (mais propensos a lançar exceção).
        // Índice 0: ID
        professor.setId(stol(fields[0]));

        // Mapeamento de strings (usando move semantics para evitar cópia
        // desnecessária)
        professor.setNome(move(fields[1]));
        professor.setEmail(move(fields[2]));
        professor.setSenha(move(fields[3]));
        professor.setDisciplina(move(fields[4]));

        // Horarios ficam vazios (default-initialized) e serão injetados
        // pelo Service.

    } catch (const exception& e) {
        // Captura exceções de stol (invalid_argument/out_of_range) e relança um
        // erro de formato específico.
        throw invalid_argument(
            "Falha ao analisar campos numéricos (ID) na linha CSV: " + line);
    }

    return professor;
}