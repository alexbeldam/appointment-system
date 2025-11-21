#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "service/sessionService.hpp"

/**
 * @brief Classe base para interfaces de usuário no console.
 *
 * Fornece funcionalidades comuns para todas as interfaces de usuário que
 * operam no console, incluindo gerenciamento de sessão.
 */
class ConsoleUI {
   protected:
    const std::shared_ptr<SessionService>& sessionService;

    /**
     * @brief Realiza o logout do usuário atual.
     */
    void fazer_logout();

   public:
    ConsoleUI(const std::shared_ptr<SessionService>& ss);

    virtual ~ConsoleUI() = default;

    /**
     * @brief Exibe a interface de usuário no console.
     *
     * @return true se a aplicação deve continuar rodando, false caso contrário.
     */
    virtual bool show() = 0;
};

#endif