#include "view/professorUI.hpp"

#include <iostream>
#include <stdexcept>
#include <memory>

#include "event/events.hpp"
#include "util/utils.hpp"
using namespace std;

static void imprimir_menu();
static void imprimir_confirmacao();

ProfessorUI::ProfessorUI(const ProfessorController& pc,
                         const HorarioController& hc,
                         const AgendamentoController& ac, EventBus& bus,
                         SessionManager& sm)
    : ConsoleUI(sm),
      professorController(pc),
      horarioController(hc),
      agendamentoController(ac),
      bus(bus),
      sessionManager(sm) {}

void ProfessorUI::cadastro_horario() const {
    string inicioStr, fimStr;

    cout << "\n--- Cadastrar Novo Horário ---" << endl;

    cout << "Data e hora de início (ex: 1/11 08:30): ";
    getline(cin, inicioStr);

    time_t inicio = string_to_time(inicioStr);

    if (inicio == -1) {
        cout << "\n>> ERRO DE VALIDAÇÃO: Data e hora de início inválidas."
             << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
        return;
    }

    cout << "Data e hora de término (ex: 1/11 09:00): ";
    getline(cin, fimStr);

    time_t fim = string_to_time(fimStr);

    if (fim == -1) {
        cout << "\n>> ERRO DE VALIDAÇÃO: Data e hora de término inválidas."
             << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
        return;
    }

    try {
        const auto& prof = sessionManager.getCurrentProfessor();
        const auto& horario =
            horarioController.cadastrarHorario(prof.getId(), inicio, fim);

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Horário cadastrado: " << endl;
        cout << "   Início: " << horario.getInicioStr() << endl;
        cout << "   Fim: " << horario.getFimStr() << endl;
        cout << "==================================================" << endl;
    } catch (const std::invalid_argument& e) {
        cout << "\n>> ERRO DE VALIDAÇÃO: " << e.what() << endl;
        cout << ">> Tente novamente com dados válidos." << endl;
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao salvar o horário."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "o cadastro."
             << endl;
    }
}

void ProfessorUI::listar_horarios() const {
    cout << "\n--- Meus Horários ---" << endl;

    try {
        const auto& prof = sessionManager.getCurrentProfessor();
        const auto& horarios = prof.getHorarios();

        if (horarios.empty()) {
            cout << ">> Nenhum horário cadastrado." << endl;
            return;
        }

        for (const auto& h : horarios) {
            cout << "Início: " << h.getInicioStr()
                 << " | Fim: " << h.getFimStr()
                 << " | Disponível: " << (h.isDisponivel() ? "Sim" : "Não")
                 << endl;
        }
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao listar horários."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a listagem."
             << endl;
    }
}

void ProfessorUI::excluir_horario() const {
    cout << "\n--- Excluir Horário ---" << endl;

    const auto& prof = sessionManager.getCurrentProfessor();
    const auto& horarios = prof.getHorarios();

    if (horarios.empty()) {
        cout << ">> Nenhum horário para remover." << endl;
        return;
    }

    for (size_t i = 0; i < horarios.size(); i++) {
        const auto& h = horarios[i];
        cout << '#' << (i + 1) << " | Início: " << h.getInicioStr()
             << " | Fim: " << h.getFimStr() << endl;
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
        horarioController.excluirPorId(horario.getId());

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Horário removido!" << endl;
        cout << "==================================================" << endl;
    } catch (const std::runtime_error& e) {
        cout << "\n>> ERRO INTERNO DO SISTEMA: Falha ao excluir o horário."
             << endl;
        cout << ">> Detalhes do Erro: " << e.what() << endl;
    } catch (...) {
        cout << "\n>> ERRO DESCONHECIDO: Ocorreu uma falha inesperada durante "
                "a exclusão."
             << endl;
    }
}

void ProfessorUI::excluir_todos_horarios() const {
    cout << "\n--- Excluir Todos os Meus Horários ---" << endl;

    try {
        const auto& prof = sessionManager.getCurrentProfessor();
        const auto& horarios = prof.getHorarios();

        if (horarios.empty()) {
            cout << ">> Nenhum horário para remover." << endl;
            return;
        }

        imprimir_confirmacao();
        int confirm = read_integer_range("Escolha uma opcao: ", 0, 1);

        if (confirm == 0) {
            cout << "\n>> Exclusão cancelada." << endl;
            return;
        }

        horarioController.excluirTodosPorProfessor(prof.getId());

        cout << "\n==================================================" << endl;
        cout << "✅ SUCESSO! Todos os horários foram removidos!" << endl;
        cout << "==================================================" << endl;
    } catch (const std::runtime_error& e) {
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
        Professor current = sessionManager.getCurrentProfessor();
        long professorId = current.getId();

        cout << "\n--- Atualizar Perfil (Professor) ---" << endl;
        cout << "Deixe em branco para manter o valor atual." << endl;

        // Nome
        cout << "Nome atual: " << current.getNome() << endl;
        cout << "Novo nome: ";
        string novoNome;
        std::getline(cin, novoNome);

        // Email
        cout << "Email atual: " << current.getEmail() << endl;
        cout << "Novo email: ";
        string novoEmail;
        std::getline(cin, novoEmail);

        // Senha
        cout << "Nova senha (deixe em branco para manter): ";
        string novaSenha;
        std::getline(cin, novaSenha);

        // Disciplina
        cout << "Disciplina atual: " << current.getDisciplina() << endl;
        cout << "Nova disciplina (deixe em branco para manter): ";
        string novaDisciplina;
        std::getline(cin, novaDisciplina);

        Professor updated = professorController.update(professorId, novoNome,
                                                       novoEmail, novaSenha,
                                                       novaDisciplina);

        std::shared_ptr<Usuario> user_ptr =
            std::make_shared<Professor>(updated);
        bus.publish(UsuarioUpdatedEvent(user_ptr));

        cout << "\n✅ Perfil atualizado com sucesso!" << endl;
        cout << "Nome: " << updated.getNome() << endl;
        cout << "Email: " << updated.getEmail() << endl;
        cout << "Disciplina: " << updated.getDisciplina() << endl;

    } catch (const exception& e) {
        cout << "\n>> ERRO ao atualizar perfil: " << e.what() << endl;
    }
}


bool ProfessorUI::show() const {
    while (sessionManager.isProfessor()) {
        desenhar_relogio();
        imprimir_menu();

        int opcao = read_integer_range("Escolha uma opcao: ", 0, 6);

        switch (opcao) {
            case 0:
                return false;
            case 1:
                fazer_logout();
                break;
            case 2:
                cadastro_horario();
                break;
            case 3:
                listar_horarios();
                break;
            case 4:
                excluir_todos_horarios();
                break;
            case 5:
                excluir_horario();
                break;
                case 6:
                const_cast<ProfessorUI*>(this)->atualizar_perfil();
                break;
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }

    return true;
}

void imprimir_menu() {
    cout << "MENU PROFESSOR:" << endl;
    cout << "1 - Logout" << endl;
    cout << "2 - Cadastrar horário disponível" << endl;
    cout << "3 - Listar meus horários" << endl;
    cout << "4 - Excluir todos meus horários" << endl;
    cout << "5 - Excluir um horário" << endl;
    cout << "6 - Atualizar Perfil" << endl;
    cout << "0 - Sair do programa" << endl;
}

void imprimir_confirmacao() {
    cout << "Essa operação é irreversível. ";
    cout << "Tem certeza que deseja continuar?" << endl;
    cout << "0 - Não" << endl;
    cout << "1 - Sim" << endl;
}