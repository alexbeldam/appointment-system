#include "view/professorUI.hpp"

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

static void imprimir_menu();
static void imprimir_opcoes();
static void imprimir_confirmacao();

ProfessorUI::ProfessorUI(ProfessorController& pc, HorarioController& hc,
                         AgendamentoController& ac,
                         const shared_ptr<SessionService>& ss)
    : ConsoleUI(ss),
      professorController(pc),
      horarioController(hc),
      agendamentoController(ac) {}

void ProfessorUI::cadastro_horario() {
    string inicioStr, fimStr;

    cout << "\n--- Cadastrar Novo Horário ---" << endl;

    cout << "Data e hora de início (ex: 1/11 08:30): ";
    getline(cin, inicioStr);

    Timestamp inicio = string_to_timestamp(inicioStr);

    if (inicio == -1) {
        cout << "\n>> ERRO DE VALIDAÇÃO: Data e hora de início inválidas."
             << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
        return;
    }

    cout << "Data e hora de término (ex: 1/11 09:00): ";
    getline(cin, fimStr);

    Timestamp fim = string_to_timestamp(fimStr);

    if (fim == -1) {
        cout << "\n>> ERRO DE VALIDAÇÃO: Data e hora de término inválidas."
             << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
        return;
    }

    try {
        const auto& prof = sessionService->getProfessor();
        const auto& horario =
            horarioController.cadastrarHorario(prof->getId(), inicio, fim);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Horário cadastrado: " << endl;
        cout << "   Início: " << horario->getInicioStr() << endl;
        cout << "   Fim: " << horario->getFimStr() << endl;
        cout << "==================================================" << endl;
    } catch (const invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao salvar o horário."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "o cadastro."
             << endl;
    }
}

void ProfessorUI::listar_horarios() {
    cout << "\n--- Meus Horários ---" << endl;

    try {
        const auto& prof = sessionService->getProfessor();
        auto& horarios = prof->getHorarios();

        if (horarios.empty()) {
            cout << "\n>> Nenhum horário cadastrado." << endl;
            return;
        }

        for (const auto& h : horarios) {
            cout << "Início: " << h->getInicioStr()
                 << " | Fim: " << h->getFimStr()
                 << " | Disponível: " << (h->isDisponivel() ? "Sim" : "Não")
                 << endl;
        }
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao listar horários."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a listagem."
             << endl;
    }
}

void ProfessorUI::excluir_horario() {
    cout << "\n--- Excluir Horário ---" << endl;

    const auto& prof = sessionService->getProfessor();
    auto& horarios = prof->getHorarios();

    if (horarios.empty()) {
        cout << "\n>> Nenhum horário para remover." << endl;
        return;
    }

    for (size_t i = 0; i < horarios.size(); i++) {
        const auto& h = horarios[i];
        cout << '#' << (i + 1) << " | Início: " << h->getInicioStr()
             << " | Fim: " << h->getFimStr() << endl;
    }

    size_t horarioIdx = read_integer_range(
        "Escolha um horário para deletar (0 para cancelar): ", 0,
        horarios.size());

    if (horarioIdx == 0) {
        cout << "\n>> Exclusão cancelada." << endl;
        return;
    }

    cout << endl;
    imprimir_confirmacao();
    int confirm = read_integer_range("Escolha uma opcao: ", 0, 1);

    if (confirm == 0) {
        cout << "\n>> Exclusão cancelada." << endl;
        return;
    }

    const auto& horario = horarios[horarioIdx - 1];

    try {
        horarioController.excluirPorId(horario->getId());

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Horário removido!" << endl;
        cout << "==================================================" << endl;
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao excluir o horário."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a exclusão."
             << endl;
    }
}

void ProfessorUI::excluir_todos_horarios() {
    cout << "\n--- Excluir Todos os Meus Horários ---" << endl;

    try {
        const auto& prof = sessionService->getProfessor();
        auto& horarios = prof->getHorarios();

        if (horarios.empty()) {
            cout << "\n>> Nenhum horário para remover." << endl;
            return;
        }

        imprimir_confirmacao();
        int confirm = read_integer_range("Escolha uma opcao: ", 0, 1);

        if (confirm == 0) {
            cout << "\n>> Exclusão cancelada." << endl;
            return;
        }

        horarioController.excluirTodosPorProfessor(prof->getId());

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Todos os horários foram removidos!" << endl;
        cout << "==================================================" << endl;
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao excluir os horários."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a exclusão."
             << endl;
    }
}

