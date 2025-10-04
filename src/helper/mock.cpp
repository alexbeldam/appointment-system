#include "helper/mock.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

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

long extractIdFromLine(const string& line) {
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

long insert(const string& filename, const string& data) {
    vector<string> lines = readAllLines(filename);
    long max_id = 0;

    for (size_t i = 1; i < lines.size(); ++i) {
        try {
            long current_id = extractIdFromLine(lines[i]);
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

vector<string> findByColumn(const string& filename, size_t index,
                            const string& value) {
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

string find(const string& filename, long id) {
    vector<string> lines = findByColumn(filename, 0, to_string(id));

    if (lines.empty())
        throw invalid_argument("O ID " + to_string(id) + " não existe.");
    else if (lines.size() > 1)
        throw runtime_error("Mais de uma linha com ID " + to_string(id) +
                            ". Integridade do arquivo comprometida.");

    return lines.front();
}

vector<string> findAll(const string& filename) {
    vector<string> all_lines = readAllLines(filename);

    if (all_lines.size() > 1) {
        return vector<string>(all_lines.begin() + 1, all_lines.end());
    }
    return {};
}

void update(const string& filename, long id, const string& data) {
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
            long current_id = extractIdFromLine(line);
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
        throw invalid_argument("O ID " + to_string(id) +
                               " não existe para ser atualizado.");
    }

    writeAllLines(filename, lines);
}

void remove(const string& filename, long id) {
    vector<string> lines = readAllLines(filename);
    size_t initial_size = lines.size();

    auto start_it = lines.begin();
    if (lines.size() > 0) {
        start_it++;
    } else {
        throw invalid_argument("O ID " + to_string(id) + " não existe.");
    }

    lines.erase(remove_if(start_it, lines.end(),
                          [&id](const string& line) {
                              try {
                                  long current_id = extractIdFromLine(line);
                                  return current_id == id;
                              } catch (const invalid_argument& e) {
                                  return false;
                              }
                          }),
                lines.end());

    if (lines.size() >= initial_size) {
        throw invalid_argument("O id " + to_string(id) + " não existe.");
    }

    writeAllLines(filename, lines);
}