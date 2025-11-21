#ifndef AGENDAMENTO_CONTROLLER_HPP
#define AGENDAMENTO_CONTROLLER_HPP

#include "service/agendamentoService.hpp"

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
    const std::shared_ptr<AgendamentoService>& agendamentoService;

   public:
    /**
     * @brief Construtor para injeção de dependência.
     * @param service Referência constante para o serviço de agendamento.
     */
    AgendamentoController(const std::shared_ptr<AgendamentoService>& service);

    ~AgendamentoController() = default;

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
    std::shared_ptr<Agendamento> agendarHorario(long alunoID, long horarioId);

    /**
     * @brief Cancela um agendamento existente e libera o horário
     * correspondente.
     * @param agendamentoId ID do agvoidendamento a ser cancelado.
     */
    void cancelar(long agendamentoId);

    void confirmar(long id);

    void recusar(long id);
};

#endif