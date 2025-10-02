#include <iostream>
#include <cstdlib>
#include "funcoes.hpp"

using namespace std;

int main() {
    int opcao = -1;

    do {
        desenhar_relogio();
        imprimir_opcoes();

        cin >> opcao;

        switch (opcao) {
            case 0:
                cout << "\n>> Saindo do programa" << endl;
                break;
            default:
                cout << "\n>> Opcao invalida! Tente novamente" << endl;
                break;
        }

        if (opcao != 0) {
            cout << "\nPressione Enter para continuar";
            cin.ignore();
            cin.get();
        }

    } while (opcao != 0);

    return 0;
}
