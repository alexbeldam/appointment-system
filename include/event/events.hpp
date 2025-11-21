#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <memory>
#include <string>

/**
 * @brief Template base para todos os eventos do sistema.
 * * Fornece uma funcionalidade comum (como obter o nome do tipo) para todas
 * as classes de evento que herdam dela.
 * * O uso de CRTP (Curiously Recurring Template Pattern) com 'Derrived' permite
 * que a classe base acesse informações específicas do tipo derivado.
 * @tparam Derrived A classe de evento que herda desta base.
 */
template <typename Derrived>
struct Event {
    /**
     * @brief Retorna o nome do tipo da classe de evento.
     * * Utiliza RTTI (Run-Time Type Information) via typeid.
     * @return std::string O nome do tipo do evento.
     */
    std::string name() const {
        return typeid(Derrived).name();
    }
};

/**
 * @brief Macro para definir classes de evento de forma concisa.
 * * Simplifica a criação de classes de evento, garantindo que herdem de Event
 * e inicializem seus membros corretamente no construtor.
 * * @param EventName O nome da nova classe de evento.
 * @param MemberTypeAndName O tipo e nome do membro (ex: long professorId).
 * @param MemberName O nome do membro a ser inicializado (ex: professorId).
 */
#define DEFINE_EVENT(EventName, MemberTypeAndName, MemberName)   \
    struct EventName : public Event<EventName> {                 \
        MemberTypeAndName;                                       \
        EventName(MemberTypeAndName) : MemberName(MemberName) {} \
    };

/**
 * @brief Evento disparado quando um Professor realiza login com sucesso.
 */
DEFINE_EVENT(ProfessorLoggedInEvent, long professorId, professorId)

/**
 * @brief Evento disparado quando um Aluno realiza login com sucesso.
 */
DEFINE_EVENT(AlunoLoggedInEvent, long alunoId, alunoId)

/**
 * @brief Evento disparado após a exclusão de um Professor.
 */
DEFINE_EVENT(ProfessorDeletedEvent, long professorId, professorId)

/**
 * @brief Evento disparado após a exclusão de um Aluno.
 */
DEFINE_EVENT(AlunoDeletedEvent, long alunoId, alunoId)

/**
 * @brief Evento disparado quando um Horário se torna ocupado (reservado).
 */
DEFINE_EVENT(HorarioOcupadoEvent, long horarioId, horarioId)

/**
 * @brief Evento disparado quando um Horário se torna livre (desreservado).
 */
DEFINE_EVENT(HorarioLiberadoEvent, long horarioId, horarioId)

#endif