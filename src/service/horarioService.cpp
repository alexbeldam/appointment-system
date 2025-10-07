#include "service/horarioService.hpp"

#include <stdexcept>

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

// A função é usada para reagir ao evento de deleção do Professor.
vector<Horario> HorarioService::listByIdProfessor(long id) const {
    // [TODO] Esta é uma função placeholder incompleta.

    // A implementação final deve:
    // 1. Chamar connection.selectByColumn(HORARIO_TABLE,
    // ID_PROFESSOR_COL_INDEX, to_string(id)) para buscar TODOS os horários do
    // professor.
    // 2. Mapear as linhas CSV para objetos Horario.

    // Atualmente, retorna um vetor vazio para cumprir o contrato e resolver
    // dependências.
    return vector<Horario>();
}

// A função é o ponto de injeção de dados relacionados para a classe Professor.
vector<Horario> HorarioService::listDisponivelByIdProfessor(long id) const {
    // [TODO] Esta é uma função placeholder incompleta.

    // A implementação final deve:
    // 1. Chamar listByProfessorId
    // 2. Filtrar a lista resultante, mantendo apenas aqueles com atributo
    // horario.disponivel for true

    // Atualmente, retorna listByProfessorId.
    return listByIdProfessor(id);
}

bool HorarioService::deleteByIdProfessor(long id) const {
    try {
        vector<Horario> horarios = listByIdProfessor(id);

        if (horarios.size() == 0)
            return false;

        // Deleta do arquivo
        connection.deleteByColumn(HORARIO_TABLE, ID_PROFESSOR_COL_INDEX,
                                  to_string(id));

        // Publica a deleção
        for (const auto& h : horarios)
            bus.publish(HorarioDeletedEvent(h.getId()));

        return true;
    } catch (const invalid_argument& e) {
        return false;  // Nenhum horário encontrado
    } catch (const runtime_error& e) {
        throw;  // Relança erros de I/O.
    }
}