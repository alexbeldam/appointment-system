#include "controller/loginController.hpp"

#include <memory>

#include "event/events.hpp"
#include "util/utils.hpp"
using namespace std;

LoginController::LoginController(const AlunoService& alunoService,
                                 const ProfessorService& professorService,
                                 EventBus& bus)
    : alunoService(alunoService),
      professorService(professorService),
      bus(bus) {}

// Tenta autenticar um Aluno.
Aluno LoginController::loginAluno(string email, string senha) const {
    try {
        // 1. Busca o Aluno.
        optional<Aluno> opt_aluno = alunoService.getOneByEmail(email);

        if (!opt_aluno.has_value()) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        Aluno aluno = move(opt_aluno.value());

        // 2. Verifica a senha.
        if (!check(aluno.getSenha(), senha)) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        // 3. CRÍTICO: Cria o objeto no HEAP usando shared_ptr<Usuario>.
        // Isso garante: A) Posse compartilhada para o EventBus. B)
        // Polimorfismo. O objeto 'aluno' é movido para o heap (sem cópia).
        shared_ptr<Usuario> user_ptr = make_shared<Aluno>(move(aluno));

        // 4. Publica o evento de sucesso de login.
        // O SessionManager está inscrito e irá reagir, salvando o user_ptr.
        bus.publish(UsuarioLoggedInEvent(user_ptr));

        // 5. Retorna a CÓPIA do objeto Aluno (valor) para o chamador.
        // A cópia é feita a partir do shared_ptr, que mantém o objeto vivo.
        return static_cast<Aluno&>(*user_ptr);
    } catch (const std::invalid_argument& e) {
        handle_controller_exception(e, "validar autenticação");

        throw;
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "resgatar aluno do serviço");

        throw;
    }
}

// Tenta autenticar um Professor. O fluxo é idêntico ao de Aluno.
Professor LoginController::loginProfessor(string email, string senha) const {
    try {
        // 1. Busca o Professor.
        optional<Professor> opt_professor =
            professorService.getOneByEmail(email);

        if (!opt_professor.has_value()) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        Professor professor = move(opt_professor.value());

        // 2. Verifica a senha.
        if (!check(professor.getSenha(), senha)) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        // 3. CRÍTICO: Cria o objeto no HEAP com shared_ptr<Usuario>.
        shared_ptr<Usuario> user_ptr = make_shared<Professor>(move(professor));

        // 4. Publica o evento de sucesso de login.
        bus.publish(UsuarioLoggedInEvent(user_ptr));

        // 5. Retorna a CÓPIA do objeto Professor (valor) para o chamador.
        return static_cast<Professor&>(*user_ptr);
    } catch (const std::invalid_argument& e) {
        handle_controller_exception(e, "validar autenticação");

        throw;
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "resgatar professor do serviço");

        throw;
    }
}