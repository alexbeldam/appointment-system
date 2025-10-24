#include "service/agendamentoService.hpp"
#include "event/events.hpp"
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <optional>

using std::vector;
using std::string;
using std::to_string;
using std::invalid_argument;
using std::runtime_error;
using std::stringstream;
using std::getline;
using std::stol;

// --- CONFIGURAÇÕES DE PERSISTÊNCIA ---
#define AGENDAMENTO_TABLE "agendamentos"
#define ID_ALUNO_COL_INDEX 1
#define ID_HORARIO_COL_INDEX 2

// --- DECLARAÇÃO DE FUNÇÃO AUXILIAR ---
static vector<Agendamento> listByIdHorario(const MockConnection& connection, long id);

// --- CONSTRUTOR ---
AgendamentoService::AgendamentoService(const MockConnection& connection,
                                     EventBus& bus, const HorarioService& horarioService)
    : connection(connection), bus(bus), horarioService(horarioService) {
    
    bus.subscribe<AlunoDeletedEvent>([this](const AlunoDeletedEvent& event) {
        this->deleteByIdAluno(event.id);
    });

    bus.subscribe<HorarioDeletedEvent>(
        [this](const HorarioDeletedEvent& event) {
            this->deleteByIdHorario(event.id);
        });
}

// --- MÉTODOS CRUD ---

/**
 * @brief Salva um novo agendamento, aplicando as regras de negócio.
 */
Agendamento AgendamentoService::save(const Agendamento& agendamento) const {
    
    // AC 2 (Parte 1): O professor abriu este slot?
    try {
        Horario h = this->horarioService.getById(agendamento.getHorarioId());
        if (!h.isDisponivel()) { 
            throw std::runtime_error("Este horário não está aberto para agendamentos.");
        }
    } catch (const std::runtime_error& e) {
        throw; // Repassa erro (ex: "Horário não encontrado")
    }

    // AC 2 (Parte 2): O slot já foi reservado por outro aluno?
    // (Verifica se já existe agendamento, PENDENTE ou CONFIRMADO)
    vector<Agendamento> agendamentosExistentes = listByIdHorario(this->connection, agendamento.getHorarioId());
    
    if (!agendamentosExistentes.empty()) {
        throw std::runtime_error("Horário indisponível (já solicitado por outro aluno).");
    }

    // AC 1 (Registro): Salvar o agendamento (com status "PENDENTE")
    stringstream dados;
    dados << agendamento.getAlunoId() << "," 
          << agendamento.getHorarioId() << "," 
          << agendamento.getStatus(); // status será "PENDENTE"
          
    long newId = connection.insert(AGENDAMENTO_TABLE, dados.str());
    
    Agendamento salvo(newId, agendamento.getAlunoId(), 
                      agendamento.getHorarioId(), agendamento.getStatus());

    // AC 1 (Notificar)
    this->bus.publish(AgendamentoCreatedEvent(salvo));

    return salvo;
}

Agendamento AgendamentoService::getById(long id) const {
    try {
        string linha = connection.selectOne(AGENDAMENTO_TABLE, id);
        if (linha.empty()) {
            throw runtime_error("Agendamento não encontrado.");
        }

        stringstream ss(linha);
        string idStr, alunoIdStr, horarioIdStr, statusStr;
        getline(ss, idStr, ',');
        getline(ss, alunoIdStr, ',');
        getline(ss, horarioIdStr, ',');
        getline(ss, statusStr, ',');

        return Agendamento(stol(idStr), stol(alunoIdStr), stol(horarioIdStr), statusStr);

    } catch (const runtime_error& e) {
        throw; 
    }
}

vector<Agendamento> AgendamentoService::listAll() const {
    vector<Agendamento> agendamentos;
    try {
        auto linhas = connection.selectAll(AGENDAMENTO_TABLE);
        for (const auto& linha : linhas) {
            stringstream ss(linha);
            string idStr, alunoIdStr, horarioIdStr, statusStr;
            getline(ss, idStr, ',');
            getline(ss, alunoIdStr, ',');
            getline(ss, horarioIdStr, ',');
            getline(ss, statusStr, ',');

            agendamentos.emplace_back(stol(idStr), stol(alunoIdStr), stol(horarioIdStr), statusStr);
        }
        return agendamentos;
    } catch (const runtime_error& e) {
        throw; 
    }
}

