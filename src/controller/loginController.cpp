#include "controller/loginController.hpp"

#include "event/events.hpp"
#include "util/utils.hpp"

using std::invalid_argument;
using std::runtime_error;
using std::shared_ptr;
using std::string;

LoginController::LoginController(
    const shared_ptr<AlunoService>& alunoService,
    const shared_ptr<ProfessorService>& professorService, EventBus& bus)
    : alunoService(alunoService),
      professorService(professorService),
      bus(bus) {}

shared_ptr<Aluno> LoginController::loginAluno(string email, string senha) {
    try {
        auto aluno = alunoService->getOneByEmail(email);

        if (!aluno) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

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

shared_ptr<Professor> LoginController::loginProfessor(string email,
                                                      string senha) {
    try {
        auto professor = professorService->getOneByEmail(email);

        if (!professor) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

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