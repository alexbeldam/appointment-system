#include "util/utils.hpp"

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
using namespace std;

char map(char c) {
    return '0' + (c + 10) % 79;
}

string encrypt(const string& pwd) {
    stringstream ss;

    for (const char c : pwd)
        ss << map(c);

    return ss.str();
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