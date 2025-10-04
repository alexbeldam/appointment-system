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

Aluno AlunoController::create(const string& nome, const string& email,
                              const string& senha, long matricula) {
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
                "A senha deve conter apenas letras e números (caracteres "
                "especiais não são permitidos).");
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

Aluno AlunoController::read(long id) {
    try {
        return service.getById(id);
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e,
                                    "ler Aluno pelo ID " + std::to_string(id));
    }
    throw;
}

vector<Aluno> AlunoController::list() {
    try {
        return service.listAll();
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "listar todos os Alunos");
    }
    throw;
}

Aluno AlunoController::update(long id, const string& nome, const string& email,
                              const string& senha, long matricula) {
    try {
        Aluno existingAluno = service.getById(id);

        if (nome.empty() || email.empty() || matricula <= 0) {
            throw std::invalid_argument(
                "Nome, email e matrícula são obrigatórios para a atualização.");
        }

        AlunoDTO dto;
        dto.setId(id);
        dto.setEmail(email);
        dto.setNome(nome);
        dto.setMatricula(matricula);

        if (senha.empty()) {
            dto.setSenha(existingAluno.getSenha());
        } else {
            if (senha.length() < 4) {
                throw std::invalid_argument(
                    "A nova senha deve ter pelo menos 4 caracteres.");
            }
            if (!is_alphanumeric(senha)) {
                throw std::invalid_argument(
                    "A nova senha deve conter apenas letras e números "
                    "(caracteres especiais não são permitidos).");
            }
            dto.setSenha(encrypt(senha));
        }

        return service.updateById(id, dto);
    } catch (const std::invalid_argument& e) {
        handle_controller_exception(
            e, "validar dados de atualização para o ID " + std::to_string(id));
    } catch (const std::runtime_error& e) {
        handle_controller_exception(
            e, "atualizar Aluno com ID " + std::to_string(id));
    }
    throw;
}

bool AlunoController::destroy(long id) {
    try {
        return service.deleteById(id);
    } catch (const std::runtime_error& e) {
        handle_controller_exception(
            e, "excluir Aluno com ID " + std::to_string(id));
    }
    throw;
}