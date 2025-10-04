#include "service/alunoService.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "dto/alunoDTO.hpp"
#include "helper/mock.hpp"
#include "model/aluno.hpp"
using namespace std;

const string ALUNO_FILE = "data/usuarios.csv";
const int MIN_FIELDS = 5;

string to_csv_data(const AlunoDTO& aluno) {
    stringstream ss;
    ss << aluno.getNome() << "," << aluno.getEmail() << "," << aluno.getSenha()
       << "," << aluno.getMatricula();
    return ss.str();
}

Aluno from_csv_line(const string& csv_line, AlunoMapper& mapper) {
    stringstream ss(csv_line);
    string segment;
    vector<string> fields;

    while (getline(ss, segment, ',')) {
        fields.push_back(segment);
    }

    if (fields.size() < MIN_FIELDS) {
        throw runtime_error("Registro de Aluno malformado: esperados " +
                            to_string(MIN_FIELDS) + " campos, encontrados " +
                            to_string(fields.size()));
    }

    AlunoDTO dto;
    try {
        dto.setId(stol(fields[0]));
        dto.setNome(fields[1]);
        dto.setEmail(fields[2]);
        dto.setSenha(fields[3]);
        dto.setMatricula(stol(fields[4]));
    } catch (const exception& e) {
        throw runtime_error(
            "Falha ao analisar campos numéricos na linha CSV: " + csv_line);
    }

    return mapper.map(dto);
}

Aluno AlunoService::save(const AlunoDTO& aluno) {
    string data_csv = to_csv_data(aluno);

    if (!insert(ALUNO_FILE, data_csv)) {
        throw runtime_error(
            "Falha ao gravar o novo registro de Aluno no arquivo.");
    }

    long max_id = 0;
    string new_record_csv;

    vector<string> all_records = findAll(ALUNO_FILE);
    for (const string& record : all_records) {
        try {
            size_t comma_pos = record.find(',');
            long current_id = stol(record.substr(0, comma_pos));

            if (current_id > max_id) {
                max_id = current_id;
                new_record_csv = record;
            }
        } catch (const exception& e) {
        }
    }

    if (max_id > 0) {
        try {
            return from_csv_line(new_record_csv, mapper);
        } catch (const runtime_error& e) {
            throw runtime_error(
                "Inserção bem-sucedida, mas falha ao processar o novo "
                "registro: " +
                string(e.what()));
        }
    }

    throw runtime_error(
        "Inserção bem-sucedida, mas falha ao recuperar o novo registro (falha "
        "na recuperação do ID).");
}

Aluno AlunoService::getById(long id) {
    string csv_line = find(ALUNO_FILE, id);

    if (csv_line.empty()) {
        throw runtime_error("Aluno com ID " + to_string(id) +
                            " não encontrado.");
    }

    try {
        return from_csv_line(csv_line, mapper);
    } catch (const runtime_error& e) {
        throw runtime_error("Registro encontrado para o ID " + to_string(id) +
                            ", mas falha ao analisar: " + string(e.what()));
    }
}

vector<Aluno> AlunoService::listAll() {
    vector<string> csv_records = findAll(ALUNO_FILE);
    vector<Aluno> alunos;

    for (const string& csv_line : csv_records) {
        try {
            alunos.push_back(from_csv_line(csv_line, mapper));
        } catch (const runtime_error& e) {
            cerr << "Aviso: Pulando registro malformado durante listAll: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

Aluno AlunoService::updateById(long id, const AlunoDTO& aluno) {
    string data_csv = to_csv_data(aluno);

    string updated_csv = update(ALUNO_FILE, id, data_csv);

    if (updated_csv.empty()) {
        throw runtime_error("Falha ao atualizar Aluno com ID " + to_string(id) +
                            ": registro não encontrado.");
    }

    try {
        return from_csv_line(updated_csv, mapper);
    } catch (const runtime_error& e) {
        throw runtime_error(
            "Atualização bem-sucedida, mas falha ao processar o registro "
            "atualizado: " +
            string(e.what()));
    }
}

bool AlunoService::deleteById(long id) {
    if (!remove(ALUNO_FILE, id)) {
        throw runtime_error("Falha ao excluir Aluno com ID " + to_string(id) +
                            ": registro não encontrado.");
    }
    return true;
}