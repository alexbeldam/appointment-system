#ifndef MOCK_HPP
#define MOCK_HPP

#include <string>
#include <vector>

/**
 * @brief Insere um novo registro de dados no arquivo.
 * * * * A função lê o arquivo para determinar o maior ID existente,
 * incrementa-o e anexa a nova linha (ID,data) ao arquivo.
 * * Lança std::runtime_error em caso de falha de I/O.
 * * @param filename O caminho do arquivo CSV (caminho do mock DB).
 * @param data A string CSV contendo os dados do registro (excluindo o ID).
 * @return O novo ID gerado para o registro inserido.
 */
long insert(const std::string& filename, const std::string& data);

/**
 * @brief Busca um registro específico pelo ID (coluna 0).
 * * * * Esta função garante que o ID seja único, retornando exatamente uma
 * linha.
 * * Lança std::invalid_argument se o ID não for encontrado.
 * * Lança std::runtime_error se mais de um registro for encontrado (falha de
 * integridade).
 * * @param filename O caminho do arquivo CSV.
 * @param id O ID do registro a ser buscado.
 * @return A linha completa do registro encontrado (ID,dados).
 */
std::string find(const std::string& filename, long id);

/**
 * @brief Busca todos os registros onde uma coluna específica corresponde a um
 * valor.
 * * * * Esta é uma função de busca genérica que pode retornar zero ou mais
 * resultados.
 * * Retorna um vetor vazio se nenhum registro for encontrado.
 * * Lança std::runtime_error em caso de falha de I/O.
 * * @param filename O caminho do arquivo CSV.
 * @param index O índice (base 0) da coluna a ser comparada.
 * @param value O valor a ser buscado na coluna.
 * @return Um vetor contendo todas as linhas CSV que satisfazem a condição.
 */
std::vector<std::string> findByColumn(const std::string& filename, size_t index,
                                      const std::string& value);

/**
 * @brief Lista todos os registros de dados no arquivo.
 * * * * Ignora a linha de cabeçalho (linha 0) do arquivo.
 * * @param filename O caminho do arquivo CSV.
 * @return Um vetor contendo todas as linhas de dados (excluindo o cabeçalho).
 */
std::vector<std::string> findAll(const std::string& filename);

/**
 * @brief Atualiza os dados de um registro existente.
 * * * * Lança std::invalid_argument se o ID do registro não for encontrado.
 * * Lança std::runtime_error em caso de falha de I/O.
 * * @param filename O caminho do arquivo CSV.
 * @param id O ID do registro a ser atualizado.
 * @param data A nova string CSV contendo os dados do registro (excluindo o ID).
 * @return void (lança exceção em caso de falha).
 */
void update(const std::string& filename, long id, const std::string& data);

/**
 * @brief Remove o registro associado ao ID fornecido.
 * * * * Lança std::invalid_argument se o ID do registro não for encontrado.
 * * Lança std::runtime_error em caso de falha de I/O.
 * * @param filename O caminho do arquivo CSV.
 * @param id O ID do registro a ser removido.
 * @return void (lança exceção em caso de falha).
 */
void remove(const std::string& filename, long id);

#endif