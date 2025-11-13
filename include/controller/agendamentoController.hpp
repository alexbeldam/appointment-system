#ifndef AGENDAMENTO_CONTROLLER_HPP
#define AGENDAMENTO_CONTROLLER_HPP

#include "service/agendamentoService.hpp"
#include "service/horarioService.hpp"

/**
 * @brief Camada de Controle para o caso de uso "Agendar Horário".
 *
 * Recebe requisições da UI (ConsoleUI) e orquestra as chamadas
 * necessárias para a camada de serviço (AgendamentoService).
 */
class AgendamentoController {
   private:
    /**
     * @brief Referência constante ao serviço de agendamento.
     */
    const AgendamentoService& agendamentoService;
    const HorarioService& horarioService;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param service Referência constante para o serviço de agendamento.
     */
    AgendamentoController(const AgendamentoService& service, const HorarioService &horarioservice);

    /**
     * @brief Orquestra a ação de agendar um horário.
     *
     * Esta função é chamada pela UI. A UI é responsável por fornecer
     * tanto o ID do aluno logado quanto o ID do horário selecionado.
     *
     * A UI deve estar preparada para capturar (try/catch) exceções
     * vindas deste método (ex: horário indisponível - AC 2).
     *
     * @param alunoID O ID do aluno (fornecido pela UI).
     * @param horarioId O ID do horário que o usuário selecionou na UI.
     */
    void agendarHorario(long alunoID, long horarioId) const;

     /**
     * @brief Cancela um agendamento existente e libera o horário correspondente.
     * @param agendamentoId ID do agendamento a ser cancelado.
     */
    void cancelar(long agendamentoId) const;

    void confirmar(long id) const;

    void recusar(long id) const;

};

#endif