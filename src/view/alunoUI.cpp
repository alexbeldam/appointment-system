#include "view/alunoUI.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "event/events.hpp"
#include "util/utils.hpp"
using namespace std;

static void imprimir_menu();

AlunoUI::AlunoUI(const AlunoController& ac, const ProfessorController& pc,
                 const HorarioController& hc, const AgendamentoController& agc,
                 EventBus& bus, SessionManager& sm)
    : ConsoleUI(sm),
      alunoController(ac),
      professorController(pc),
      horarioController(hc),
      agendamentoController(agc),
      bus(bus),
      sessionManager(sm) {}

void AlunoUI::agendar_horario() const {
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
    const auto& horarios = prof.getHorariosDisponiveis();

    cout << "\n--- Horários Disponíveis ---" << endl;

    if (horarios.empty()) {
        cout << ">> Nenhum horário disponível para este professor." << endl;
        return;
    }

    for (size_t i = 0; i < horarios.size(); i++) {
        const auto& h = horarios[i];
        cout << '#' << (i + 1) << " | Início: " << h.getInicioStr()
             << " | Fim: " << h.getFimStr() << endl;
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
        long alunoId = sessionManager.getCurrentAluno().getId();
        long horarioId = horario.getId();

        agendamentoController.agendarHorario(alunoId, horarioId);

        cout << "\n================================================"
                "=="
             << endl;
        cout << "✅ SUCESSO! Agendamento enviado:" << endl;
        cout << "   Professor: " << prof.getNome() << endl;
        cout << "   Início: " << horario.getInicioStr() << endl;
        cout << "   Fim: " << horario.getFimStr() << endl;
        cout << "================================================"
                "=="
             << endl;
    } catch (const exception& e) {
        cout << "\n>> ERRO ao agendar horário: " << e.what() << endl;
    }
}

void AlunoUI::atualizar_perfil() const {
    try {
        Aluno current = sessionManager.getCurrentAluno();
        long alunoId = current.getId();

        cout << "\n--- Atualizar Perfil ---" << endl;
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

        // Matrícula
        cout << "Matrícula atual: " << current.getMatricula() << endl;
        cout << "Nova matrícula (0 para manter): ";
        string matriculaInput;
        std::getline(cin, matriculaInput);
        long novaMatricula = 0;
        if (!matriculaInput.empty()) {
            try {
                novaMatricula = std::stol(matriculaInput);
            } catch (...) {
                cout << "Entrada de matrícula inválida. Mantendo a antiga."
                     << endl;
                novaMatricula = 0;
            }
        }

        // Senha
        cout << "Nova senha: ";
        string novaSenha;
        std::getline(cin, novaSenha);

        // Chama o controller para atualizar (fará validações)
        Aluno updated = alunoController.update(alunoId, novoNome, novoEmail,
                                               novaSenha, novaMatricula);

        // Cria shared_ptr<Usuario> no heap e publica UsuarioUpdatedEvent
        std::shared_ptr<Usuario> user_ptr = std::make_shared<Aluno>(updated);
        bus.publish(UsuarioUpdatedEvent(user_ptr));

        cout << "\n✅ Perfil atualizado com sucesso!" << endl;
        cout << "Nome: " << updated.getNome() << endl;
        cout << "Email: " << updated.getEmail() << endl;
        cout << "Matrícula: " << updated.getMatricula() << endl;

    } catch (const exception& e) {
        cout << "\n>> ERRO ao atualizar perfil: " << e.what() << endl;
    }
}

bool AlunoUI::show() const {
    int opcao = -1;

    while (sessionManager.isAluno()) {
        desenhar_relogio();
        imprimir_menu();

        opcao = read_integer_range("Escolha uma opcao: ", 0, 3);

        switch (opcao) {
            case 0:
                return false;
            case 1:
                fazer_logout();
                break;
            case 2:
                agendar_horario();
                break;
            case 3:
                atualizar_perfil();
                break;
        }

        cout << "\nPressione Enter para continuar...";
        cin.get();
    }

    return true;
}

void imprimir_menu() {
    cout << "MENU ALUNO:" << endl;
    cout << "1 - Logout" << endl;
    cout << "2 - Agendar Horário" << endl;
    cout << "3 - Atualizar Perfil" << endl;
    cout << "0 - Sair do programa" << endl;
}