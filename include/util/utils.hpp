#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>

using Timestamp = std::time_t;

/**
 * @brief Padroniza o tratamento de exceções no Controller (Captura, Loga,
 * Relança).
 *
 * Esta função é utilizada para centralizar o tratamento de exceções,
 * registrando uma mensagem de erro no console (stderr) e relançando a exceção
 * original para que o tratamento de nível superior (p. ex., o Controller) possa
 * reagir ou encerrar a operação.
 *
 * @param e A exceção capturada (por referência constante).
 * @param acao Uma string que descreve a ação que falhou (ex: "salvar usuário",
 * "carregar dados").
 * @throws Relança a exceção 'e' capturada.
 */
inline void handle_controller_exception(const std::exception& e,
                                        const std::string& acao) {
    std::cerr << "\n[ERRO] Falha ao " << acao << ": " << e.what() << std::endl;
    throw;
}

/**
 * @brief Verifica se a string contém apenas caracteres alfanuméricos.
 *
 * Esta é uma utilidade de saneamento de entrada. Uma string vazia
 * não é considerada alfanumérica.
 *
 * @param s A string de entrada a ser verificada.
 * @return true se a string não estiver vazia e contiver *apenas*
 * letras (a-z, A-Z) e dígitos (0-9).
 * @return false caso contrário (string vazia ou contém caracteres especiais).
 */
inline bool is_alphanumeric(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) {
        return std::isalnum(static_cast<unsigned char>(c));
    });
}

std::string mock_bcrypt(const std::string& pwd, int cost_factor = 12);

bool check(const std::string& cypher, const std::string& pwd);

int read_integer_range(const std::string& prompt, int min_val, int max_val);

void desenhar_relogio();

std::string timestamp_to_string(Timestamp tt);

Timestamp string_to_timestamp(const std::string& date_str);

#endif