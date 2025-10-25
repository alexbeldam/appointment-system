#include "consoleUI.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

#include "util/utils.hpp"

using namespace std;

ConsoleUI::ConsoleUI(const AlunoController& ac, const ProfessorController& pc,
                     const LoginController& lc, const HorarioController& hc,
                     SessionManager& sm, const AgendamentoController& agc)
    : alunoController(ac),
      professorController(pc),
      loginController(lc),
      horarioController(hc),
      sessionManager(sm),
      agendamentoController(agc) {}

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

void imprimir_login() {
    cout << "MENU PRINCIPAL:" << endl;
    cout << "1 - Criar Novo Aluno (Cadastro)" << endl;
    cout << "2 - Criar Novo Professor (Cadastro)" << endl;
    cout << "3 - Fazer Login" << endl;
    cout << "0 - Sair do programa" << endl;
}

void imprimir_opcoes_login() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "0 - Entrar como aluno" << endl;
    cout << "1 - Entrar como professor" << endl;
}

void imprimir_opcoes_aluno() {
    cout << "MENU ALUNO:" << endl;
    cout << "1 - Logout" << endl;
    cout << "2 - Agendar Horário" << endl;
    cout << "0 - Sair do programa" << endl;
}

void imprimir_opcoes_professor() {
    cout << "MENU PROFESSOR:" << endl;
    cout << "1 - Logout" << endl;
    cout << "2 - Cadastrar horário disponível" << endl;
    cout << "3 - Listar meus horários" << endl;
    cout << "4 - Excluir todos meus horários" << endl;
    cout << "5 - Excluir horário por ID" << endl;
    cout << "0 - Sair do programa" << endl;
}

void imprimir_opcoes_logout() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "1 - Voltar" << endl;
    cout << "0 - Deslogar" << endl;
}