void ProfessorUI::atualizar_perfil() {
    try {
        const auto& current = sessionService->getProfessor();
        long professorId = current->getId();

        cout << "\n--- Atualizar Perfil (Professor) ---" << endl;
        cout << "Deixe em branco para manter o valor atual." << endl;

        cout << "Nome atual: " << current->getNome() << endl;
        cout << "Novo nome: ";
        string novoNome;
        getline(cin, novoNome);

        cout << "Email atual: " << current->getEmail() << endl;
        cout << "Novo email: ";
        string novoEmail;
        getline(cin, novoEmail);

        cout << "Disciplina atual: " << current->getDisciplina() << endl;
        cout << "Nova disciplina: ";
        string novaDisciplina;
        getline(cin, novaDisciplina);

        cout << "Nova senha: ";
        string novaSenha;
        getline(cin, novaSenha);

        const auto& updated = professorController.update(
            professorId, novoNome, novoEmail, novaSenha, novaDisciplina);

        cout << "\n=========================================" << endl;
        cout << "✅ SUCESSO! Perfil atualizado:" << endl;
        cout << "   Nome: " << updated->getNome() << endl;
        cout << "   Email: " << updated->getEmail() << endl;
        cout << "   Disciplina: " << updated->getDisciplina() << endl;
        cout << "=========================================" << endl;
    } catch (const exception& e) {
        cout << "\n>> ERRO ao atualizar perfil: " << e.what() << endl;
    }
}

void ProfessorUI::deletar_perfil() {
    cout << "\n--- Deletar perfil ---" << endl;

    imprimir_confirmacao();
    int confirm = read_integer_range("Escolha uma opcao: ", 0, 1);

    if (confirm == 0) {
        cout << "\n>> Exclusão cancelada." << endl;
        return;
    }

    const auto& professor = sessionService->getProfessor();

    try {
        professorController.destroy(professor->getId());

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Perfil deletado!" << endl;
        cout << "==================================================" << endl;
    } catch (const invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao deletar professor."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a exclusão."
             << endl;
    }
}

void ProfessorUI::fazer_avaliacoes() {
    cout << "\n--- Gerenciar Agendamentos Pendentes ---" << endl;

    bool keepRunning = true;

    while (keepRunning) {
        keepRunning = avaliar_agendamentos();
    }
}

bool ProfessorUI::avaliar_agendamentos() {
    const auto& professor = sessionService->getProfessor();
    auto horarios = professor->getHorariosDisponiveis();

    Horario::AgendamentoVector agendamentos;

    for (auto& horario : horarios) {
        auto pendentes = horario->getAgendamentosPendentes();

        agendamentos.insert(agendamentos.begin(), pendentes.begin(),
                            pendentes.end());
    }

    if (agendamentos.empty()) {
        cout << "\n>> Nenhum agendamento pendente." << endl;
        return false;
    }

    cout << "\nAgendamentos aguardando sua resposta:" << endl;
    for (size_t i = 0; i < agendamentos.size(); i++) {
        const auto& agt = agendamentos[i];
        auto aluno = agt->getAluno();
        auto horario = agt->getHorario();

        cout << '#' << (i + 1) << " | Aluno: " << aluno->getNome()
             << " | Início: " << horario->getInicioStr()
             << " | Fim: " << horario->getFimStr() << endl;
    }
    size_t agtIdx = read_integer_range(
        "\nEscolha um agendamento para confirmar ou recusar (0 para "
        "voltar): ",
        0, agendamentos.size());

    if (agtIdx == 0) {
        cout << "\n>> Voltando ao menu principal." << endl;
        return false;
    }
    size_t vectorIndex = agtIdx - 1;
    const auto& agendamento = agendamentos[vectorIndex];

    imprimir_opcoes();
    int escolha = read_integer_range("Escolha uma opção: ", 0, 2);

    if (escolha == 0) {
        cout << "\n>> Seleção cancelada. Voltando à lista..." << endl;
        return true;
    }
    try {
        if (escolha == 1) {
            agendamentoController.confirmar(agendamento->getId());
            cout << "\n=========================================" << endl;
            cout << "✅ SUCESSO! Agendamento CONFIRMADO." << endl;
            cout << "=========================================" << endl;

        } else if (escolha == 2) {
            agendamentoController.recusar(agendamento->getId());
            cout << "\n=========================================" << endl;
            cout << "✅ SUCESSO! Agendamento RECUSADO." << endl;
            cout << "=========================================" << endl;
        }
    } catch (const exception& e) {
        cout << "\n>> ERRO AO PROCESSAR AGENDAMENTO: " << e.what() << endl;
        cout << ">> Tente novamente." << endl;
    }

    return true;
}

