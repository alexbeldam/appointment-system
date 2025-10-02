#include <iostream>
#include "funcoes.hpp"

using namespace std;

void desenhar_relogio() {
    cout << "*-------------------------------*" << endl;
    cout << "|      AGENDAMENTO  DE          |" << endl;
    cout << "|           HORARIOS            |" << endl;
    cout << "|         .---.                 |" << endl;
    cout << "|        /   /  \\               |" << endl;
    cout << "|       |   o   |               |" << endl;
    cout << "|        \\  |  /                |" << endl;
    cout << "|         '---'                 |" << endl;
    cout << "*-------------------------------*" << endl;
    cout << endl; 
}

void imprimir_opcoes() {
    cout << "MENU DE OPCOES:" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}