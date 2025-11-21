#include "controller/alunoController.hpp"

#include "util/utils.hpp"

using std::invalid_argument;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::to_string;

AlunoController::AlunoController(const shared_ptr<AlunoService>& service)
    : service(service) {}

shared_ptr<Aluno> AlunoController::create(const string& nome,
                                          const string& email,
                                          const string& senha, long matricula) {
    try {
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

    } catch (const invalid_argument& e) {
        handle_controller_exception(e, "validar novos dados do Aluno");

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "criar Aluno no serviço");

        throw;
    }
}

shared_ptr<Aluno> AlunoController::read(long id) {
    try {
        return service->getById(id);
    } catch (const invalid_argument& e) {
        handle_controller_exception(e, "ler Aluno pelo ID " + to_string(id));

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "ler Aluno pelo ID " + to_string(id));

        throw;
    }
}

shared_ptr<Aluno> AlunoController::update(long id, const string& nome,
                                          const string& email,
                                          const string& senha, long matricula) {
    try {
        auto late = service->getById(id);

        if (!late) {
            throw invalid_argument("Aluno com ID " + to_string(id) +
                                   " não encontrado para atualização.");
        }

        string newNome = nome.empty() ? late->getNome() : nome;
        if (newNome.length() < 3) {
            throw invalid_argument("O nome deve ter pelo menos 3 caracteres.");
        }

        string newEmail = email.empty() ? late->getEmail() : email;
        if (newEmail.find('@') == string::npos ||
            newEmail.find('.') == string::npos) {
            throw invalid_argument("Formato de email inválido.");
        }

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

        long newMatricula = (matricula <= 0) ? late->getMatricula() : matricula;

        auto updated =
            service->updateById(id, newNome, newEmail, newSenha, newMatricula);

        if (!updated) {
            throw runtime_error(
                "O registro foi encontrado, mas a atualização falhou de forma "
                "inesperada.");
        }

        return updated;
    } catch (const invalid_argument& e) {
        handle_controller_exception(
            e, "validar dados de atualização para o ID " + to_string(id));

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e,
                                    "atualizar Aluno com ID " + to_string(id));

        throw;
    }
}

bool AlunoController::destroy(long id) {
    try {
        return service->deleteById(id);
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "excluir Aluno com ID " + to_string(id));
        throw;
    }
}