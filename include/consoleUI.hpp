#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "appComposer.hpp"

/**
 * @brief Classe da Interface de Usuário (Presentation Layer) para aplicações de
 * console.
 *
 * Esta classe é responsável por gerenciar toda a interação com o usuário (I/O
 * via cin/cout), apresentar menus e gerenciar o loop principal da aplicação.
 * Delega todas as ações de negócio e persistência aos Controllers injetados
 * através do AppComposer.
 */
class ConsoleUI {
   private:
    /**
     * @brief O objeto Compositor da Aplicação.
     * * Armazena todas as dependências (Controllers, Services) necessárias para
     * executar as funcionalidades do sistema.
     */
    const AppComposer& app;

    /**
     * @brief Imprime um relógio.
     *
     */
    void desenhar_relogio() const;

    /**
     * @brief Imprime a tela de login no console.
     */
    void imprimir_login() const;

    /**
     * @brief Imprime o menu de opções disponíveis para o usuário no console.
     */
    void imprimir_opcoes() const;

    /**
     * @brief Gerencia a interação completa de I/O para a criação de um novo
     * Aluno.
     * * Coleta os dados brutos e delega a operação de persistência ao
     * AlunoController.
     */
    void criar_aluno() const;

   public:
    /**
     * @brief Construtor da Interface.
     * * Inicializa a UI com o objeto AppComposer, garantindo acesso a todos os
     * Controllers.
     * @param app O objeto Compositor que contém todas as dependências
     * injetadas.
     */
    ConsoleUI(const AppComposer& app);

    /**
     * @brief Inicia o loop principal da aplicação de console.
     * * Gerencia a leitura de opções do menu e o fluxo de execução para os
     * métodos da UI.
     */
    void run() const;
};

#endif