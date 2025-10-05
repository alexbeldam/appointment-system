#include "consoleUI.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

ConsoleUI::ConsoleUI(const AppComposer& app) : app(app) {}

void ConsoleUI::desenhar_relogio() const {
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

void ConsoleUI::imprimir_login() const {
    cout << "MENU PRINCIPAL:" << endl;
    cout << "1 - Criar Novo Aluno (Cadastro)" << endl;
    cout << "2 - Fazer Login" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}

void ConsoleUI::imprimir_opcoes() const {
    cout << "MENU DE OPCOES:" << endl;
    cout << "0 - Sair do programa" << endl;
    cout << "Escolha uma opcao: ";
}

// Função de interface de console para a criação de um novo Aluno.
void ConsoleUI::criar_aluno() const {
    const AlunoController& controller = app.getAlunoController();
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

    // --- FLUXO DE PROCESSAMENTO E TRATAMENTO DE EXCEÇÕES ---

    try {
        // Delega a criação ao Controller (que valida e persiste).
        Aluno novo_aluno = controller.create(nome, email, senha, matricula);

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

void ConsoleUI::run() const {
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
                criar_aluno();
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
}