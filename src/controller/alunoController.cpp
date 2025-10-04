#include "controller/alunoController.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

#include "helper/encrypt.hpp"
using namespace std;

void handle_controller_exception(const std::exception& e,
                                 const std::string& acao) {
    std::cerr << "\n[ERRO] Falha ao " << acao << ": " << e.what() << std::endl;
    throw;
}

bool is_alphanumeric(const std::string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), [](char c) {
        return ::isalnum(static_cast<unsigned char>(c));
    });
}

AlunoController::AlunoController(const AlunoService& service)
    : service(service) {}

Aluno AlunoController::create(const string& nome, const string& email,
                              const string& senha, long matricula) const {
    try {
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

        AlunoDTO dto;
        dto.setEmail(email);
        dto.setNome(nome);
        dto.setSenha(encrypt(senha));
        dto.setMatricula(matricula);

        return service.save(dto);
    } catch (const std::invalid_argument& e) {
        handle_controller_exception(e, "validar novos dados do Aluno");
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "criar Aluno no serviço");
    }
    throw;
}

Aluno AlunoController::read(long id) const {
    try {
        optional<Aluno> aluno_opt = service.getById(id);

        if (!aluno_opt.has_value()) {
            throw std::invalid_argument("Aluno com ID " + std::to_string(id) +
                                        " não encontrado.");
        }

        return aluno_opt.value();

    } catch (const std::invalid_argument& e) {
        handle_controller_exception(e,
                                    "ler Aluno pelo ID " + std::to_string(id));

    } catch (const std::runtime_error& e) {
        handle_controller_exception(e,
                                    "ler Aluno pelo ID " + std::to_string(id));
    }
    throw;
}

vector<Aluno> AlunoController::list() const {
    try {
        return service.listAll();
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "listar todos os Alunos");
    }
    throw;
}

Aluno AlunoController::update(long id, const string& nome, const string& email,
                              const string& senha, long matricula) const {
    try {
        optional<Aluno> existingAluno_opt = service.getById(id);

        if (!existingAluno_opt.has_value()) {
            throw std::invalid_argument("Aluno com ID " + std::to_string(id) +
                                        " não encontrado para atualização.");
        }
        Aluno existingAluno = existingAluno_opt.value();

        AlunoDTO dto;
        dto.setId(id);

        string newNome = nome.empty() ? existingAluno.getNome() : nome;
        if (newNome.length() < 3) {
            throw std::invalid_argument(
                "O nome deve ter pelo menos 3 caracteres.");
        }
        dto.setNome(newNome);

        string newEmail = email.empty() ? existingAluno.getEmail() : email;
        if (newEmail.find('@') == std::string::npos ||
            newEmail.find('.') == std::string::npos) {
            throw std::invalid_argument("Formato de email inválido.");
        }
        dto.setEmail(newEmail);

        if (senha.empty()) {
            dto.setSenha(existingAluno.getSenha());
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

        long newMatricula =
            (matricula <= 0) ? existingAluno.getMatricula() : matricula;

        dto.setMatricula(newMatricula);

        optional<Aluno> updatedAluno_opt = service.updateById(id, dto);

        if (!updatedAluno_opt.has_value()) {
            throw std::runtime_error(
                "O registro foi encontrado, mas a atualização falhou de forma "
                "inesperada.");
        }

        return updatedAluno_opt.value();

    } catch (const std::invalid_argument& e) {
        handle_controller_exception(
            e, "validar dados de atualização para o ID " + std::to_string(id));
    } catch (const std::runtime_error& e) {
        handle_controller_exception(
            e, "atualizar Aluno com ID " + std::to_string(id));
    }
    throw;
}

bool AlunoController::destroy(long id) const {
    try {
        return service.deleteById(id);
    } catch (const std::runtime_error& e) {
        handle_controller_exception(
            e, "excluir Aluno com ID " + std::to_string(id));
    }
    throw;
}
