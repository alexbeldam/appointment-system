#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "service/sessionService.hpp"

/**
 * @brief Classe base abstrata para todas as interfaces de usuário de console
 * (UI).
 * * Define a estrutura comum e as dependências mínimas necessárias para
 * qualquer componente de interface do sistema, como o SessionService.
 */
class ConsoleUI {
   protected:
    /**
     * @brief Ponteiro inteligente para o serviço de sessão.
     * * Usado pelas subclasses para verificar o estado de login e realizar
     * operações de logout.
     */
    const std::shared_ptr<SessionService>& sessionService;

    /**
     * @brief Executa a operação de logout no SessionService.
     */
    void fazer_logout();

   public:
    /**
     * @brief Construtor da classe ConsoleUI.
     * * Injeta a dependência do SessionService.
     * @param ss O ponteiro inteligente para o SessionService.
     */
    ConsoleUI(const std::shared_ptr<SessionService>& ss);

    /**
     * @brief Destrutor virtual padrão.
     * * Essencial para garantir a desalocação correta das classes derivadas.
     */
    virtual ~ConsoleUI() = default;

    /**
     * @brief Método virtual puro que exibe a interface e processa a interação
     * do usuário.
     * * Deve ser implementado por todas as subclasses.
     * @return bool Retorna true para continuar no menu/interface atual; false
     * para sair ou encerrar.
     */
    virtual bool show() = 0;
};

#endif