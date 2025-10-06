#include "controller/loginController.hpp"

#include "util/utils.hpp"
using namespace std;

LoginController::LoginController(const AlunoService& alunoService,
                                 const ProfessorService& professorService,
                                 SessionManager& sessionManager)
    : alunoService(alunoService),
      professorService(professorService),
      sessionManager(sessionManager) {}

// Implementação do login de Aluno.
// O retorno é por VALOR (Aluno), o que exige que retornemos uma cópia,
// mas o objeto original logado é movido para o heap e entregue ao
// SessionManager.
Aluno LoginController::loginAluno(string email, string senha) const {
    try {
        // 1. Busca o Aluno pelo email. O Service retorna o objeto por valor
        // (dentro
        // de optional).
        optional<Aluno> opt_aluno = alunoService.getOneByEmail(email);

        if (!opt_aluno.has_value()) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        // Move o objeto Aluno do optional para a variável local 'aluno'.
        // Neste ponto, 'aluno' está na Stack do Controller.
        Aluno aluno = move(opt_aluno.value());

        // 2. Verifica a senha.
        if (!check(aluno.getSenha(), senha)) {
            throw invalid_argument("Senha e/ou email inválidos.");
        }

        // --- Início da Gestão de Propriedade (Ownership) ---

        // 3. CRÍTICO: Cria o objeto no HEAP e o armazena em um
        // unique_ptr<Usuario>. O objeto 'aluno' é movido (seus dados são
        // transferidos) para a nova memória no heap. Isso é feito para: A)
        // Garantir que o objeto sobreviva ao fim desta função. B) Habilitar o
        // polimorfismo (upcast implícito de Aluno para Usuario).
        unique_ptr<Usuario> user_ptr = make_unique<Aluno>(move(aluno));

        // 4. Salva a cópia de retorno.
        // O chamador espera um retorno por valor (cópia). Criamos a cópia ANTES
        // de transferir a propriedade do ponteiro.
        // Usamos static_cast para garantir que a referência seja tratada como
        // Aluno, pegando todos os dados da classe derivada.
        Aluno aluno_return_copy = static_cast<Aluno&>(*user_ptr);

        // 5. Sucesso: Transfere a propriedade (Ownership) para o
        // SessionManager. O std::move transfere o unique_ptr. Após esta linha,
        // a SessionManager é a única responsável por deletar o objeto.
        sessionManager.login(move(user_ptr));

        // Retorna a cópia do objeto Aluno (valor) para o chamador.
        return aluno_return_copy;
    } catch (const std::invalid_argument& e) {
        handle_controller_exception(e, "validar autenticação");
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "resgatar aluno do serviço");
    }
    throw;
}

// Implementação do login de Professor.
// O fluxo é idêntico ao de Aluno, garantindo a gestão correta da propriedade.
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

        // 3. CRÍTICO: Cria o objeto no HEAP com unique_ptr<Usuario>.
        unique_ptr<Usuario> user_ptr = make_unique<Professor>(move(professor));

        // 4. Salva a cópia de retorno ANTES da transferência.
        Professor professor_return_copy = static_cast<Professor&>(*user_ptr);

        // 5. Sucesso: Transfere a propriedade (Ownership) para o
        // SessionManager.
        sessionManager.login(move(user_ptr));

        // Retorna a cópia do objeto Professor (valor) para o chamador.
        return professor_return_copy;
    } catch (const std::invalid_argument& e) {
        handle_controller_exception(e, "validar autenticação");
    } catch (const std::runtime_error& e) {
        handle_controller_exception(e, "resgatar professor do serviço");
    }
    throw;
}