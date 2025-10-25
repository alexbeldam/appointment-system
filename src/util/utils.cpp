#include "util/utils.hpp"

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