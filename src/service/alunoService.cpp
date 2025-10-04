#include "service/alunoService.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "dto/alunoDTO.hpp"
#include "helper/mock.hpp"
#include "model/aluno.hpp"
using namespace std;

#define ALUNO_FILE "data/usuarios.csv"
#define EMAIL_COL_INDEX 2
#define MATRICULA_COL_INDEX 4

Aluno AlunoService::mapAndInjectAgendamentos(const string& csv_line) const {
    Aluno aluno;

    try {
        aluno = mapper.fromCsvLine(csv_line);
    } catch (const invalid_argument& e) {
        throw runtime_error("Falha ao processar registro CSV: " +
                            string(e.what()));
    }

    vector<Agendamento> agendamentos = service.listByIdAluno(aluno.getId());
    aluno.setAgendamentos(move(agendamentos));

    return aluno;
}

vector<Aluno> AlunoService::getByEmail(const string& email) const {
    vector<string> csv_matches;
    vector<Aluno> alunos;

    try {
        csv_matches = findByColumn(ALUNO_FILE, EMAIL_COL_INDEX, email);
    } catch (const invalid_argument& e) {
        return {};
    } catch (const runtime_error& e) {
        throw;
    }

    for (const string& csv_line : csv_matches) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            cerr << "Aviso: Pulando registro malformado durante getByEmail: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

vector<Aluno> AlunoService::getByMatricula(long matricula) const {
    vector<string> csv_matches;
    vector<Aluno> alunos;
    string sMatricula = to_string(matricula);

    try {
        csv_matches = findByColumn(ALUNO_FILE, MATRICULA_COL_INDEX, sMatricula);
    } catch (const invalid_argument& e) {
        return {};
    } catch (const runtime_error& e) {
        throw;
    }

    for (const string& csv_line : csv_matches) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            cerr
                << "Aviso: Pulando registro malformado durante getByMatricula: "
                << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

bool AlunoService::existsByEmail(string email) const {
    return !getByEmail(email).empty();
}

bool AlunoService::existsByEmailAndIdNot(string email, long id) const {
    vector<Aluno> email_matches = getByEmail(email);

    for (const Aluno& aluno : email_matches) {
        if (aluno.getId() != id) {
            return true;
        }
    }

    return false;
}

bool AlunoService::existsByMatricula(long matricula) const {
    return !getByMatricula(matricula).empty();
}

bool AlunoService::existsByMatriculaAndIdNot(long matricula, long id) const {
    vector<Aluno> matricula_matches = getByMatricula(matricula);

    for (const Aluno& aluno : matricula_matches) {
        if (aluno.getId() != id) {
            return true;
        }
    }

    return false;
}

AlunoService::AlunoService(const AlunoMapper& mapper,
                           const AgendamentoService& service)
    : mapper(mapper), service(service) {}

Aluno AlunoService::save(const AlunoDTO& aluno) const {
    if (existsByEmail(aluno.getEmail())) {
        throw invalid_argument("O email '" + aluno.getEmail() +
                               "' já está em uso por outro aluno.");
    }

    if (existsByMatricula(aluno.getMatricula())) {
        throw invalid_argument("A matrícula '" +
                               to_string(aluno.getMatricula()) +
                               "' já está registrada.");
    }

    string data_csv = mapper.toCsvData(aluno);
    long id = insert(ALUNO_FILE, data_csv);

    string new_record_csv = to_string(id) + "," + data_csv;

    try {
        return mapAndInjectAgendamentos(new_record_csv);
    } catch (const runtime_error& e) {
        throw runtime_error(
            "Inserção bem-sucedida (ID: " + to_string(id) +
            "), mas falha ao processar o registro: " + string(e.what()));
    }
}

optional<Aluno> AlunoService::getById(long id) const {
    try {
        string csv_line = find(ALUNO_FILE, id);

        return mapAndInjectAgendamentos(csv_line);
    } catch (const invalid_argument& e) {
        return nullopt;
    } catch (const runtime_error& e) {
        throw;
    }
}

vector<Aluno> AlunoService::listAll() const {
    vector<string> csv_records = findAll(ALUNO_FILE);
    vector<Aluno> alunos;

    for (const string& csv_line : csv_records) {
        try {
            alunos.push_back(mapAndInjectAgendamentos(csv_line));
        } catch (const runtime_error& e) {
            cerr << "Aviso: Pulando registro malformado durante listAll: "
                 << csv_line << " (" << e.what() << ")" << endl;
        }
    }

    return alunos;
}

optional<Aluno> AlunoService::updateById(long id, const AlunoDTO& aluno) const {
    if (existsByEmailAndIdNot(aluno.getEmail(), id)) {
        throw invalid_argument("O email '" + aluno.getEmail() +
                               "' já está em uso por outro aluno.");
    }

    if (existsByMatriculaAndIdNot(aluno.getMatricula(), id)) {
        throw invalid_argument("A matrícula '" +
                               to_string(aluno.getMatricula()) +
                               "' já está registrada por outro aluno.");
    }

    string data_csv = mapper.toCsvData(aluno);
    string updated_csv;

    try {
        update(ALUNO_FILE, id, data_csv);
        updated_csv = to_string(id) + "," + data_csv;
    } catch (const invalid_argument& e) {
        return nullopt;
    } catch (const runtime_error& e) {
        throw;
    }

    return mapAndInjectAgendamentos(updated_csv);
}

bool AlunoService::deleteById(long id) const {
    try {
        remove(ALUNO_FILE, id);
        return true;
    } catch (const invalid_argument& e) {
        return false;
    } catch (const runtime_error& e) {
        throw;
    }
}