#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>

/**
 * @brief Alias de tipo para representar um ponto no tempo.
 */
using Timestamp = std::time_t;

/**
 * @brief Função utilitária para lidar com exceções na camada Controller.
 * * Imprime uma mensagem de erro formatada no stderr e relança a exceção.
 * @param e A exceção capturada.
 * @param acao Uma descrição da ação que falhou (ex: "realizar login").
 */
inline void handle_controller_exception(const std::exception& e,
                                        const std::string& acao) {
    std::cerr << "\n[ERRO] Falha ao " << acao << ": " << e.what() << std::endl;
    throw;
}

/**
 * @brief Verifica se uma string contém apenas caracteres alfanuméricos (letras
 * e números).
 * * Retorna falso se a string for vazia.
 * @param s A string a ser verificada.
 * @return bool True se a string for não vazia e contiver apenas caracteres
 * alfanuméricos.
 */
inline bool is_alphanumeric(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) {
        return std::isalnum(static_cast<unsigned char>(c));
    });
}

/**
 * @brief Simula o processo de hashing de senha no estilo bcrypt.
 * * Implementado usando o std::hash
 * @param pwd A senha em texto claro.
 * @param cost_factor O fator de custo (simulado), padrão é 12.
 * @return std::string A senha "hasheada" (cyphertext) simulada.
 */
std::string mock_bcrypt(const std::string& pwd, int cost_factor = 12);

/**
 * @brief Verifica se uma senha em texto claro corresponde a um hash simulado.
 * * @param cypher O hash simulado.
 * @param pwd A senha em texto claro (fornecida pelo usuário).
 * @return bool True se a senha corresponder ao hash simulado.
 */
bool check(const std::string& cypher, const std::string& pwd);

/**
 * @brief Solicita ao usuário que insira um número inteiro dentro de um
 * intervalo específico.
 * * Lida com entrada inválida e garante que o valor esteja entre min_val e
 * max_val.
 * @param prompt A mensagem a ser exibida ao usuário.
 * @param min_val O valor mínimo aceitável.
 * @param max_val O valor máximo aceitável.
 * @return int O valor inteiro válido inserido pelo usuário.
 */
int read_integer_range(const std::string& prompt, int min_val, int max_val);

/**
 * @brief Desenha um relógio no
 * console.
 */
void desenhar_relogio();

/**
 * @brief Converte um Timestamp (std::time_t) em uma string de data e hora
 * formatada.
 * @param tt O timestamp a ser formatado.
 * @return std::string A representação do timestamp como string.
 */
std::string timestamp_to_string(Timestamp tt);

/**
 * @brief Converte uma string de data e hora em um Timestamp (std::time_t).
 * * A string deve estar no formato esperado (dd/mm HH:MM).
 * @param date_str A string de data e hora.
 * @return Timestamp O valor de std::time_t correspondente.
 */
Timestamp string_to_timestamp(const std::string& date_str);

#endif