#include "service/professorService.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "helper/mock.hpp"
using namespace std;

// Definições de macros
#define PROFESSOR_FILE "data/professores.csv"
#define EMAIL_COL_INDEX 2

// Construtor: Realiza a Injeção de Dependência das referências constantes
ProfessorService::ProfessorService(const ProfessorMapper& mapper,
                                   const HorarioService& service)
    : mapper(mapper), service(service) {}

// --- MÉTODOS PRIVADOS/AUXILIARES ---

// Converte uma linha CSV em Model e injeta Horarios (Responsabilidade do
// Service)
Professor ProfessorService::mapAndInjectHorarios(const string& csv_line) const {
    Professor professor;

    // 1. Mapeamento CSV -> Model (Mapper lança invalid_argument em caso de
    // dados malformados)
    try {
        professor = mapper.fromCsvLine(csv_line);
    } catch (const invalid_argument& e) {
        // Promove o erro de mapeamento para runtime_error (indicando falha
        // crítica no registro)
        throw runtime_error("Falha ao processar registro CSV: " +
                            string(e.what()));
    }

    // 2. Injeção de Horarios (Consulta a serviço externo/outra entidade)
    // NOTE: HorarioService::listDisponivelByIdProfessor é a dependência
    // injetada.
    vector<Horario> horarios =
        service.listDisponivelByIdProfessor(professor.getId());
    // Usa std::move para transferência eficiente do vetor de Horarios
    professor.setHorarios(move(horarios));

    return professor;
}

// Helper: Busca todos os Professors por Email (usa o DAL e mapeia o resultado)
vector<Professor> ProfessorService::getByEmail(const string& email) const {
    vector<string> csv_matches;
    vector<Professor> professors;

    try {
        // Chamada ao DAL. findByColumn retorna vetor vazio ou lança
        // runtime_error (I/O)
        csv_matches = findByColumn(PROFESSOR_FILE, EMAIL_COL_INDEX, email);
    } catch (const invalid_argument& e) {
        // Captura "Nenhum registro encontrado" do DAL e traduz para um vetor
        // vazio
        return {};
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O críticos
    }

    // Mapeia todas as linhas CSV válidas para Model
    for (const string& csv_line : csv_matches) {
        try {
            professors.push_back(mapAndInjectHorarios(csv_line));
        } catch (const runtime_error& e) {
            // Logs e ignora registros malformados encontrados
            cerr << "Aviso: Pulando registro malformado durante getByEmail: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return professors;
}

// --- MÉTODOS DE VALIDAÇÃO DE NEGÓCIO ---

bool ProfessorService::existsByEmail(string email) const {
    // Verifica se a lista retornada por getByEmail não está vazia
    return !getByEmail(email).empty();
}

bool ProfessorService::existsByEmailAndIdNot(string email, long id) const {
    vector<Professor> email_matches = getByEmail(email);

    // Itera os Models e compara o ID para verificar unicidade
    for (const Professor& professor : email_matches) {
        if (professor.getId() != id) {
            return true;  // Encontrado outro professor com o mesmo email
        }
    }

    return false;
}

// --- MÉTODOS PÚBLICOS (CRUD) ---

// CREATE
Professor ProfessorService::save(const ProfessorDTO& professor) const {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade)
    if (existsByEmail(professor.getEmail())) {
        throw invalid_argument("O email '" + professor.getEmail() +
                               "' já está em uso por outro professor.");
    }

    // 2. DAL WRITE
    string data_csv = mapper.toCsvData(professor);
    long id =
        insert(PROFESSOR_FILE, data_csv);  // Persiste os dados e obtém o novo
                                           // ID (lança runtime_error em I/O)

    // 3. MONTAGEM DO MODELO (Evita leitura desnecessária do DB)
    string new_record_csv = to_string(id) + "," + data_csv;

    try {
        // Mapeia e injeta Horarios (lança runtime_error em mapeamento)
        return mapAndInjectHorarios(new_record_csv);
    } catch (const runtime_error& e) {
        // Erro crítico após sucesso na escrita: relança com contexto
        throw runtime_error(
            "Inserção bem-sucedida (ID: " + to_string(id) +
            "), mas falha ao processar o registro: " + string(e.what()));
    }
}

// READ BY ID
optional<Professor> ProfessorService::getById(long id) const {
    try {
        // find() lança invalid_argument se ID não existe, runtime_error se I/O
        // falhar
        string csv_line = find(PROFESSOR_FILE, id);

        // Se encontrou, mapeia e retorna o optional<Professor> com valor
        return mapAndInjectHorarios(csv_line);
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" e retorna optional vazio
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O
    }
}

// LIST ALL
vector<Professor> ProfessorService::listAll() const {
    vector<string> csv_records =
        findAll(PROFESSOR_FILE);  // Pega todas as linhas (exclui cabeçalho)
    vector<Professor> professors;

    // Mapeia e injeta Horarios para todos os registros
    for (const string& csv_line : csv_records) {
        try {
            professors.push_back(mapAndInjectHorarios(csv_line));
        } catch (const runtime_error& e) {
            // Logs e ignora registros malformados
            cerr << "Aviso: Pulando registro malformado durante listAll: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return professors;
}

// UPDATE
optional<Professor> ProfessorService::updateById(
    long id, const ProfessorDTO& professor) const {
    // 1. VALIDAÇÃO DE NEGÓCIO (Unicidade para UPDATE, excluindo o próprio ID)
    if (existsByEmailAndIdNot(professor.getEmail(), id)) {
        throw invalid_argument("O email '" + professor.getEmail() +
                               "' já está em uso por outro professor.");
    }

    // 2. DAL WRITE (Atualiza a linha no arquivo)
    string data_csv = mapper.toCsvData(professor);
    string updated_csv;

    try {
        // update() lança invalid_argument se ID não existe
        update(PROFESSOR_FILE, id, data_csv);

        // Constrói a linha CSV completa do registro atualizado (ID + novos
        // dados)
        updated_csv = to_string(id) + "," + data_csv;
    } catch (const invalid_argument& e) {
        // Captura "ID não existe" e retorna optional vazio
        return nullopt;
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O
    }

    // 3. MONTAGEM DO MODELO
    return mapAndInjectHorarios(updated_csv);
}

// DELETE
bool ProfessorService::deleteById(long id) const {
    try {
        // remove() lança invalid_argument se ID não existe
        remove(PROFESSOR_FILE, id);
        return true;  // Sucesso na remoção
    } catch (const invalid_argument& e) {
        return false;  // ID não encontrado
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O
    }
}