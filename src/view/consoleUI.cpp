#include "view/consoleUI.hpp"

#include <iostream>

#include "util/utils.hpp"
using namespace std;

ConsoleUI::ConsoleUI(const shared_ptr<SessionService>& ss)
    : sessionService(ss) {}

static void imprimir_menu() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "1 - Voltar" << endl;
    cout << "0 - Deslogar" << endl;
}

void ConsoleUI::fazer_logout() {
    if (!sessionService->isLogged())
        return;

    imprimir_menu();

    int opcao = read_integer_range("Escolha uma opcao: ", 0, 1);

    if (opcao == 0) {
        sessionService->logout();
        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Logout realizado." << endl;
        cout << "==================================================" << endl;
    } else {
        cout << "\n>> Operação de logout cancelada." << endl;
    }
}