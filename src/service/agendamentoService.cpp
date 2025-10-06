#include "service/agendamentoService.hpp"

// Incluir o evento necessário para a funcionalidade de deleção
#include "event/events.hpp"
using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---

// Define o nome da "tabela" (arquivo CSV) gerenciada por este Service.
#define AGENDAMENTO_TABLE "agendamentos"
// Índices das colunas usados para buscas específicas.
#define ID_ALUNO_COL_INDEX 1

AgendamentoService::AgendamentoService(const MockConnection& connection,
                                       EventBus& bus)
    : connection(connection), bus(bus) {
    // Assina o evento de deleção de aluno para garantir a integridade
    // referencial. Quando um aluno é deletado, seus agendamentos são
    // removidos.
    bus.subscribe<AlunoDeletedEvent>([this](const AlunoDeletedEvent& event) {
        this->deleteByIdAluno(event.id);
    });
}

// A função é o ponto de injeção de dados relacionados para a classe Aluno.
vector<Agendamento> AgendamentoService::listByIdAluno(long id) const {
    // [TODO] Esta é uma função placeholder incompleta.

    // A implementação final deve:
    // 1. Chamar connection.selectByColumn(AGENDAMENTO_TABLE,
    // ID_ALUNO_COL_INDEX, to_string(id)).
    // 2. Mapear as linhas CSV resultantes para objetos Agendamento (usando um
    // AgendamentoMapper).
    // 3. Tratar exceções de I/O do DAL (runtime_error).

    // Retorna um vetor vazio, cumprindo o contrato do placeholder.
    return vector<Agendamento>();
}

void AgendamentoService::deleteByIdAluno(long id) const {
    // [TODO] Implementar a lógica de deleção em massa no DAL.

    // A implementação final deve:
    // 1. Chamar connection.deleteByColumn(AGENDAMENTO_TABLE,
    // ID_ALUNO_COL_INDEX, to_string(id)).
    // 2. Tratar exceções de I/O.
}