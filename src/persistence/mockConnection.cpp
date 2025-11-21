#include "persistence/mockConnection.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

using std::exception;
using std::getline;
using std::ifstream;
using std::invalid_argument;
using std::ios;
using std::ofstream;
using std::runtime_error;
using std::stol;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

#define DATA_PATH_PREFIX "data/"

string getFullFilePath(const string& table_name) {
    return DATA_PATH_PREFIX + table_name + ".csv";
}

string extractColumnFromLine(const string& line, size_t col) {
    stringstream ss(line);
    string segment;
    size_t current_col = 0;

    while (getline(ss, segment, ',')) {
        if (current_col == col) {
            return segment;
        }
        current_col++;
    }
    throw invalid_argument("Coluna " + to_string(col) +
                           " não existe na linha.");
}

long getIdFromLine(const string& line) {
    string id_str;
    try {
        id_str = extractColumnFromLine(line, 0);
        return stol(id_str);
    } catch (const invalid_argument& e) {
        throw invalid_argument("A linha não contém um ID válido na coluna 0.");
    } catch (const exception& e) {
        throw invalid_argument("O valor do ID '" + id_str +
                               "' não é um número válido.");
    }
}

vector<string> readAllLines(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    if (!file.is_open()) {
        return lines;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

void writeAllLines(const string& filename, const vector<string>& lines) {
    ofstream file(filename, ios::trunc);

    if (!file.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo para escrita: '" +
                            filename + "'.");
    }
    for (const string& line : lines) {
        file << line << "\n";
    }
}

long MockConnection::insert(const string& table_name,
                            const string& data) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    long max_id = 0;

    for (size_t i = 1; i < lines.size(); ++i) {
        try {
            long current_id = getIdFromLine(lines[i]);
            if (current_id > max_id) {
                max_id = current_id;
            }
        } catch (const invalid_argument& ignore) {
        }
    }

    long new_id = max_id + 1;
    string new_record;

    if (data.empty()) {
        new_record = to_string(new_id);
    } else {
        new_record = to_string(new_id) + "," + data;
    }

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo '" + filename +
                            "' para anexar.");
    }
    file << new_record << "\n";

    return new_id;
}

string MockConnection::selectOne(const string& table_name, long id) const {
    vector<string> lines = selectByColumn(table_name, 0, to_string(id));

    if (lines.empty())
        throw runtime_error("O ID " + to_string(id) + " não existe na tabela " +
                            table_name + ".");
    else if (lines.size() > 1)
        throw runtime_error("Mais de uma linha com ID " + to_string(id) +
                            ". Integridade da tabela " + table_name +
                            " comprometida.");

    return lines.front();
}

vector<string> MockConnection::selectByColumn(const string& table_name,
                                              size_t index,
                                              const string& value) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    vector<string> results;

    for (size_t i = 1; i < lines.size(); ++i) {
        const string& line = lines[i];

        try {
            string col = extractColumnFromLine(line, index);
            if (col == value) {
                results.push_back(line);
            }
        } catch (const invalid_argument& ignore) {
            continue;
        }
    }

    return results;
}

vector<string> MockConnection::selectAll(const string& table_name) const {
    string filename = getFullFilePath(table_name);
    vector<string> all_lines = readAllLines(filename);

    if (all_lines.size() > 1) {
        return vector<string>(all_lines.begin() + 1, all_lines.end());
    }
    return {};
}

void MockConnection::update(const string& table_name, long id,
                            const string& data) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    string id_str = to_string(id);
    string new_record;
    bool updated = false;

    if (data.empty()) {
        new_record = id_str;
    } else {
        new_record = id_str + "," + data;
    }

    for (size_t i = 1; i < lines.size(); ++i) {
        string& line = lines[i];

        try {
            long current_id = getIdFromLine(line);
            if (current_id == id) {
                line = new_record;
                updated = true;
                break;
            }
        } catch (const invalid_argument& e) {
            continue;
        }
    }

    if (!updated) {
        throw runtime_error("O ID " + to_string(id) +
                            " não existe para ser atualizado na tabela " +
                            table_name + ".");
    }

    writeAllLines(filename, lines);
}

size_t MockConnection::deleteByColumn(const string& table_name, size_t index,
                                      const string& value) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    size_t initial_size = lines.size();

    if (initial_size <= 1) {
        return 0;
    }

    auto start_it = lines.begin() + 1;

    lines.erase(remove_if(start_it, lines.end(),
                          [&index, &value](const string& line) {
                              try {
                                  string col_value =
                                      extractColumnFromLine(line, index);
                                  return col_value == value;
                              } catch (const invalid_argument& e) {
                                  return false;
                              }
                          }),
                lines.end());

    size_t removed_count = initial_size - lines.size();

    if (removed_count > 0) {
        writeAllLines(filename, lines);
    }

    return removed_count;
}

void MockConnection::deleteRecord(const string& table_name, long id) const {
    string id_str = to_string(id);

    size_t removed_count = deleteByColumn(table_name, 0, id_str);

    if (removed_count == 0) {
        throw invalid_argument("O ID " + to_string(id) +
                               " não existe na tabela " + table_name + ".");
    }
}