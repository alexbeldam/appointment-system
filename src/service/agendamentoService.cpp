#include "service/agendamentoService.hpp"

using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---

// Define o nome da "tabela" (arquivo CSV) gerenciada por este Service.
#define AGENDAMENTO_TABLE "agendamentos"
// Índices das colunas usados para buscas específicas.
#define ID_ALUNO_COL_INDEX 1

// Construtor: Inicializa a única dependência necessária (MockConnection).
AgendamentoService::AgendamentoService(const MockConnection& connection)
    : connection(connection) {}

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