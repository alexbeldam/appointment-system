#include "service/horarioService.hpp"

using namespace std;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---

// Define o nome da "tabela" (arquivo CSV) gerenciada por este Service.
#define HORARIO_TABLE "horarios"
// Índices das colunas usados para buscas específicas.
#define ID_PROFESSOR_COL_INDEX 1

// Construtor: Inicializa a única dependência necessária (a MockConnection).
HorarioService::HorarioService(const MockConnection& connection)
    : connection(connection) {}

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