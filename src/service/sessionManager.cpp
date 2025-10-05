#include <stdexcept>

#include "service/sessionManager.hpp"

using namespace std;

// --- Implementação dos Métodos de Gerenciamento de Sessão ---

/**
 * Define o usuário que acabou de fazer login.
 * O SessionManager assume a PROPRIEDADE EXCLUSIVA do objeto Usuario alocado no
 * heap.
 */
void SessionManager::login(unique_ptr<Usuario> user) {
    // std::move transfere a propriedade exclusiva do ponteiro 'user' para o
    // membro 'loggedUser'.
    loggedUser = move(user);
}

/**
 * Encerra a sessão.
 */
void SessionManager::logout() {
    // Reseta o unique_ptr. Isso destrói o objeto no heap e garante que a
    // memória seja liberada.
    loggedUser.reset();
}

/**
 * Verifica se há um usuário ativo.
 * Para unique_ptr, a checagem é feita diretamente contra nullptr.
 */
bool SessionManager::isLogged() const {
    return (loggedUser != nullptr);
}

// --- Implementação dos Métodos de Checagem de Tipo (instanceof) ---

/**
 * Verifica se o usuário logado é da classe Aluno (ou uma classe derivada).
 * Utiliza dynamic_cast, que requer a checagem do ponteiro bruto.
 */
bool SessionManager::isAluno() const {
    if (!isLogged()) {
        return false;
    }

    // Tenta converter o ponteiro base (Usuario*) para o derivado (Aluno*).
    // O .get() retorna o ponteiro bruto que o dynamic_cast precisa.
    return (dynamic_cast<const Aluno*>(loggedUser.get()) != nullptr);
}

/**
 * Verifica se o usuário logado é da classe Professor.
 */
bool SessionManager::isProfessor() const {
    if (!isLogged()) {
        return false;
    }

    // Tenta o downcast: Se for um Professor, o ponteiro não será nulo.
    return (dynamic_cast<const Professor*>(loggedUser.get()) != nullptr);
}

// --- Implementação dos Métodos de Acesso de Valor Seguro ---

/**
 * Retorna uma cópia do objeto Professor logado.
 * Lança runtime_error se não estiver logado ou o tipo for incompatível.
 */
Professor SessionManager::getCurrentProfessor() const {
    // 1. Checagem de Sessão Ativa
    if (!isLogged()) {
        throw runtime_error("Não há usuário logado na sessão.");
    }

    // 2. Tentativa de Downcast
    // Tenta converter o ponteiro genérico (Usuario*) para o tipo específico
    // (Professor*).
    if (const Professor* professor_ptr =
            dynamic_cast<const Professor*>(loggedUser.get())) {
        // Cast bem-sucedido! Retorna uma CÓPIA completa do objeto Professor.
        // Isso evita o 'slicing'.
        return *professor_ptr;
    }

    // 3. Checagem de Tipo Incorreto
    // O usuário está logado, mas não é um Professor (é Aluno ou outro).
    throw runtime_error("O usuário logado não é um Professor.");
}

/**
 * Retorna uma cópia do objeto Aluno logado.
 * Lança runtime_error se não estiver logado ou o tipo for incompatível.
 */
Aluno SessionManager::getCurrentAluno() const {
    // 1. Checagem de Sessão Ativa
    if (!isLogged()) {
        throw runtime_error("Não há usuário logado na sessão.");
    }

    // 2. Tentativa de Downcast
    // Tenta converter o ponteiro genérico (Usuario*) para o tipo específico
    // (Aluno*).
    if (const Aluno* aluno_ptr = dynamic_cast<const Aluno*>(loggedUser.get())) {
        // Cast bem-sucedido! Retorna uma CÓPIA completa do objeto Aluno.
        return *aluno_ptr;
    }

    // 3. Checagem de Tipo Incorreto
    // O usuário está logado, mas não é um Aluno.
    throw runtime_error("O usuário logado não é um Aluno.");
}