void ConsoleUI::criar_aluno() const {
    string nome, email, senha;
    long matricula;

    cout << "\n--- Criar Novo Aluno ---" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
        Aluno novo_aluno =
            alunoController.create(nome, email, senha, matricula);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Aluno criado com ID: " << novo_aluno.getId()
             << endl;
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

    try {
        Professor novo_professor =
            professorController.create(nome, email, senha, disciplina);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Professor criado com ID: "
             << novo_professor.getId() << endl;
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

void ConsoleUI::realizar_login() const {
    imprimir_opcoes_login();

    int opcao = read_integer_range("Escolha uma opcao: ", 0, 1);

    switch (opcao) {
        case 0:
            login_aluno();
            break;
        case 1:
            login_professor();
    }
}

void ConsoleUI::login_aluno() const {
    string email, senha;

    cout << "\n--- Entrar Aluno ---" << endl;

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

    imprimir_opcoes_logout();

    int opcao = read_integer_range("Escolha uma opcao: ", 0, 1);

    if (opcao == 0) {
        sessionManager.logout();
        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Logout realizado." << endl;
        cout << "==================================================" << endl;
        return;
    }
}

void ConsoleUI::handleAgendarHorario() const {
    cout << "\n--- Professores ---" << endl;

    vector<Professor> professores;

    try {
        professores = this->professorController.list();
        if (professores.empty()) {
            cout << ">> Nenhum professor cadastrado no sistema." << endl;
            return;
        }

        for (size_t i = 0; i < professores.size(); i++) {
            const auto& prof = professores[i];

            cout << '#' << (i + 1) << " | Nome: " << prof.getNome()
                 << " | Disciplina: " << prof.getDisciplina() << endl;
        }
    } catch (const std::exception& e) {
        cout << "\n>> ERRO ao listar professores: " << e.what() << endl;
        return;
    }

    size_t profIdx =
        read_integer_range("Escolha um professor: ", 1, professores.size());

    const auto& prof = professores[profIdx - 1];
    const auto& horarios = prof.getHorariosDisponiveis();

    cout << "\n--- Horários Disponíveis ---" << endl;

    if (horarios.empty()) {
        cout << ">> Nenhum horário disponível para este professor." << endl;
        return;
    }

    for (size_t i = 0; i < horarios.size(); i++) {
        const auto& h = horarios[i];
        cout << '#' << (i + 1) << " | Início: " << h.getInicio()
             << " | Fim: " << h.getFim() << endl;
    }

    size_t horarioIdx = read_integer_range(
        "Escolha um horário para agendar: ", 1, horarios.size());

    const auto& horario = horarios[horarioIdx - 1];

    try {
        long alunoId = sessionManager.getCurrentAluno().getId();
        long horarioId = horario.getId();

        agendamentoController.agendarHorario(alunoId, horarioId);

        cout << "\n================================================"
                "=="
             << endl;
        cout << "✅ SUCESSO! Solicitação de agendamento enviada." << endl;
        cout << "==================================================" << endl;

    } catch (const std::invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Falha ao agendar." << endl;
    }
}

void ConsoleUI::loop_login(int& opcao) const {
    while (!sessionManager.isLogged()) {
        desenhar_relogio();
        imprimir_login();

        opcao = read_integer_range("Escolha uma opcao: ", 0, 3);

        if (opcao == 0)
            return;

        switch (opcao) {
            case 1:
                criar_aluno();
                break;
            case 2:
                criar_professor();
                break;
            case 3:
                realizar_login();
                break;
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }
}

void ConsoleUI::loop_aluno(int& opcao) const {
    while (sessionManager.isAluno()) {
        desenhar_relogio();
        imprimir_opcoes_aluno();

        opcao = read_integer_range("Escolha uma opcao: ", 0, 2);

        if (opcao == 0)
            return;

        switch (opcao) {
            case 1:
                realizar_logout();
                break;
            case 2:
                handleAgendarHorario();
                break;
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }
}

void ConsoleUI::loop_professor(int& opcao) const {
    while (sessionManager.isProfessor()) {
        desenhar_relogio();
        imprimir_opcoes_professor();

        opcao = read_integer_range("Escolha uma opcao: ", 0, 5);

        if (opcao == 0)
            return;

        switch (opcao) {
            case 1:
                realizar_logout();
                break;

            case 2: {
                string inicio, fim;
                cout << "\nData e hora de início (ex: 1/11 08:30): ";
                getline(cin, inicio);
                cout << "Data e hora de término (ex: 1/11 09:00): ";
                getline(cin, fim);

                try {
                    auto prof = sessionManager.getCurrentProfessor();
                    auto horario = horarioController.cadastrarHorario(
                        prof.getId(), inicio, fim);
                    cout << "\n✅ Horário cadastrado com sucesso! ID: "
                         << horario.getId() << endl;
                } catch (const std::invalid_argument& e) {
                    cout << "\n⚠️  " << e.what() << endl;
                } catch (const std::exception& e) {
                    cout << "\n Erro inesperado: " << e.what() << endl;
                }
                break;
            }

            case 3: {  // Listar horários
                auto prof = sessionManager.getCurrentProfessor();
                auto horarios = prof.getHorarios();

                if (horarios.empty()) {
                    cout << "\n⚠️  Nenhum horário cadastrado." << endl;
                } else {
                    cout << "\n--- Horários cadastrados ---" << endl;
                    for (const auto& h : horarios) {
                        cout << "ID: " << h.getId()
                             << " | Início: " << h.getInicio()
                             << " | Fim: " << h.getFim() << " | Disponível: "
                             << (h.isDisponivel() ? "Sim" : "Não") << endl;
                    }
                }
                break;
            }

            case 4: {  // Excluir todos os horários
                auto prof = sessionManager.getCurrentProfessor();
                char confirm;
                cout << "\n⚠️  Deseja realmente excluir todos os horários? "
                        "(s/n): ";
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (confirm == 's' || confirm == 'S') {
                    bool ok = horarioController.excluirTodosPorProfessor(
                        prof.getId());
                    if (ok)
                        cout << "\n   Todos os horários foram removidos!"
                             << endl;
                    else
                        cout << "\n⚠️  Nenhum horário para remover." << endl;
                } else {
                    cout << "\nAção cancelada." << endl;
                }
                break;
            }

            case 5: {  // Excluir horário por ID
                long idHorario;
                cout << "\nDigite o ID do horário a ser excluído: ";
                cin >> idHorario;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                char confirm;
                cout << "Confirmar exclusão do horário " << idHorario
                     << "? (s/n): ";
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (confirm == 's' || confirm == 'S') {
                    bool ok = horarioController.excluirPorId(idHorario);
                    if (ok)
                        cout << "\n   Horário excluído com sucesso!" << endl;
                    else
                        cout << "\n⚠️  Horário não encontrado." << endl;
                } else {
                    cout << "\nAção cancelada." << endl;
                }
                break;
            }
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }
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

    cout << "\n>> Saindo do programa" << endl;
}