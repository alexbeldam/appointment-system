#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <memory>
#include <string>

#include "model/agendamento.hpp"
#include "model/aluno.hpp"
#include "model/horario.hpp"
#include "model/professor.hpp"

/**
 * @brief Template base para todas as estruturas de eventos.
 *
 * Utiliza o padrão CRTP (Curiously Recurring Template Pattern) para fornecer
 * funcionalidades base (como `name()`) à classe derivada.
 *
 * @tparam Derived O tipo da classe de evento concreta que herda de `Event`.
 */
template <typename Derrived>
struct Event {
    /**
     * @brief Retorna o nome do tipo de evento.
     * @return std::string O nome do evento.
     */
    std::string name() const {
        return typeid(Derrived).name();
    }
};

/**
 * @def DEFINE_EVENT
 * @brief Macro para definir uma nova estrutura de evento que herda de
 * `Event<>`.
 *
 * O formato de uso é: `DEFINE_EVENT(NomeDoEvento, Membro1 Tipo1, Membro2 Tipo2,
 * ...)`
 *
 * @param EventName O nome da nova estrutura de evento.
 * @param ... A lista de membros (para a declaração) e parâmetros (para o
 * construtor).
 */
#define DEFINE_EVENT(EventName, MemberTypeAndName, MemberName)                 \
    struct EventName : public Event<EventName> {                               \
        MemberTypeAndName; /* Ex: std::shared_ptr<Usuario> usuario; */         \
        /* Construtor que usa a lista de inicialização para evitar nullptr. */ \
        EventName(MemberTypeAndName) : MemberName(MemberName) {}               \
    };

// --- DEFINIÇÕES CONSOLIDADAS DE EVENTOS ---

// Eventos de Usuário
DEFINE_EVENT(UsuarioLoggedInEvent, std::shared_ptr<Usuario> usuario, usuario)
DEFINE_EVENT(UsuarioUpdatedEvent, std::shared_ptr<Usuario> usuario, usuario)
DEFINE_EVENT(AlunoDeletedEvent, long id, id)
DEFINE_EVENT(ProfessorDeletedEvent, long id, id)

// Eventos de Horário
DEFINE_EVENT(HorarioCreatedEvent, Horario horario, horario)
DEFINE_EVENT(HorarioUpdatedEvent, Horario horario, horario)
DEFINE_EVENT(HorarioDeletedEvent, long id, id)

// Eventos de Agendamento
DEFINE_EVENT(AgendamentoCreatedEvent, Agendamento agendamento, agendamento)
DEFINE_EVENT(AgendamentoUpdatedEvent, Agendamento agendamento, agendamento)
DEFINE_EVENT(AgendamentoDeletedEvent, long id, id)

#endif