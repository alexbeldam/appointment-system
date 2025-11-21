#include "view/alunoUI.hpp"

#include <iostream>

#include "util/utils.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::vector;

static void imprimir_menu();
static void imprimir_confirmacao();

AlunoUI::AlunoUI(AlunoController& ac, ProfessorController& pc,
                 AgendamentoController& agc,
                 const shared_ptr<SessionService>& ss)
    : ConsoleUI(ss),
      alunoController(ac),
      professorController(pc),
      agendamentoController(agc) {}

void AlunoUI::agendar_horario() {
    cout << "\n--- Professores ---" << endl;

    vector<shared_ptr<Professor>> professores;

    try {
        professores = this->professorController.list();
        if (professores.empty()) {
            cout << "\n>> Nenhum professor cadastrado no sistema." << endl;
            return;
        }

        for (size_t i = 0; i < professores.size(); i++) {
            const auto& prof = professores[i];

            cout << '#' << (i + 1) << " | Nome: " << prof->getNome()
                 << " | Disciplina: " << prof->getDisciplina() << endl;
        }
    } catch (const exception& e) {
        cout << "\n>> ERRO ao listar professores: " << e.what() << endl;
        return;
    }

    size_t profIdx = read_integer_range(
        "Escolha um professor (0 para cancelar): ", 0, professores.size());

    if (profIdx == 0) {
        cout << "\n>> Agendamento cancelado." << endl;
        return;
    }

    const auto& prof = professores[profIdx - 1];
    const auto& horarios = prof->getHorariosDisponiveis();

    cout << "\n--- Horários Disponíveis ---" << endl;

    if (horarios.empty()) {
        cout << "\n>> Nenhum horário disponível para este professor." << endl;
        return;
    }

    for (size_t i = 0; i < horarios.size(); i++) {
        const auto& h = horarios[i];
        cout << '#' << (i + 1) << " | Início: " << h->getInicioStr()
             << " | Fim: " << h->getFimStr() << endl;
    }

    size_t horarioIdx = read_integer_range(
        "Escolha um horário para agendar (0 para cancelar): ", 0,
        horarios.size());

    if (horarioIdx == 0) {
        cout << "\n>> Agendamento cancelado." << endl;
        return;
    }

    const auto& horario = horarios[horarioIdx - 1];

    try {
        long alunoId = sessionService->getAluno()->getId();
        long horarioId = horario->getId();

        agendamentoController.agendarHorario(alunoId, horarioId);

        cout << "\n================================================"
                "=="
             << endl;
        cout << "✅ SUCESSO! Agendamento enviado:" << endl;
        cout << "   Professor: " << prof->getNome() << endl;
        cout << "   Início: " << horario->getInicioStr() << endl;
        cout << "   Fim: " << horario->getFimStr() << endl;
        cout << "================================================"
                "=="
             << endl;
    } catch (const exception& e) {
        cout << "\n>> ERRO ao agendar horário: " << e.what() << endl;
    }
}

void AlunoUI::atualizar_perfil() {
    try {
        const auto& current = sessionService->getAluno();
        long alunoId = current->getId();

        cout << "\n--- Atualizar Perfil ---" << endl;
        cout << "Deixe em branco para manter o valor atual." << endl;

        cout << "Nome atual: " << current->getNome() << endl;
        cout << "Novo nome: ";
        string novoNome;
        getline(cin, novoNome);

        cout << "Email atual: " << current->getEmail() << endl;
        cout << "Novo email: ";
        string novoEmail;
        getline(cin, novoEmail);

        cout << "Matrícula atual: " << current->getMatricula() << endl;
        cout << "Nova matrícula (0 para manter): ";
        string matriculaInput;
        getline(cin, matriculaInput);
        long novaMatricula = 0;
        if (!matriculaInput.empty()) {
            try {
                novaMatricula = stol(matriculaInput);
            } catch (...) {
                cout << "Entrada de matrícula inválida. Mantendo a antiga."
                     << endl;
                novaMatricula = 0;
            }
        }

        cout << "Nova senha: ";
        string novaSenha;
        getline(cin, novaSenha);

        auto updated = alunoController.update(alunoId, novoNome, novoEmail,
                                              novaSenha, novaMatricula);

        cout << "\n=========================================" << endl;
        cout << "✅ SUCESSO! Perfil atualizado:" << endl;
        cout << "   Nome: " << updated->getNome() << endl;
        cout << "   Email: " << updated->getEmail() << endl;
        cout << "   Matrícula: " << updated->getMatricula() << endl;
        cout << "=========================================" << endl;
    } catch (const exception& e) {
        cout << "\n>> ERRO ao atualizar perfil: " << e.what() << endl;
    }
}

