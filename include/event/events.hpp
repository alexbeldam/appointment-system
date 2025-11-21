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

// --- EVENTOS DE GESTÃO DE SESSÃO (Login e Reset) ---

DEFINE_EVENT(ProfessorLoggedInEvent, long professorId, professorId)
DEFINE_EVENT(AlunoLoggedInEvent, long alunoId, alunoId)

// --- EVENTOS DE DELEÇÃO EM CASCATA ---

DEFINE_EVENT(ProfessorDeletedEvent, long professorId, professorId)
DEFINE_EVENT(AlunoDeletedEvent, long alunoId, alunoId)

// --- EVENTOS DE MUDANÇA DE ESTADO ---

DEFINE_EVENT(HorarioOcupadoEvent, long horarioId, horarioId)
DEFINE_EVENT(HorarioLiberadoEvent, long horarioId, horarioId)

#endif