std::optional<Agendamento> AgendamentoService::updateById(long id, const Agendamento& agendamento) const {
    try {
        stringstream dados;
        dados << agendamento.getAlunoId() << "," 
              << agendamento.getHorarioId() << "," 
              << agendamento.getStatus();
              
        connection.update(AGENDAMENTO_TABLE, id, dados.str());
        Agendamento agendamentoAtualizado(id, agendamento.getAlunoId(), 
                                          agendamento.getHorarioId(), agendamento.getStatus());
        this->bus.publish(AgendamentoUpdatedEvent(agendamentoAtualizado));
        return agendamentoAtualizado;
    }catch(const invalid_argument& e){
        return std::nullopt;
    } catch (const runtime_error& e) {
        throw;
    }
}

bool AgendamentoService::deleteById(long id) const {
    try {
        (void)this->getById(id); // Verifica se existe
        connection.deleteRecord(AGENDAMENTO_TABLE, id);
        bus.publish(AgendamentoDeletedEvent(id));
        return true;
    } catch (...) {
        return false; 
    }
}


// --- MÉTODOS EXISTENTES ---

vector<Agendamento> AgendamentoService::listByIdAluno(long id) const {
    vector<Agendamento> agendamentos;
    try {
        auto linhas = connection.selectByColumn(AGENDAMENTO_TABLE, ID_ALUNO_COL_INDEX, to_string(id));
        for (const auto& linha : linhas) {
            stringstream ss(linha);
            string idStr, alunoIdStr, horarioIdStr, statusStr;
            getline(ss, idStr, ',');
            getline(ss, alunoIdStr, ',');
            getline(ss, horarioIdStr, ',');
            getline(ss, statusStr, ',');

            agendamentos.emplace_back(stol(idStr), stol(alunoIdStr), stol(horarioIdStr), statusStr);
        }
        return agendamentos;
    } catch (const runtime_error& e) {
        throw;
    }
}

bool AgendamentoService::deleteByIdAluno(long id) const {
    try {
        vector<Agendamento> agendamentos = listByIdAluno(id);
        if (agendamentos.empty())
            return false;
        connection.deleteByColumn(AGENDAMENTO_TABLE, ID_ALUNO_COL_INDEX, to_string(id));
        for (const auto& a : agendamentos)
            bus.publish(AgendamentoDeletedEvent(a.getId()));
        return true;
    } catch (const invalid_argument& e) {
        return false; 
    } catch (const runtime_error& e) {
        throw; 
    }
}

bool AgendamentoService::deleteByIdHorario(long id) const {
    try {
        vector<Agendamento> agendamentos = listByIdHorario(this->connection, id);
        if (agendamentos.empty())
            return false;
        connection.deleteByColumn(AGENDAMENTO_TABLE, ID_HORARIO_COL_INDEX, to_string(id));
        for (const auto& a : agendamentos)
            bus.publish(AgendamentoDeletedEvent(a.getId()));
        return true;
    } catch (const invalid_argument& e) {
        return false; 
    } catch (const runtime_error& e) {
        throw;
    }
}


// --- IMPLEMENTAÇÃO DA FUNÇÃO AUXILIAR ---

/**
 * @brief Método auxiliar estático para listar agendamentos por ID_HORARIO.
 * @param connection Conexão com o banco de dados.
 * @param id O id do horario.
 * @return vector<Agendamento>
 */
static vector<Agendamento> listByIdHorario(const MockConnection& connection, long id) {
    vector<Agendamento> agendamentos;
    try {
        auto linhas = connection.selectByColumn(AGENDAMENTO_TABLE, ID_HORARIO_COL_INDEX, to_string(id));
        for (const auto& linha : linhas) {
            stringstream ss(linha);
            string idStr, alunoIdStr, horarioIdStr, statusStr;
            getline(ss, idStr, ',');
            getline(ss, alunoIdStr, ',');
            getline(ss, horarioIdStr, ',');
            getline(ss, statusStr, ',');

            agendamentos.emplace_back(stol(idStr), stol(alunoIdStr), stol(horarioIdStr), statusStr);
        }
        return agendamentos;
    } catch (const runtime_error& e) {
        throw; 
    }
}