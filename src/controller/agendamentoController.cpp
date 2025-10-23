#include "controller/agendamentoController.hpp"

#include <stdexcept>
#include <string>

#include "model/agendamento.hpp"
#include "util/utils.hpp"  // Para handle_controller_exception

// --- MÉTODOS PÚBLICOS DA CLASSE AGENDAMENTOCONTROLLER ---

/**
 * @brief Construtor: Injeção de Dependência da Camada de Serviço (AgendamentoService).
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
        // O Controller valida se os IDs são minimamente aceitáveis.
        if (alunoID <= 0) {
            throw std::invalid_argument("ID do aluno inválido ou não fornecido.");
        }
        if (horarioId <= 0) {
            throw std::invalid_argument("ID do horário inválido ou não fornecido.");
        }

        // 2. PREPARAÇÃO DO MODELO
        // Com base no CSV (id,aluno,horario,status), definimos
        // "CONFIRMADO" como o status padrão para novos agendamentos.
        const std::string statusPadrao = "CONFIRMADO";
        Agendamento novoAgendamento(alunoID, horarioId, statusPadrao);

        // 3. DELEGAÇÃO PARA O SERVICE
        // O Service trata a lógica de negócio principal:
        // - Verifica se o Horario (horarioId) está disponível (AC 2)
        // - Salva o novo Agendamento no CSV (AC 1)
        // - Atualiza o Horario para "reservado" (AC 1)
        // - Dispara o evento de notificação (AC 1)
        this->agendamentoService.save(novoAgendamento);

        // 4. TRATAMENTO DE ERROS
    } catch (const std::invalid_argument& e) {
        // Captura erros de validação (IDs <= 0)
        handle_controller_exception(e, "validar dados do novo agendamento");
        
    } catch (const std::runtime_error& e) {
        // Captura erros de I/O ou de lógica de negócio do Service
        // (Ex: "Horário indisponível." ou "Horário não encontrado.")
        handle_controller_exception(e, "criar agendamento no serviço");
    }
    // Relança a exceção para a camada da UI (Console) tratar
    throw;
}