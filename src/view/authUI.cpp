#include "view/authUI.hpp"

#include <iostream>
#include <stdexcept>

#include "util/utils.hpp"
using namespace std;

static void imprimir_menu();
static void imprimir_menu_login();
static void imprimir_menu_signup();

AuthUI::AuthUI(const AlunoController& ac, const ProfessorController& pc,
               const LoginController& lc, SessionManager& sm)
    : ConsoleUI(sm),
      alunoController(ac),
      loginController(lc),
      professorController(pc) {}

void AuthUI::fazer_login() const {
    imprimir_menu_login();

    int opcao = read_integer_range("Escolha uma opcao: ", 0, 2);

    switch (opcao) {
        case 0:
            cout << "\n>> Operação cancelada." << endl;
            return;
        case 1:
            login_aluno();
            break;
        case 2:
            login_professor();
    }
}

void AuthUI::fazer_cadastro() const {
    imprimir_menu_signup();

    int opcao = read_integer_range("Escolha uma opcao: ", 0, 2);

    switch (opcao) {
        case 0:
            cout << "\n>> Operação cancelada." << endl;
            return;
        case 1:
            cadastro_aluno();
            break;
        case 2:
            cadastro_professor();
    }
}

void AuthUI::login_aluno() const {
    string email, senha;

    cout << "\n--- Entrar Aluno ---" << endl;

    cout << "E-mail: ";
    getline(cin, email);
    cout << "Senha: ";
    getline(cin, senha);

    try {
        const auto& aluno = loginController.loginAluno(email, senha);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Seja bem vinda(o) " << aluno.getNome() << endl;
        cout << "==================================================" << endl;

    } catch (const std::invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao logar aluno." << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a autenticação."
             << endl;
    }
}

void AuthUI::login_professor() const {
    string email, senha;

    cout << "\n--- Entrar Professor ---" << endl;

    cout << "E-mail: ";
    getline(cin, email);
    cout << "Senha: ";
    getline(cin, senha);

    try {
        const auto& professor = loginController.loginProfessor(email, senha);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Seja bem vinda(o) " << professor.getNome() << endl;
        cout << "==================================================" << endl;

    } catch (const std::invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao logar professor."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a autenticação."
             << endl;
    }
}

void AuthUI::cadastro_aluno() const {
    string nome, email, senha;
    long matricula;

    cout << "\n--- Cadastrar Novo Aluno ---" << endl;

    cout << "Nome: ";
    getline(cin, nome);

    cout << "E-mail: ";
    getline(cin, email);

    cout << "Senha: ";
    getline(cin, senha);

    cout << "Matrícula: ";
    if (!(cin >> matricula)) {
        cout << "\n>> ERRO: A matrícula deve ser um número inteiro." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try {
        const auto& novo_aluno =
            alunoController.create(nome, email, senha, matricula);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Aluno cadastrado:" << endl;
        cout << "     Nome: " << novo_aluno.getNome() << endl;
        cout << "     Matrícula: " << novo_aluno.getMatricula() << endl;
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

void AuthUI::cadastro_professor() const {
    string nome, email, senha, disciplina;

    cout << "\n--- Cadastrar Novo Professor ---" << endl;

    cout << "Nome: ";
    getline(cin, nome);
    cout << "E-mail: ";
    getline(cin, email);
    cout << "Senha: ";
    getline(cin, senha);
    cout << "Disciplina: ";
    getline(cin, disciplina);

    try {
        const auto& novo_professor =
            professorController.create(nome, email, senha, disciplina);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Professor cadastrado:" << endl;
        cout << "   Nome: " << novo_professor.getNome() << endl;
        cout << "   Disciplina: " << disciplina << endl;
        cout << "==================================================" << endl;

    } catch (const std::invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao salvar o Professor."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a criação."
             << endl;
    }
}

bool AuthUI::show() const {
    while (!sessionManager.isLogged()) {
        desenhar_relogio();
        imprimir_menu();

        int opcao = read_integer_range("Escolha uma opcao: ", 0, 3);

        switch (opcao) {
            case 0:
                return false;
            case 1:
                fazer_login();
                break;
            case 2:
                fazer_cadastro();
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }

    return true;
}

void imprimir_menu() {
    cout << "MENU PRINCIPAL:" << endl;
    cout << "1 - Fazer Login" << endl;
    cout << "2 - Fazer cadastro" << endl;
    cout << "0 - Sair do programa" << endl;
}

void imprimir_menu_login() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "1 - Entrar como aluno" << endl;
    cout << "2 - Entrar como professor" << endl;
    cout << "0 - Voltar ao menu principal" << endl;
}

void imprimir_menu_signup() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "1 - Cadastar aluno" << endl;
    cout << "2 - Cadastrar professor" << endl;
    cout << "0 - Voltar ao menu principal" << endl;
}