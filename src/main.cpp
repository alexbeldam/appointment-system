#include <cstdlib>
#include <iostream>
#include <limits>

#include "funcoes.hpp"

using namespace std;

int main() {
    const AlunoMapper mapper;
    const AgendamentoService aService;
    const AlunoService service(mapper, aService);
    const AlunoController alunoController(service);
    int opcao = -1;

    do {
        desenhar_relogio();
        imprimir_login();

        if (!(cin >> opcao)) {
            cout << "\n>> Entrada inválida. Por favor, digite um número."
                 << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcao) {
            case 0:
                cout << "\n>> Saindo do programa" << endl;
                break;

            case 1:
                criar_aluno(alunoController);
                break;

            case 2:
                cout << "\n>> A funcionalidade de login será implementada em "
                        "breve."
                     << endl;
                break;

            default:
                cout << "\n>> Opcao invalida! Tente novamente" << endl;
                break;
        }

        if (opcao != 0) {
            cout << "\nPressione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    } while (opcao != 0);

    return 0;
}
