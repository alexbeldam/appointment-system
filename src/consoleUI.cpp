#include "consoleUI.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

ConsoleUI::ConsoleUI(const AlunoController& ac, const ProfessorController& pc,
                     const LoginController& lc, SessionManager& sm)
    : alunoController(ac),
      professorController(pc),
      loginController(lc),
      sessionManager(sm) {}

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
    cout << "2 - Criar Novo Professor (Cadastro)" << endl;
    cout << "3 - Fazer Login" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}

void imprimir_opcoes_login() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "0 - Entrar como aluno" << endl;
    cout << "1 - Entrar como professor" << endl;
    cout << "Escolha uma opcao: ";
}

void imprimir_opcoes_aluno() {
    cout << "MENU ALUNO:" << endl;
    cout << "1 - Logout" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}

void imprimir_opcoes_professor() {
    cout << "MENU PROFESSOR:" << endl;
    cout << "1 - Logout" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}

void imprimir_opcoes_logout() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "1 - Voltar" << endl;
    cout << "0 - Deslogar" << endl;
    cout << "Escolha uma opcao: ";
}

// Função de interface de console para a criação de um novo Aluno.
void ConsoleUI::criar_aluno() const {
    string nome, email, senha;
    long matricula;

    cout << "\n--- Criar Novo Aluno ---" << endl;

    // --- COLETA DE INPUTS (STRING) ---

    // O 'cin.ignore' é essencial aqui para limpar qualquer '\n' pendente no
    // buffer de operações anteriores (e.g., seleção de menu).
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Nome: ";
    // Usamos getline para ler a linha inteira, incluindo espaços no nome.
    getline(cin, nome);

    cout << "E-mail: ";
    getline(cin, email);

    cout << "Senha: ";
    getline(cin, senha);

    // --- COLETA DE INPUT NUMÉRICO ---

    cout << "Matrícula: ";

    // Tentativa de leitura do long. Se falhar, é porque o usuário digitou
    // texto.
    if (!(cin >> matricula)) {
        cout << ">> ERRO: A matrícula deve ser um número inteiro." << endl;
        // Limpa o estado de erro do cin e o buffer para permitir futuras
        // operações.
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;  // Retorna, pois não há dados válidos para processar.
    }

    // Limpa o buffer, pois a última leitura foi númerica
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // --- FLUXO DE PROCESSAMENTO E TRATAMENTO DE EXCEÇÕES ---

    try {
        // Delega a criação ao Controller (que valida e persiste).
        Aluno novo_aluno =
            alunoController.create(nome, email, senha, matricula);

        // Mensagem de sucesso e exibição dos dados retornados pelo Service.
        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Aluno criado com ID: " << novo_aluno.getId()
             << endl;
        cout << "    Nome: " << novo_aluno.getNome() << endl;
        cout << "    Matrícula: " << novo_aluno.getMatricula() << endl;
        cout << "==================================================" << endl;

    } catch (const std::invalid_argument& e) {
        // Captura erros de validação (formato, unicidade) vindos do
        // Controller/Service.
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const std::runtime_error& e) {
        // Captura erros críticos (I/O, falhas no DAL/Mapper) vindos do Service.
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao salvar o Aluno."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        // Captura qualquer outra exceção inesperada.
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a criação."
             << endl;
    }
}

void ConsoleUI::criar_professor() const {
    string nome, email, senha, disciplina;

    cout << "\n--- Criar Novo Professor ---" << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nome: ";
    getline(cin, nome);
    cout << "E-mail: ";
    getline(cin, email);
    cout << "Senha: ";
    getline(cin, senha);
    cout << "Disciplina: ";
    getline(cin, disciplina);
    // Não precisa limpar o buffer, pois a última leitura foi da linha

    try {
        Professor novo_professor =
            professorController.create(nome, email, senha, disciplina);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Professor criado com ID: "
             << novo_professor.getId() << endl;
        cout << "    Nome: " << novo_professor.getNome() << endl;
        cout << "    Disciplina: " << disciplina << endl;
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

void ConsoleUI::realizar_login() const {
    int opcao = -1;

    do {
        imprimir_opcoes_login();

        if (!(cin >> opcao)) {
            cout << "\n>> Entrada inválida. Por favor, digite um número."
                 << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcao) {
            case 0:
                login_aluno();
                break;

            case 1:
                login_professor();
                break;

            default:
                cout << "\n>> Opcao invalida! Tente novamente" << endl;
                break;
        }
    } while (opcao < 0 && opcao > 1);
}

void ConsoleUI::login_aluno() const {
    string email, senha;

    cout << "\n--- Entrar Aluno ---" << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "E-mail: ";
    getline(cin, email);
    cout << "Senha: ";
    getline(cin, senha);

    try {
        Aluno aluno = loginController.loginAluno(email, senha);

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

void ConsoleUI::login_professor() const {
    string email, senha;

    cout << "\n--- Entrar Professor ---" << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "E-mail: ";
    getline(cin, email);
    cout << "Senha: ";
    getline(cin, senha);

    try {
        Professor professor = loginController.loginProfessor(email, senha);

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

void ConsoleUI::realizar_logout() const {
    if (!sessionManager.isLogged())
        return;

    int opcao = -1;

    do {
        imprimir_opcoes_logout();

        if (!(cin >> opcao)) {
            cout << "\n>> Entrada inválida. Por favor, digite um número."
                 << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcao) {
            case 0:
                cout << "\n>> Deslogando..." << endl;
                sessionManager.logout();
                break;

            case 1:
                break;

            default:
                cout << "\n>> Opcao invalida! Tente novamente" << endl;
                break;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (opcao < 0 && opcao > 1);
}

void ConsoleUI::loop_login(int& opcao) const {
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
                criar_aluno();
                break;

            case 2:
                criar_professor();
                break;

            case 3:
                realizar_login();
                break;

            default:
                cout << "\n>> Opcao invalida! Tente novamente" << endl;
                break;
        }

        if (opcao != 0) {
            cout << "\nPressione Enter para continuar...";
            cin.get();
        }
    } while (opcao != 0 && !sessionManager.isLogged());
}

void ConsoleUI::loop_aluno(int& opcao) const {
    do {
        desenhar_relogio();
        imprimir_opcoes_aluno();

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
                realizar_logout();
                break;

            default:
                cout << "\n>> Opcao invalida! Tente novamente" << endl;
                break;
        }

        if (opcao != 0) {
            cout << "\nPressione Enter para continuar...";
            cin.get();
        }
    } while (opcao != 0 && sessionManager.isAluno());
}

void ConsoleUI::loop_professor(int& opcao) const {
    do {
        desenhar_relogio();
        imprimir_opcoes_professor();

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
                realizar_logout();
                break;

            default:
                cout << "\n>> Opcao invalida! Tente novamente" << endl;
                break;
        }

        if (opcao != 0) {
            cout << "\nPressione Enter para continuar...";
            cin.get();
        }
    } while (opcao != 0 && sessionManager.isProfessor());
}

void ConsoleUI::start() const {
    int opcao = -1;

    do {
        if (!sessionManager.isLogged())
            loop_login(opcao);
        else if (sessionManager.isProfessor())
            loop_professor(opcao);
        else
            loop_aluno(opcao);
    } while (opcao != 0);
}