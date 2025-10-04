#include "funcoes.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

void desenhar_relogio() {
    cout << "*-------------------------------*" << endl;
    cout << "|       AGENDAMENTO DE          |" << endl;
    cout << "|           HORARIOS            |" << endl;
    cout << "|          .---.                |" << endl;
    cout << "|         /   / \\               |" << endl;
    cout << "|        |   o   |              |" << endl;
    cout << "|         \\ | /               |" << endl;
    cout << "|          '---'                |" << endl;
    cout << "*-------------------------------*" << endl;
    cout << endl;
}

void imprimir_login() {
    cout << "MENU PRINCIPAL:" << endl;
    cout << "1 - Criar Novo Aluno (Cadastro)" << endl;
    cout << "2 - Fazer Login" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}

void imprimir_opcoes() {
    cout << "MENU DE OPCOES:" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}

void criar_aluno(AlunoController& controller) {
    string nome, email, senha;
    long matricula;

    cout << "\n--- Criar Novo Aluno ---" << endl;

    cout << "Nome: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nome);

    cout << "E-mail: ";
    getline(cin, email);

    cout << "Senha: ";
    getline(cin, senha);

    cout << "Matrícula: ";

    if (!(cin >> matricula)) {
        cout << ">> ERRO: A matrícula deve ser um número inteiro." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    try {
        Aluno novo_aluno = controller.create(nome, email, senha, matricula);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Aluno criado com ID: " << novo_aluno.getId()
             << endl;
        cout << "   Nome: " << novo_aluno.getNome() << endl;
        cout << "   Matrícula: " << novo_aluno.getMatricula() << endl;
        cout << "==================================================" << endl;

    } catch (const std::invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao salvar o Aluno."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a criação."
             << endl;
    }
}