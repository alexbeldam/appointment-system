#include "controller/professorController.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

#include "util/utils.hpp"
using namespace std;

// --- MÉTODOS PÚBLICOS DA CLASSE PROFESSORCONTROLLER ---

// Construtor: Injeção de Dependência da Camada de Serviço (ProfessorService).
ProfessorController::ProfessorController(
    const shared_ptr<ProfessorService>& service)
    : service(service) {}

// CREATE: Cria um novo Professor no sistema.
shared_ptr<Professor> ProfessorController::create(const string& nome,
                                                  const string& email,
                                                  const string& senha,
                                                  const string& disciplina) {
    try {
        // 1. VALIDAÇÃO DE FORMATO
        if (nome.empty() || nome.length() < 3) {
            throw invalid_argument("O nome deve ter pelo menos 3 caracteres.");
        }
        if (email.find('@') == string::npos ||
            email.find('.') == string::npos) {
            throw invalid_argument("Formato de email inválido.");
        }
        if (senha.length() < 4) {
            throw invalid_argument("A senha deve ter pelo menos 4 caracteres.");
        }
        if (!is_alphanumeric(senha)) {
            throw invalid_argument(
                "A senha deve conter apenas letras e números.");
        }
        if (disciplina.empty() || disciplina.length() < 3) {
            throw invalid_argument(
                "A disciplina deve ter pelo menos 3 caracteres.");
        }

        return service->save(nome, email, mock_bcrypt(senha), disciplina);

        // 4. TRATAMENTO DE ERROS
    } catch (const invalid_argument& e) {
        // Captura erros de validação (formato e unicidade do Service)
        handle_controller_exception(e, "validar novos dados do Professor");

        throw;
    } catch (const runtime_error& e) {
        // Captura erros de I/O ou falhas críticas do Service
        handle_controller_exception(e, "criar Professor no serviço");

        throw;
    }
}

// READ: Busca um Professor pelo ID.
shared_ptr<Professor> ProfessorController::read(long id) {
    try {
        return service->getById(id);
    } catch (const invalid_argument& e) {
        // Captura o erro "não encontrado" lançado acima.
        handle_controller_exception(e,
                                    "ler Professor pelo ID " + to_string(id));

        throw;
    } catch (const runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e,
                                    "ler Professor pelo ID " + to_string(id));

        throw;
    }
}

// LIST: Retorna todos os Professors.
vector<shared_ptr<Professor>> ProfessorController::list() {
    try {
        // Service retorna um vetor (pode ser vazio).
        return service->listAll();
    } catch (const runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e, "listar todos os Professors");

        throw;
    }
}

// UPDATE: Atualiza um registro existente com lógica de "patch" (parcial).
shared_ptr<Professor> ProfessorController::update(long id, const string& nome,
                                                  const string& email,
                                                  const string& senha,
                                                  const string& disciplina) {
    try {
        // 1. OBTÉM DADOS ATUAIS (Base para o Patch)
        auto late = service->getById(id);

        if (!late) {
            throw invalid_argument("Professor com ID " + to_string(id) +
                                   " não encontrado para atualização.");
        }

        // a. Nome: Usa o novo valor se não for vazio. Valida o resultado.
        string newNome = nome.empty() ? late->getNome() : nome;
        if (newNome.length() < 3) {
            throw invalid_argument("O nome deve ter pelo menos 3 caracteres.");
        }

        // b. Email: Usa o novo valor se não for vazio. Valida o resultado.
        string newEmail = email.empty() ? late->getEmail() : email;
        if (newEmail.find('@') == string::npos ||
            newEmail.find('.') == string::npos) {
            throw invalid_argument("Formato de email inválido.");
        }

        // c. Senha: Se vazia, mantém a senha antiga. Caso contrário, valida e
        // criptografa a nova.
        string newSenha;
        if (senha.empty()) {
            newSenha = late->getSenha();
        } else {
            if (senha.length() < 4) {
                throw invalid_argument(
                    "A nova senha deve ter pelo menos 4 caracteres.");
            }
            if (!is_alphanumeric(senha)) {
                throw invalid_argument(
                    "A nova senha deve conter apenas letras e números...");
            }
            newSenha = mock_bcrypt(senha);
        }

        // d. Disciplina: Usa o novo valor se não for vazio. Valida o resultado.
        string newDisciplina =
            disciplina.empty() ? late->getDisciplina() : disciplina;
        if (newDisciplina.length() < 3) {
            throw invalid_argument("O nome deve ter pelo menos 3 caracteres.");
        }

        // 3. SERVICE CALL (Service verifica unicidade final e persiste)
        auto updated =
            service->updateById(id, newNome, newEmail, newSenha, newDisciplina);

        // Se o Service retornar nullopt, algo falhou internamente no Service
        // após o getById.
        if (!updated) {
            throw runtime_error(
                "O registro foi encontrado, mas a atualização falhou de forma "
                "inesperada.");
        }

        return updated;

        // 4. TRATAMENTO DE ERROS
    } catch (const invalid_argument& e) {
        // Captura erros de validação (Controller) e erros de unicidade/não
        // encontrado (Service)
        handle_controller_exception(
            e, "validar dados de atualização para o ID " + to_string(id));

        throw;
    } catch (const runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(
            e, "atualizar Professor com ID " + to_string(id));

        throw;
    }
}

// DESTROY: Deleta um registro pelo ID.
bool ProfessorController::destroy(long id) {
    try {
        // Service retorna true (sucesso) ou false (ID não encontrado).
        return service->deleteById(id);
    } catch (const runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(
            e, "excluir Professor com ID " + to_string(id));

        throw;
    }
}