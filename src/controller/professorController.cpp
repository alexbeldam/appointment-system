#include "controller/professorController.hpp"

#include "util/utils.hpp"

using std::invalid_argument;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;

ProfessorController::ProfessorController(
    const shared_ptr<ProfessorService>& service)
    : service(service) {}

shared_ptr<Professor> ProfessorController::create(const string& nome,
                                                  const string& email,
                                                  const string& senha,
                                                  const string& disciplina) {
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
        if (disciplina.empty() || disciplina.length() < 3) {
            throw invalid_argument(
                "A disciplina deve ter pelo menos 3 caracteres.");
        }

        return service->save(nome, email, mock_bcrypt(senha), disciplina);

    } catch (const invalid_argument& e) {
        handle_controller_exception(e, "validar novos dados do Professor");

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "criar Professor no serviço");

        throw;
    }
}

shared_ptr<Professor> ProfessorController::read(long id) {
    try {
        return service->getById(id);
    } catch (const invalid_argument& e) {
        handle_controller_exception(e,
                                    "ler Professor pelo ID " + to_string(id));

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e,
                                    "ler Professor pelo ID " + to_string(id));

        throw;
    }
}

vector<shared_ptr<Professor>> ProfessorController::list() {
    try {
        return service->listAll();
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "listar todos os Professors");

        throw;
    }
}

shared_ptr<Professor> ProfessorController::update(long id, const string& nome,
                                                  const string& email,
                                                  const string& senha,
                                                  const string& disciplina) {
    try {
        auto late = service->getById(id);

        if (!late) {
            throw invalid_argument("Professor com ID " + to_string(id) +
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

        string newDisciplina =
            disciplina.empty() ? late->getDisciplina() : disciplina;
        if (newDisciplina.length() < 3) {
            throw invalid_argument("O nome deve ter pelo menos 3 caracteres.");
        }

        auto updated =
            service->updateById(id, newNome, newEmail, newSenha, newDisciplina);

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
        handle_controller_exception(
            e, "atualizar Professor com ID " + to_string(id));

        throw;
    }
}

bool ProfessorController::destroy(long id) {
    try {
        return service->deleteById(id);
    } catch (const runtime_error& e) {
        handle_controller_exception(
            e, "excluir Professor com ID " + to_string(id));

        throw;
    }
}