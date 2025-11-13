#include "util/utils.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
using namespace std;

const string CHARSET =
    "./"
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
const size_t CHARSET_LEN = 64;
const size_t SALT_LEN = 16;
const size_t HASH_LEN = 11;
const string ALGO_PREFIX = "$mk$";

string generate_salt() {
    auto rand_char = []() -> char {
        static random_device rd;
        static mt19937 generator(rd());
        static uniform_int_distribution<> distribution(0, CHARSET_LEN - 1);
        return CHARSET[distribution(generator)];
    };

    string salt(SALT_LEN, 0);
    generate_n(salt.begin(), SALT_LEN, rand_char);
    return salt;
}

string encode_digest(size_t hash) {
    string encoded_hash = "";

    for (size_t i = 0; i < HASH_LEN; ++i) {
        size_t index = hash % CHARSET_LEN;
        encoded_hash += CHARSET[index];
        hash /= CHARSET_LEN;
    }

    reverse(encoded_hash.begin(), encoded_hash.end());
    return encoded_hash;
}

string mock_bcrypt(const string& pwd, int cost_factor) {
    int iterations = static_cast<int>(pow(2, cost_factor));

    string salt = generate_salt();

    string to_hash = pwd + salt;
    size_t hash_value = hash<string>{}(to_hash);

    for (int i = 0; i < iterations; ++i) {
        to_hash = to_string(hash_value);
        hash_value = hash<string>{}(to_hash);
    }

    string encoded_hash = encode_digest(hash_value);

    stringstream cf_ss;
    cf_ss << setw(2) << setfill('0') << cost_factor;
    string cf_str = cf_ss.str();

    return ALGO_PREFIX + cf_str + "$" + salt + encoded_hash;
}

bool check(const string& cypher, const string& pwd) {
    stringstream ss(cypher);
    string segment;
    vector<string> parts;

    while (getline(ss, segment, '$')) {
        parts.push_back(segment);
    }

    if (parts.size() != 4 || parts[1] != "mk" ||
        cypher.length() != 7 + SALT_LEN + HASH_LEN) {
        return false;
    }

    int cost_factor = stoi(parts[2]);
    string salt_hash = parts[3];

    string stored_salt = salt_hash.substr(0, SALT_LEN);
    string stored_digest = salt_hash.substr(SALT_LEN, HASH_LEN);

    int iterations = static_cast<int>(pow(2, cost_factor));

    string to_hash = pwd + stored_salt;
    size_t hash_value = hash<string>{}(to_hash);

    for (int i = 0; i < iterations; ++i) {
        to_hash = to_string(hash_value);
        hash_value = hash<string>{}(to_hash);
    }

    string new_digest = encode_digest(hash_value);

    return new_digest == stored_digest;
}

int read_integer_range(const string& prompt, int min_val, int max_val) {
    int valor;

    while (true) {
        cout << prompt;

        if (cin >> valor) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (valor >= min_val && valor <= max_val)
                return valor;

            cout << "\n>> Valor fora do intervalo permitido (" << min_val
                 << " - " << max_val << ").\n";
        } else {
            cout << "\n>> Entrada inválida. Por favor, digite um número.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Pressione Enter para tentar novamente...";
        cin.get();
        cout << "\n";
    }
}

void desenhar_relogio() {
    cout << "*-------------------------------*" << endl;
    cout << "|       AGENDAMENTO DE          |" << endl;
    cout << "|           HORARIOS            |" << endl;
    cout << "|           .---.               |" << endl;
    cout << "|          /   / \\              |" << endl;
    cout << "|         |   o   |             |" << endl;
    cout << "|          \\ | /                |" << endl;
    cout << "|           '---'               |" << endl;
    cout << "*-------------------------------*" << endl;
    cout << endl;
}

string time_to_string(time_t tt) {
    tm tm_struct = *localtime(&tt);

    ostringstream ss;
    ss << put_time(&tm_struct, "%d/%m %H:%M");
    return ss.str();
}

time_t string_to_time(const string& timeStr) {
    tm tm_struct = {};
    istringstream ss(timeStr);

    ss >> get_time(&tm_struct, "%d/%m %H:%M");

    if (ss.fail()) {
        return (time_t)-1;
    }

    int original_day = tm_struct.tm_mday;
    int original_mon = tm_struct.tm_mon;

    const int ANO_BASE = 2025;
    tm_struct.tm_year = ANO_BASE - 1900;
    tm_struct.tm_isdst = -1;

    time_t result = mktime(&tm_struct);

    if (result == (time_t)-1 || tm_struct.tm_mday != original_day ||
        tm_struct.tm_mon != original_mon) {
        return (time_t)-1;
    }

    return result;
}