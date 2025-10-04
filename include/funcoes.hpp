#ifndef FUNCOES_HPP
#define FUNCOES_HPP

#include "controller/alunoController.hpp"

/**
 * @brief Desenha ou exibe a representação visual de um relógio/hora atual na
 * tela.
 */
void desenhar_relogio();

/**
 * @brief Imprime a tela ou cabeçalho de login no console.
 * * Usado para iniciar a sessão do usuário no sistema.
 */
void imprimir_login();

/**
 * @brief Imprime o menu de opções disponíveis para o usuário no console.
 * * Usado para navegar pelas funcionalidades do sistema.
 */
void imprimir_opcoes();

/**
 * @brief Gerencia o fluxo de I/O para a criação de um novo Aluno.
 * * Esta função coleta os dados brutos do usuário e delega a operação
 * à camada de Controller, tratando as exceções de validação e sistema.
 * * @param controller Uma referência constante para o AlunoController.
 */
void criar_aluno(const AlunoController& controller);

#endif