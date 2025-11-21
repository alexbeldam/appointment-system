#include "controller/loginController.hpp"

#include <memory>

#include "event/events.hpp"
#include "util/utils.hpp"
using namespace std;

LoginController::LoginController(
    const shared_ptr<AlunoService>& alunoService,
    const shared_ptr<ProfessorService>& professorService, EventBus& bus)
    : alunoService(alunoService),
      professorService(professorService),
      bus(bus) {}

// Tenta autenticar um Aluno.
shared_ptr<Aluno> LoginController::loginAluno(string email, string senha) {
    try {
        // 1. Busca o Aluno.
        auto aluno = alunoService->getOneByEmail(email);

        if (!aluno) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        // 2. Verifica a senha.
        if (!check(aluno->getSenha(), senha)) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        bus.publish(AlunoLoggedInEvent(aluno->getId()));

        return aluno;
    } catch (const invalid_argument& e) {
        handle_controller_exception(e, "validar autenticação");

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "resgatar aluno do serviço");

        throw;
    }
}

// Tenta autenticar um Professor. O fluxo é idêntico ao de Aluno.
shared_ptr<Professor> LoginController::loginProfessor(string email,
                                                      string senha) {
    try {
        // 1. Busca o Professor.
        auto professor = professorService->getOneByEmail(email);

        if (!professor) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        // 2. Verifica a senha.
        if (!check(professor->getSenha(), senha)) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        bus.publish(ProfessorLoggedInEvent(professor->getId()));

        return professor;
    } catch (const invalid_argument& e) {
        handle_controller_exception(e, "validar autenticação");

        throw;
    } catch (const runtime_error& e) {
        handle_controller_exception(e, "resgatar professor do serviço");

        throw;
    }
}