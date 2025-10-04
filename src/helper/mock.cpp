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
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    return lines;
}

void writeAllLines(const string& filename, const vector<string>& lines) {
    ofstream file(filename, ios::trunc);

    if (!file.is_open()) {
        cerr << "Error: Could not open file for writing: " << filename << endl;
        return;
    }
    for (const string& line : lines) {
        file << line << "\n";
    }
}

long extractIdFromLine(const string& line) {
    size_t comma_pos = line.find(',');
    string id_str;

    if (comma_pos != string::npos) {
        id_str = line.substr(0, comma_pos);
    } else {
        id_str = line;
    }

    try {
        return stol(id_str);
    } catch (const exception& e) {
        return -1;
    }
}

bool insert(string filename, string data) {
    vector<string> lines = readAllLines(filename);
    long max_id = 0;

    size_t data_start_index = 0;
    if (!lines.empty() && extractIdFromLine(lines[0]) == -1) {
        data_start_index = 1;
    }

    for (size_t i = data_start_index; i < lines.size(); ++i) {
        long current_id = extractIdFromLine(lines[i]);
        if (current_id > max_id) {
            max_id = current_id;
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
        cerr << "Error: Could not open file for appending: " << filename
             << endl;
        return false;
    }
    file << new_record << "\n";

    return true;
}

string find(string filename, long id) {
    vector<string> lines = readAllLines(filename);
    string id_str = to_string(id);

    size_t data_start_index = 0;
    if (!lines.empty() && extractIdFromLine(lines[0]) == -1) {
        data_start_index = 1;
    }

    for (size_t i = data_start_index; i < lines.size(); ++i) {
        const string& line = lines[i];
        long current_id = extractIdFromLine(line);

        if (current_id == id) {
            return line;
        }
    }
    return "";
}

vector<string> findAll(string filename) {
    vector<string> all_lines = readAllLines(filename);
    if (all_lines.empty()) {
        return {};
    }

    size_t data_start_index = 0;
    if (extractIdFromLine(all_lines[0]) == -1) {
        data_start_index = 1;
    }

    if (all_lines.size() > data_start_index) {
        return vector<string>(all_lines.begin() + data_start_index,
                              all_lines.end());
    }
    return {};
}

string update(string filename, long id, string data) {
    vector<string> lines = readAllLines(filename);
    string id_str = to_string(id);
    string new_record;
    bool updated = false;

    if (data.empty()) {
        new_record = id_str;
    } else {
        new_record = id_str + "," + data;
    }

    size_t data_start_index = 0;
    if (!lines.empty() && extractIdFromLine(lines[0]) == -1) {
        data_start_index = 1;
    }

    for (size_t i = data_start_index; i < lines.size(); ++i) {
        string& line = lines[i];
        long current_id = extractIdFromLine(line);

        if (current_id == id) {
            line = new_record;
            updated = true;
            break;
        }
    }

    if (updated) {
        writeAllLines(filename, lines);
        return new_record;
    }

    return "";
}

bool remove(string filename, long id) {
    vector<string> lines = readAllLines(filename);
    size_t initial_size = lines.size();

    auto start_it = lines.begin();
    if (!lines.empty() && extractIdFromLine(lines[0]) == -1) {
        start_it++;
    }

    lines.erase(remove_if(start_it, lines.end(),
                          [&id](const string& line) {
                              return extractIdFromLine(line) == id;
                          }),
                lines.end());

    if (lines.size() < initial_size) {
        writeAllLines(filename, lines);
        return true;
    }

    return false;
}