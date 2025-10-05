#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include <string>

/**
 * @brief Criptografa uma string de texto.
 * * Utilizada para gerar uma representação segura de uma senha
 * de texto simples antes de armazená-la.
 * * @param pwd A string de texto simples a ser processada.
 * @return A string criptografada (cyphertext).
 */
std::string encrypt(const std::string& pwd);

/**
 * @brief Verifica se uma string de texto simples corresponde a uma string
 * criptografada.
 * * Usada para autenticar o usuário, comparando a senha fornecida com o
 * cyphertext armazenado.
 * * @param cypher A string criptografada (cyphertext) armazenada.
 * @param pwd A string de texto simples fornecida pelo usuário.
 * @return true se as strings coincidirem após a aplicação da
 * criptografia/verificação.
 * @return false caso contrário.
 */
bool check(const std::string& cypher, const std::string& pwd);

#endif