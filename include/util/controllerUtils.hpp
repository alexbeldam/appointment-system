#ifndef CONTROLLER_UTILS_HPP
#define CONTROLLER_UTILS_HPP

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

// Funções utilitárias

// Função que padroniza o tratamento de exceções no Controller (Captura, Loga,
// Relança).
inline void handle_controller_exception(const std::exception& e,
                                        const std::string& acao) {
    std::cerr << "\n[ERRO] Falha ao " << acao << ": " << e.what() << std::endl;
    throw;
}

// Verifica se a string contém apenas caracteres alfanuméricos.
// Esta é uma utilidade de saneamento de entrada.
inline bool is_alphanumeric(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) {
        return std::isalnum(static_cast<unsigned char>(c));
    });
}

#endif