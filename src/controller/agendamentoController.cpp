#include "controller/agendamentoController.hpp"

#include <stdexcept>
#include <string>

#include "model/agendamento.hpp"
#include "util/utils.hpp"  // Para handle_controller_exception

// --- MÉTODOS PÚBLICOS DA CLASSE AGENDAMENTOCONTROLLER ---

/**
 * @brief Construtor: Injeção de Dependência da Camada de Serviço
 * (AgendamentoService).
 * @param service Referência constante para o serviço de agendamento.
 */

AgendamentoController::AgendamentoController(const AgendamentoService& service)
    : agendamentoService(service) {
    // Lista de inicialização de membros faz todo o trabalho.
}

/**
 * @brief Orquestra a ação de criar (agendar) um novo horário.
 *
 * Segue o padrão: Validar -> Preparar -> Delegar -> Tratar Exceções.
 *
 * @param alunoID O ID do aluno (fornecido pela UI).
 * @param horarioId O ID do horário que o usuário selecionou na UI.
 */
void AgendamentoController::agendarHorario(long alunoID, long horarioId) const {
    try {
        // 1. VALIDAÇÃO DE FORMATO
        if (alunoID <= 0) {
            throw std::invalid_argument(
                "ID do aluno inválido ou não fornecido.");
        }
        if (horarioId <= 0) {
            throw std::invalid_argument(
                "ID do horário inválido ou não fornecido.");
        }

        this->agendamentoService.save(alunoID, horarioId);

    } catch (const std::invalid_argument& e) {
        handle_controller_exception(e, "validar dados do novo agendamento");

        throw;
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "criar agendamento no serviço");

        throw;
    }
}