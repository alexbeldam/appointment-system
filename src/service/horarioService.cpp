#include "service/horarioService.hpp"

// Recomendo manter o nome da tabela sem extensão para evitar o problema anterior
#define HORARIO_TABLE "horarios" 
#define ID_PROFESSOR_COL_INDEX 1

#include <sstream>
#include <stdexcept>

using std::vector;
using std::string;
using std::to_string;
using std::invalid_argument;
using std::runtime_error;


HorarioService::HorarioService(const MockConnection& connection, EventBus& bus)
    : connection(connection), bus(bus) {}

Horario HorarioService::save(long idProfessor, const std::string& inicio,
                             const std::string& fim) {
  //  Verifica se o horário final é posterior ao inicial
  if (fim <= inicio) {
    throw std::invalid_argument("O horário final deve ser posterior ao horário inicial.");
  }

  // 1. Buscar horários do professor
  auto horarios = connection.selectByColumn(HORARIO_TABLE, ID_PROFESSOR_COL_INDEX, std::to_string(idProfessor));

  // 2. Verificar conflito de intervalo
  for (const auto& linha : horarios) {
    std::stringstream ss(linha);
    std::string id, idProf, inicioExistente, fimExistente, disponivelStr;
    std::getline(ss, id, ',');
    std::getline(ss, idProf, ',');
    std::getline(ss, inicioExistente, ',');
    std::getline(ss, fimExistente, ',');
    std::getline(ss, disponivelStr, ',');

    if (inicio == inicioExistente && fim == fimExistente) {
      Horario conflito(std::stol(id), std::stol(idProf), inicioExistente, fimExistente, true);
      bus.publish(HorarioConflictEvent(conflito));
      throw std::invalid_argument("Já existe um horário cadastrado nesse período.");
    }
  }

  // 3. Inserir novo horário
  std::stringstream dados;
  dados << idProfessor << "," << inicio << "," << fim << ",1";
  long newId = connection.insert(HORARIO_TABLE, dados.str());

  Horario novo(newId, idProfessor, inicio, fim, true);
  bus.publish(HorarioCreatedEvent(novo));

  return novo;
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

std::vector<Horario> HorarioService::listDisponivelByIdProfessor(long id) const {
    std::vector<Horario> todosHorarios = listByIdProfessor(id);
    std::vector<Horario> horariosDisponiveis;

    for (const auto& horario : todosHorarios) {
        if (horario.isDisponivel()) {
            horariosDisponiveis.push_back(horario);
        }
    }

    return horariosDisponiveis;
}


std::vector<Horario> HorarioService::listByIdProfessor(long id) const {
  auto linhas = connection.selectByColumn(HORARIO_TABLE, ID_PROFESSOR_COL_INDEX, std::to_string(id));
  std::vector<Horario> horarios;
  for (const auto& linha : linhas) {
    std::stringstream ss(linha);
    std::string idStr, idProf, inicio, fim, disponivelStr;
    std::getline(ss, idStr, ',');
    std::getline(ss, idProf, ',');
    std::getline(ss, inicio, ',');
    std::getline(ss, fim, ',');
    std::getline(ss, disponivelStr, ',');
    horarios.emplace_back(std::stol(idStr), std::stol(idProf), inicio, fim, disponivelStr == "1");
  }
  return horarios;
}