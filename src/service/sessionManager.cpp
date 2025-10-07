#include "service/sessionManager.hpp"

#include <algorithm>
#include <stdexcept>

#include "event/events.hpp"

using namespace std;

// O Construtor é onde o SessionManager se inscreve nos eventos.
SessionManager::SessionManager(const EventBus& bus) : bus(bus) {
    // Handler: UsuarioLoggedInEvent (Guarda a posse compartilhada do usuário)
    bus.subscribe<UsuarioLoggedInEvent>(
        [this](const auto& e) { this->user = e.usuario; });

    // Handler: UpdatedEvent (Atualiza o objeto SE o ID for o mesmo E o
    // tipo for o mesmo)
    bus.subscribe<UsuarioUpdatedEvent>([this](const auto& e) {
        if (this->isLogged() && this->user->getId() == e.usuario->getId()) {
            // VERIFICAÇÃO CRÍTICA: Checa se a subclasse do objeto logado é a
            // mesma do objeto de evento. typeid(*ptr) retorna o tipo dinâmico
            // do objeto.
            if (typeid(*this->user) == typeid(*e.usuario)) {
                this->user = e.usuario;  // Atualização segura
            }
            // Se os tipos forem diferentes (Ex: logado como Aluno, evento como
            // Professor), a atualização é ignorada.
        }
    });

    // --- Eventos Específicos do Professor ---

    // Handler: ProfessorDeletedEvent (Faz logout se o usuário logado for
    // deletado)
    bus.subscribe<ProfessorDeletedEvent>([this](const auto& e) {
        if (this->isProfessor() && this->user->getId() == e.id) {
            this->logout();
        }
    });

    // Handler: HorarioCreatedEvent (Adiciona Horário, verificando se o usuário
    // é Professor e o ID corresponde)
    bus.subscribe<HorarioCreatedEvent>([this](const auto& e) {
        if (this->isProfessor()) {
            if (Professor* p = dynamic_cast<Professor*>(this->user.get())) {
                // Checa se o horário pertence ao professor logado
                if (e.horario.getProfessorId() == p->getId()) {
                    p->addHorario(e.horario);
                }
            }
        }
    });

    // Handler: HorarioUpdatedEvent (Atualiza Horário na lista do Professor)
    bus.subscribe<HorarioUpdatedEvent>([this](const auto& e) {
        if (this->isProfessor()) {
            if (Professor* p = dynamic_cast<Professor*>(this->user.get())) {
                p->updateHorario(e.horario);
            }
        }
    });

    // Handler: HorarioDeletedEvent (Remove Horário da lista do Professor)
    bus.subscribe<HorarioDeletedEvent>([this](const auto& e) {
        if (this->isProfessor()) {
            if (Professor* p = dynamic_cast<Professor*>(this->user.get())) {
                p->removeHorario(e.id);
            }
        }
    });

    // --- Eventos Específicos do Aluno ---

    // Handler: AlunoDeletedEvent (Faz logout se o usuário logado for
    // deletado)
    bus.subscribe<AlunoDeletedEvent>([this](const auto& e) {
        if (this->isAluno() && this->user->getId() == e.id) {
            this->logout();
        }
    });

    // Handler: AgendamentoCreatedEvent (Adiciona Agendamento, verificando se o
    // usuário é Aluno e o ID corresponde)
    bus.subscribe<AgendamentoCreatedEvent>([this](const auto& e) {
        if (this->isAluno()) {
            if (Aluno* a = dynamic_cast<Aluno*>(this->user.get())) {
                // Checa se o agendamento pertence ao aluno logado
                if (e.agendamento.getAlunoId() == a->getId()) {
                    a->addAgendamento(e.agendamento);
                }
            }
        }
    });

    // Handler: AgendamentoUpdatedEvent (Atualiza Agendamento na lista do Aluno)
    bus.subscribe<AgendamentoUpdatedEvent>([this](const auto& e) {
        if (this->isAluno()) {
            if (Aluno* a = dynamic_cast<Aluno*>(this->user.get())) {
                a->updateAgendamento(e.agendamento);
            }
        }
    });

    // Handler: AgendamentoDeletedEvent (Remove Agendamento da lista do Aluno)
    bus.subscribe<AgendamentoDeletedEvent>([this](const auto& e) {
        if (this->isAluno()) {
            if (Aluno* a = dynamic_cast<Aluno*>(this->user.get())) {
                a->removeAgendamento(e.id);
            }
        }
    });
}

// --- Métodos de Gerenciamento de Sessão ---

void SessionManager::logout() {
    // Reseta o shared_ptr, encerrando a sessão.
    user.reset();
}

// --- Métodos de Consulta ---

bool SessionManager::isLogged() const {
    // Verifica se o ponteiro interno é válido.
    return (user != nullptr);
}

bool SessionManager::isAluno() const {
    if (!isLogged())
        return false;
    // Downcast seguro para verificar o tipo em runtime.
    return (dynamic_cast<const Aluno*>(user.get()) != nullptr);
}

bool SessionManager::isProfessor() const {
    if (!isLogged())
        return false;
    // Downcast seguro para verificar o tipo em runtime.
    return (dynamic_cast<const Professor*>(user.get()) != nullptr);
}

Professor SessionManager::getCurrentProfessor() const {
    if (!isLogged()) {
        throw runtime_error("Não há usuário logado na sessão.");
    }
    // Downcast para Professor e retorna uma CÓPIA completa.
    if (const Professor* professor_ptr =
            dynamic_cast<const Professor*>(user.get())) {
        return *professor_ptr;
    }
    throw runtime_error("O usuário logado não é um Professor.");
}

Aluno SessionManager::getCurrentAluno() const {
    if (!isLogged()) {
        throw runtime_error("Não há usuário logado na sessão.");
    }
    // Downcast para Aluno e retorna uma CÓPIA completa.
    if (const Aluno* aluno_ptr = dynamic_cast<const Aluno*>(user.get())) {
        return *aluno_ptr;
    }
    throw runtime_error("O usuário logado não é um Aluno.");
}