#include "controller/alunoController.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

#include "util/utils.hpp"
using namespace std;

// --- MÉTODOS PÚBLICOS DA CLASSE ALUNOCONTROLLER ---

// Construtor: Injeção de Dependência da Camada de Serviço (AlunoService).
AlunoController::AlunoController(const AlunoService& service)
    : service(service) {}

// CREATE: Cria um novo Aluno no sistema.
Aluno AlunoController::create(const string& nome, const string& email,
                              const string& senha, long matricula) const {
    try {
        // 1. VALIDAÇÃO DE FORMATO
        if (nome.empty() || nome.length() < 3) {
            throw std::invalid_argument(
                "O nome deve ter pelo menos 3 caracteres.");
        }
        if (email.find('@') == std::string::npos ||
            email.find('.') == std::string::npos) {
            throw std::invalid_argument("Formato de email inválido.");
        }
        if (senha.length() < 4) {
            throw std::invalid_argument(
                "A senha deve ter pelo menos 4 caracteres.");
        }
        if (!is_alphanumeric(senha)) {
            throw std::invalid_argument(
                "A senha deve conter apenas letras e números.");
        }
        if (matricula <= 0) {
            throw std::invalid_argument(
                "A matrícula deve ser um número positivo.");
        }

        return service.save(nome, email, mock_bcrypt(senha), matricula);

        // 4. TRATAMENTO DE ERROS
    } catch (const std::invalid_argument& e) {
        // Captura erros de validação (formato e unicidade do Service)
        handle_controller_exception(e, "validar novos dados do Aluno");

        throw;
    } catch (const std::runtime_error& e) {
        // Captura erros de I/O ou falhas críticas do Service
        handle_controller_exception(e, "criar Aluno no serviço");

        throw;
    }
}

// READ: Busca um Aluno pelo ID.
Aluno AlunoController::read(long id) const {
    try {
        // Service retorna std::optional<Aluno>
        optional<Aluno> aluno_opt = service.getById(id);

        if (!aluno_opt.has_value()) {
            // Se o optional estiver vazio (ID não encontrado), lança erro de
            // negócio.
            throw std::invalid_argument("Aluno com ID " + std::to_string(id) +
                                        " não encontrado.");
        }

        return aluno_opt.value();  // Retorna o objeto Aluno.

    } catch (const std::invalid_argument& e) {
        // Captura o erro "não encontrado" lançado acima.
        handle_controller_exception(e,
                                    "ler Aluno pelo ID " + std::to_string(id));

        throw;
    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e,
                                    "ler Aluno pelo ID " + std::to_string(id));

        throw;
    }
}

// LIST: Retorna todos os Alunos.
vector<Aluno> AlunoController::list() const {
    try {
        // Service retorna um vetor (pode ser vazio).
        return service.listAll();
    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e, "listar todos os Alunos");

        throw;
    }
}

// UPDATE: Atualiza um registro existente com lógica de "patch" (parcial).
Aluno AlunoController::update(long id, const string& nome, const string& email,
                              const string& senha, long matricula) const {
    try {
        // 1. OBTÉM DADOS ATUAIS (Base para o Patch)
        optional<Aluno> existingAluno_opt = service.getById(id);

        if (!existingAluno_opt.has_value()) {
            throw std::invalid_argument("Aluno com ID " + std::to_string(id) +
                                        " não encontrado para atualização.");
        }
        Aluno existingAluno = existingAluno_opt.value();

        // a. Nome: Usa o novo valor se não for vazio. Valida o resultado.
        string newNome = nome.empty() ? existingAluno.getNome() : nome;
        if (newNome.length() < 3) {
            throw std::invalid_argument(
                "O nome deve ter pelo menos 3 caracteres.");
        }

        // b. Email: Usa o novo valor se não for vazio. Valida o resultado.
        string newEmail = email.empty() ? existingAluno.getEmail() : email;
        if (newEmail.find('@') == std::string::npos ||
            newEmail.find('.') == std::string::npos) {
            throw std::invalid_argument("Formato de email inválido.");
        }

        // c. Senha: Se vazia, mantém a senha antiga. Caso contrário, valida e
        // criptografa a nova.
        string newSenha;
        if (senha.empty()) {
            newSenha = existingAluno.getSenha();
        } else {
            if (senha.length() < 4) {
                throw std::invalid_argument(
                    "A nova senha deve ter pelo menos 4 caracteres.");
            }
            if (!is_alphanumeric(senha)) {
                throw std::invalid_argument(
                    "A nova senha deve conter apenas letras e números...");
            }
            newSenha = mock_bcrypt(senha);
        }

        // d. Matrícula: Usa a matrícula nova se for positiva. Caso contrário,
        // mantém a antiga.
        long newMatricula =
            (matricula <= 0) ? existingAluno.getMatricula() : matricula;

        // 3. SERVICE CALL (Service verifica unicidade final e persiste)
        optional<Aluno> updatedAluno_opt =
            service.updateById(id, newNome, newEmail, newSenha, newMatricula);

        // Se o Service retornar nullopt, algo falhou internamente no Service
        // após o getById.
        if (!updatedAluno_opt.has_value()) {
            throw std::runtime_error(
                "O registro foi encontrado, mas a atualização falhou de forma "
                "inesperada.");
        }

        return updatedAluno_opt.value();

        // 4. TRATAMENTO DE ERROS
    } catch (const std::invalid_argument& e) {
        // Captura erros de validação (Controller) e erros de unicidade/não
        // encontrado (Service)
        handle_controller_exception(
            e, "validar dados de atualização para o ID " + std::to_string(id));

        throw;
    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(
            e, "atualizar Aluno com ID " + std::to_string(id));

        throw;
    }
}

// DESTROY: Deleta um registro pelo ID.
bool AlunoController::destroy(long id) const {
    try {
        // Service retorna true (sucesso) ou false (ID não encontrado).
        return service.deleteById(id);
    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(
            e, "excluir Aluno com ID " + std::to_string(id));
        throw;
    }
}