#include "service/horarioService.hpp"

#include "event/events.hpp"
using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---

// Define o nome da "tabela" (arquivo CSV) gerenciada por este Service.
#define HORARIO_TABLE "horarios"
// Índices das colunas usados para buscas específicas.
#define ID_PROFESSOR_COL_INDEX 1

HorarioService::HorarioService(const MockConnection& connection, EventBus& bus)
    : connection(connection), bus(bus) {
    // Assina o evento de deleção de professor para garantir a integridade
    // referencial. Quando um professor é deletado, seus horarios são
    // removidos.
    bus.subscribe<ProfessorDeletedEvent>(
        [this](const ProfessorDeletedEvent& event) {
            this->deleteByIdProfessor(event.id);
        });
}

// A função é o ponto de injeção de dados relacionados para a classe Professor.
vector<Horario> HorarioService::listDisponivelByIdProfessor(long id) const {
    // [TODO] Esta é uma função placeholder incompleta.

    // A implementação final deve:
    // 1. Chamar connection.selectByColumn(HORARIO_TABLE,
    // ID_PROFESSOR_COL_INDEX, to_string(id)) para buscar TODOS os horários do
    // professor.
    // 2. Mapear as linhas CSV para objetos Horario.
    // 3. Filtrar a lista resultante, mantendo apenas aqueles onde o status de
    // DISPONIVEL (coluna DISPONIVEL_COL_INDEX) é igual a '1'.

    // Atualmente, retorna um vetor vazio para cumprir o contrato e resolver
    // dependências.
    return vector<Horario>();
}

void HorarioService::deleteByIdProfessor(long id) const {
    // [TODO] Implementar a lógica de deleção em massa no DAL.

    // A implementação final deve:
    // 1. Chamar connection.deleteByColumn(HORARIO_TABLE,
    // ID_PROFESSOR_COL_INDEX, to_string(id)).
    // 2. Tratar exceções de I/O.
}