void AlunoUI::visualizar_agendamentos() {
    const auto& aluno = sessionService->getAluno();
    auto& agendamentos = aluno->getAgendamentos();

    cout << "\n--- Meus Agendamentos ---" << endl;

    if (agendamentos.empty()) {
        cout << "\n>> Nenhum agendamento cadastrado." << endl;
        return;
    }

    for (const auto& a : agendamentos) {
        try {
            auto horario = a->getHorario();
            auto professor = horario->getProfessor();

            cout << "Professor: " << professor->getNome()
                 << " | Início: " << horario->getInicioStr()
                 << " | Fim: " << horario->getFimStr()
                 << " | Status: " << a->getStatusStr() << endl;
        } catch (const invalid_argument& e) {
            cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
            cout << ">> Tente novamente com dados válidos." << endl;
        } catch (const runtime_error& e) {
            cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao resgatar o horário."
                 << endl;
            cout << ">> Detalhes do Erro: " << e.what() << endl;
        } catch (...) {
            cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada "
                    "durante "
                    "a listagem."
                 << endl;
        }
    }
}

void AlunoUI::cancelar_agendamento() {
    const auto& aluno = sessionService->getAluno();
    auto cancelaveis = aluno->getAgendamentosCancelaveis();

    if (cancelaveis.empty()) {
        cout << "\n>> Você não tem agendamentos para cancelar." << endl;
        return;
    }

    cout << "\n--- Cancelar Agendamento ---" << endl;
    for (size_t i = 0; i < cancelaveis.size(); ++i) {
        const auto& a = cancelaveis[i];
        auto horario = a->getHorario();
        auto professor = horario->getProfessor();

        cout << '#' << (i + 1) << " | Professor: " << professor->getNome()
             << " | Início: " << horario->getInicioStr()
             << " | Fim: " << horario->getFimStr()
             << " | Status: " << a->getStatusStr() << endl;
    }
    size_t escolha = read_integer_range(
        "Escolha um agendamento (0 para sair): ", 0, cancelaveis.size());
    if (escolha == 0) {
        cout << "\n>> Operação cancelada." << endl;
        return;
    }
    const auto& selecionado = cancelaveis[escolha - 1];
    try {
        agendamentoController.cancelar(selecionado->getId());
        cout << "\n====================================" << endl;
        cout << "✅ SUCESSO! Agendamento cancelado!" << endl;
        cout << "====================================" << endl;
    } catch (const invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao cancelar agendamento."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada "
                "durante o cancelamento."
             << endl;
    }
}

void AlunoUI::deletar_perfil() {
    cout << "\n--- Deletar perfil ---" << endl;

    imprimir_confirmacao();
    int confirm = read_integer_range("Escolha uma opcao: ", 0, 1);

    if (confirm == 0) {
        cout << "\n>> Exclusão cancelada." << endl;
        return;
    }

    const auto& aluno = sessionService->getAluno();

    try {
        alunoController.destroy(aluno->getId());

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Perfil deletado!" << endl;
        cout << "==================================================" << endl;
    } catch (const invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao deletar aluno." << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a exclusão."
             << endl;
    }
}

bool AlunoUI::show() {
    int opcao = -1;

    while (sessionService->isAluno()) {
        desenhar_relogio();
        imprimir_menu();

        opcao = read_integer_range("Escolha uma opcao: ", 0, 6);

        switch (opcao) {
            case 0:
                return false;
            case 1:
                agendar_horario();
                break;
            case 2:
                visualizar_agendamentos();
                break;
            case 3:
                cancelar_agendamento();
                break;
            case 4:
                atualizar_perfil();
                break;
            case 5:
                deletar_perfil();
                break;
            case 6:
                fazer_logout();
                break;
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }

    return true;
}

void imprimir_menu() {
    cout << "MENU ALUNO:" << endl;
    cout << "1 - Agendar Horário" << endl;
    cout << "2 - Listar meus agendamentos" << endl;
    cout << "3 - Cancelar agendamento" << endl;
    cout << "4 - Atualizar Perfil" << endl;
    cout << "5 - Deletar perfil" << endl;
    cout << "6 - Logout" << endl;
    cout << "0 - Sair do programa" << endl;
}

void imprimir_confirmacao() {
    cout << "Essa operação é irreversível. ";
    cout << "Tem certeza que deseja continuar?" << endl;
    cout << "0 - Não" << endl;
    cout << "1 - Sim" << endl;
}