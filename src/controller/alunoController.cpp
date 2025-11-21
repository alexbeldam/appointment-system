#include "controller/alunoController.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

#include "util/utils.hpp"
using namespace std;

// --- MÉTODOS PÚBLICOS DA CLASSE ALUNOCONTROLLER ---

// Construtor: Injeção de Dependência da Camada de Serviço (AlunoService).
AlunoController::AlunoController(const shared_ptr<AlunoService>& service)
    : service(service) {}

// CREATE: Cria um novo Aluno no sistema.
shared_ptr<Aluno> AlunoController::create(const string& nome,
                                          const string& email,
                                          const string& senha, long matricula) {
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
        if (matricula <= 0) {
            throw invalid_argument("A matrícula deve ser um número positivo.");
        }

        return service->save(nome, email, mock_bcrypt(senha), matricula);

        // 4. TRATAMENTO DE ERROS
    } catch (const invalid_argument& e) {
        // Captura erros de validação (formato e unicidade do Service)
        handle_controller_exception(e, "validar novos dados do Aluno");

        throw;
    } catch (const runtime_error& e) {
        // Captura erros de I/O ou falhas críticas do Service
        handle_controller_exception(e, "criar Aluno no serviço");

        throw;
    }
}

// READ: Busca um Aluno pelo ID.
shared_ptr<Aluno> AlunoController::read(long id) {
    try {
        return service->getById(id);
    } catch (const invalid_argument& e) {
        // Captura o erro "não encontrado" lançado acima.
        handle_controller_exception(e, "ler Aluno pelo ID " + to_string(id));

        throw;
    } catch (const runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e, "ler Aluno pelo ID " + to_string(id));

        throw;
    }
}

// UPDATE: Atualiza um registro existente com lógica de "patch" (parcial).
shared_ptr<Aluno> AlunoController::update(long id, const string& nome,
                                          const string& email,
                                          const string& senha, long matricula) {
    try {
        // 1. OBTÉM DADOS ATUAIS (Base para o Patch)
        auto late = service->getById(id);

        if (!late) {
            throw invalid_argument("Aluno com ID " + to_string(id) +
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

        // d. Matrícula: Usa a matrícula nova se for positiva. Caso contrário,
        // mantém a antiga.
        long newMatricula = (matricula <= 0) ? late->getMatricula() : matricula;

        // 3. SERVICE CALL (Service verifica unicidade final e persiste)
        auto updated =
            service->updateById(id, newNome, newEmail, newSenha, newMatricula);

        // Se o Service retornar nullopt, algo falhou internamente no Service
        // após o getById.
        if (!updated) {
            throw runtime_error(
                "O registro foi encontrado, mas a atualização falhou de forma "
                "inesperada.");
        }

        return updated;
    } catch (const invalid_argument& e) {
        // Captura erros de validação (Controller) e erros de unicidade/não
        // encontrado (Service)
        handle_controller_exception(
            e, "validar dados de atualização para o ID " + to_string(id));

        throw;
    } catch (const runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e,
                                    "atualizar Aluno com ID " + to_string(id));

        throw;
    }
}

// DESTROY: Deleta um registro pelo ID.
bool AlunoController::destroy(long id) {
    try {
        // Service retorna true (sucesso) ou false (ID não encontrado).
        return service->deleteById(id);
    } catch (const runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e, "excluir Aluno com ID " + to_string(id));
        throw;
    }
}