void ProfessorUI::cancelar_agendamento() {
    cout << "\n--- Cancelar Agendamento ---" << endl;

    const auto& professor = sessionService->getProfessor();
    auto horarios = professor->getHorariosOcupados();

    Horario::AgendamentoVector agendamentos;

    for (const auto& horario : horarios) {
        auto confirmados = horario->getAgendamentosConfirmados();

        agendamentos.insert(agendamentos.begin(), confirmados.begin(),
                            confirmados.end());
    }

    if (agendamentos.empty()) {
        cout << "\n>> Nenhum agendamento confirmado." << endl;
        return;
    }

    for (size_t i = 0; i < agendamentos.size(); i++) {
        const auto& agt = agendamentos[i];
        auto aluno = agt->getAluno();
        auto horario = agt->getHorario();

        cout << '#' << (i + 1) << " | Aluno: " << aluno->getNome()
             << " | Início: " << horario->getInicioStr()
             << " | Fim: " << horario->getFimStr() << endl;
    }

    size_t agtIdx = read_integer_range(
        "\nEscolha um agendamento cancelar (0 para "
        "voltar): ",
        0, agendamentos.size());

    if (agtIdx == 0) {
        cout << "\n>> Voltando ao menu principal." << endl;
        return;
    }

    size_t vectorIndex = agtIdx - 1;
    const auto& agendamento = agendamentos[vectorIndex];

    try {
        agendamentoController.cancelar(agendamento->getId());

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Agendamento CANCELADO!" << endl;
        cout << "==================================================" << endl;
    } catch (const runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao cancelar agendamento."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    }
}

bool ProfessorUI::show() {
    while (sessionService->isProfessor()) {
        desenhar_relogio();
        imprimir_menu();

        int opcao = read_integer_range("Escolha uma opcao: ", 0, 9);

        switch (opcao) {
            case 0:
                return false;
            case 1:
                fazer_avaliacoes();
                break;
            case 2:
                cancelar_agendamento();
                break;
            case 3:
                cadastro_horario();
                break;
            case 4:
                listar_horarios();
                break;
            case 5:
                excluir_horario();
                break;
            case 6:
                excluir_todos_horarios();
                break;
            case 7:
                atualizar_perfil();
                break;
            case 8:
                deletar_perfil();
                break;
            case 9:
                fazer_logout();
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }

    return true;
}

void imprimir_menu() {
    cout << "MENU PROFESSOR:" << endl;
    cout << "1 - Gerenciar Agendamentos Pendentes" << endl;
    cout << "2 - Cancelar Agendamento" << endl;
    cout << "3 - Cadastrar horário disponível" << endl;
    cout << "4 - Listar meus horários" << endl;
    cout << "5 - Excluir um horário" << endl;
    cout << "6 - Excluir todos meus horários" << endl;
    cout << "7 - Atualizar Perfil" << endl;
    cout << "8 - Deletar Perfil" << endl;
    cout << "9 - Logout" << endl;
    cout << "0 - Sair do programa" << endl;
}

void imprimir_opcoes() {
    cout << "\nMENU DE OPCOES:" << endl;
    cout << "1 - Confirmar" << endl;
    cout << "2 - Recusar" << endl;
    cout << "0 - Voltar" << endl;
}

void imprimir_confirmacao() {
    cout << "Essa operação é irreversível. ";
    cout << "Tem certeza que deseja continuar?" << endl;
    cout << "0 - Não" << endl;
    cout << "1 - Sim" << endl;
}