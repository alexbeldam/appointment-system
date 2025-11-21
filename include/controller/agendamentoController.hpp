#ifndef AGENDAMENTO_CONTROLLER_HPP
#define AGENDAMENTO_CONTROLLER_HPP

#include "service/agendamentoService.hpp"

/**
 * @brief Controller para gerenciamento de Agendamentos.
 * * Esta classe atua como a camada de controle (Controller) do padrão MVC,
 * responsável por receber requisições, delegar a lógica de negócio
 * para a camada de Service e retornar o resultado.
 */
class AgendamentoController {
   private:
    /**
     * @brief Ponteiro inteligente para o serviço de agendamentos.
     * * Responsável por isolar a lógica de negócio e o acesso aos dados.
     */
    const std::shared_ptr<AgendamentoService>& agendamentoService;

   public:
    /**
     * @brief Construtor da classe AgendamentoController.
     * * @param service O serviço de agendamentos injetado via dependência.
     */
    AgendamentoController(const std::shared_ptr<AgendamentoService>& service);

    /**
     * @brief Destrutor padrão.
     */
    ~AgendamentoController() = default;

    /**
     * @brief Cria um novo agendamento (Requisição POST).
     * * O agendamento é criado com o status inicial **PENDENTE**.
     * * @param alunoID O identificador único do aluno que está agendando.
     * @param horarioId O identificador único do horário desejado.
     * @return std::shared_ptr<Agendamento> O objeto Agendamento recém-criado.
     */
    std::shared_ptr<Agendamento> agendarHorario(long alunoID, long horarioId);

    /**
     * @brief Altera o status do agendamento para CANCELADO.
     * * Realiza uma atualização parcial (PATCH) no recurso, modificando apenas
     * o campo status.
     * * @param id O identificador único do agendamento a ser cancelado.
     */
    void cancelar(long id);

    /**
     * @brief Altera o status do agendamento para CONFIRMADO.
     * * Realiza uma atualização parcial (PATCH) no recurso, modificando apenas
     * o campo status.
     * * @param id O identificador único do agendamento a ser confirmado.
     */
    void confirmar(long id);

    /**
     * @brief Altera o status do agendamento para RECUSADO.
     * * Realiza uma atualização parcial (PATCH) no recurso, modificando apenas
     * o campo status.
     * * @param id O identificador único do agendamento a ser recusado.
     */
    void recusar(long id);
};

#endif