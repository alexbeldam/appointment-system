#include "controller/professorController.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

#include "helper/encrypt.hpp"
#include "util/controllerUtils.hpp"
using namespace std;

// --- MÉTODOS PÚBLICOS DA CLASSE PROFESSORCONTROLLER ---

// Construtor: Injeção de Dependência da Camada de Serviço (ProfessorService).
ProfessorController::ProfessorController(const ProfessorService& service)
    : service(service) {}

// CREATE: Cria um novo Professor no sistema.
Professor ProfessorController::create(const string& nome, const string& email,
                                      const string& senha,
                                      const string& disciplina) const {
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
        if (disciplina.empty() || disciplina.length() < 3) {
            throw std::invalid_argument(
                "A disciplina deve ter pelo menos 3 caracteres.");
        }

        // 2. PREPARAÇÃO DO DTO
        ProfessorDTO dto;
        dto.setEmail(email);
        dto.setNome(nome);
        dto.setSenha(encrypt(
            senha));  // Criptografia da senha antes de enviar ao Service
        dto.setDisciplina(disciplina);

        // 3. DELEGAÇÃO PARA O SERVICE (Service trata unicidade e persistência)
        return service.save(dto);

        // 4. TRATAMENTO DE ERROS
    } catch (const std::invalid_argument& e) {
        // Captura erros de validação (formato e unicidade do Service)
        handle_controller_exception(e, "validar novos dados do Professor");
    } catch (const std::runtime_error& e) {
        // Captura erros de I/O ou falhas críticas do Service
        handle_controller_exception(e, "criar Professor no serviço");
    }
    throw;
}

// READ: Busca um Professor pelo ID.
Professor ProfessorController::read(long id) const {
    try {
        // Service retorna std::optional<Professor>
        optional<Professor> professor_opt = service.getById(id);

        if (!professor_opt.has_value()) {
            // Se o optional estiver vazio (ID não encontrado), lança erro de
            // negócio.
            throw std::invalid_argument(
                "Professor com ID " + std::to_string(id) + " não encontrado.");
        }

        return professor_opt.value();  // Retorna o objeto Professor.

    } catch (const std::invalid_argument& e) {
        // Captura o erro "não encontrado" lançado acima.
        handle_controller_exception(
            e, "ler Professor pelo ID " + std::to_string(id));

    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(
            e, "ler Professor pelo ID " + std::to_string(id));
    }
    throw;
}

// LIST: Retorna todos os Professors.
vector<Professor> ProfessorController::list() const {
    try {
        // Service retorna um vetor (pode ser vazio).
        return service.listAll();
    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(e, "listar todos os Professors");
    }
    throw;
}

// UPDATE: Atualiza um registro existente com lógica de "patch" (parcial).
Professor ProfessorController::update(long id, const string& nome,
                                      const string& email, const string& senha,
                                      const string& disciplina) const {
    try {
        // 1. OBTÉM DADOS ATUAIS (Base para o Patch)
        optional<Professor> existingProfessor_opt = service.getById(id);

        if (!existingProfessor_opt.has_value()) {
            throw std::invalid_argument("Professor com ID " +
                                        std::to_string(id) +
                                        " não encontrado para atualização.");
        }
        Professor existingProfessor = existingProfessor_opt.value();

        // 2. MONTA DTO USANDO LÓGICA DE PATCH
        ProfessorDTO dto;
        dto.setId(id);

        // a. Nome: Usa o novo valor se não for vazio. Valida o resultado.
        string newNome = nome.empty() ? existingProfessor.getNome() : nome;
        if (newNome.length() < 3) {
            throw std::invalid_argument(
                "O nome deve ter pelo menos 3 caracteres.");
        }
        dto.setNome(newNome);

        // b. Email: Usa o novo valor se não for vazio. Valida o resultado.
        string newEmail = email.empty() ? existingProfessor.getEmail() : email;
        if (newEmail.find('@') == std::string::npos ||
            newEmail.find('.') == std::string::npos) {
            throw std::invalid_argument("Formato de email inválido.");
        }
        dto.setEmail(newEmail);

        // c. Senha: Se vazia, mantém a senha antiga. Caso contrário, valida e
        // criptografa a nova.
        if (senha.empty()) {
            dto.setSenha(existingProfessor.getSenha());
        } else {
            if (senha.length() < 4) {
                throw std::invalid_argument(
                    "A nova senha deve ter pelo menos 4 caracteres.");
            }
            if (!is_alphanumeric(senha)) {
                throw std::invalid_argument(
                    "A nova senha deve conter apenas letras e números...");
            }
            dto.setSenha(encrypt(senha));
        }

        // d. Disciplina: Usa o novo valor se não for vazio. Valida o resultado.
        string newDisciplina =
            disciplina.empty() ? existingProfessor.getDisciplina() : disciplina;
        if (newDisciplina.length() < 3) {
            throw std::invalid_argument(
                "O nome deve ter pelo menos 3 caracteres.");
        }
        dto.setNome(newDisciplina);

        // 3. SERVICE CALL (Service verifica unicidade final e persiste)
        optional<Professor> updatedProfessor_opt = service.updateById(id, dto);

        // Se o Service retornar nullopt, algo falhou internamente no Service
        // após o getById.
        if (!updatedProfessor_opt.has_value()) {
            throw std::runtime_error(
                "O registro foi encontrado, mas a atualização falhou de forma "
                "inesperada.");
        }

        return updatedProfessor_opt.value();

        // 4. TRATAMENTO DE ERROS
    } catch (const std::invalid_argument& e) {
        // Captura erros de validação (Controller) e erros de unicidade/não
        // encontrado (Service)
        handle_controller_exception(
            e, "validar dados de atualização para o ID " + std::to_string(id));
    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(
            e, "atualizar Professor com ID " + std::to_string(id));
    }
    throw;
}

// DESTROY: Deleta um registro pelo ID.
bool ProfessorController::destroy(long id) const {
    try {
        // Service retorna true (sucesso) ou false (ID não encontrado).
        return service.deleteById(id);
    } catch (const std::runtime_error& e) {
        // Captura I/O ou erros críticos.
        handle_controller_exception(
            e, "excluir Professor com ID " + std::to_string(id));
    }
    throw;
}