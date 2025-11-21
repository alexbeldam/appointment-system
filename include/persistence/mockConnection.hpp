#ifndef MOCK_CONNECTION_HPP
#define MOCK_CONNECTION_HPP

#include <string>
#include <vector>

/**
 * @brief Simula uma conexão de persistência.
 * * Esta classe fornece métodos que imitam as operações CRUD (Create, Read,
 * Update, Delete) de um banco de dados, mas manipula dados em arquivos CSV.
 */
class MockConnection {
   public:
    /**
     * @brief Insere um novo registro na "tabela" especificada. [SQL: INSERT]
     * * Simula a criação de um novo registro.
     * @param table_name O nome da tabela.
     * @param data A string contendo os dados do registro a ser inserido.
     * @return long O ID do novo registro inserido.
     */
    long insert(const std::string& table_name, const std::string& data) const;

    /**
     * @brief Seleciona e retorna um único registro pelo seu ID. [SQL: SELECT]
     * * Simula a busca por chave primária.
     * @param table_name O nome da tabela.
     * @param id O identificador único do registro.
     * @return std::string A string contendo os dados do registro encontrado,
     * ou uma string vazia se não for encontrado.
     */
    std::string selectOne(const std::string& table_name, long id) const;

    /**
     * @brief Seleciona e retorna vários registros que correspondem a um valor
     * em uma coluna. [SQL: SELECT]
     * * Simula uma cláusula WHERE baseada em uma coluna específica.
     * @param table_name O nome da tabela.
     * @param index O índice da coluna (simulada) para a busca.
     * @param value O valor a ser comparado na coluna.
     * @return std::vector<std::string> Um vetor de strings contendo os
     * registros encontrados.
     */
    std::vector<std::string> selectByColumn(const std::string& table_name,
                                            size_t index,
                                            const std::string& value) const;

    /**
     * @brief Seleciona e retorna todos os registros de uma tabela. [SQL:
     * SELECT]
     * * @param table_name O nome da tabela.
     * @return std::vector<std::string> Um vetor de strings contendo todos os
     * registros.
     */
    std::vector<std::string> selectAll(const std::string& table_name) const;

    /**
     * @brief Atualiza um registro existente pelo seu ID. [SQL: UPDATE]
     * * @param table_name O nome da tabela.
     * @param id O identificador único do registro a ser atualizado.
     * @param data A string contendo os novos dados do registro.
     */
    void update(const std::string& table_name, long id,
                const std::string& data) const;

    /**
     * @brief Exclui um único registro pelo seu ID. [SQL: DELETE]
     * * @param table_name O nome da tabela.
     * @param id O identificador único do registro a ser excluído.
     */
    void deleteRecord(const std::string& table_name, long id) const;

    /**
     * @brief Exclui registros que correspondem a um valor em uma coluna. [SQL:
     * DELETE]
     * * Simula uma cláusula WHERE para exclusão.
     * @param table_name O nome da tabela.
     * @param index O índice da coluna para o critério de exclusão.
     * @param value O valor a ser comparado na coluna para determinar quais
     * registros excluir.
     * @return size_t O número de registros excluídos.
     */
    size_t deleteByColumn(const std::string& table_name, size_t index,
                          const std::string& value) const;
};

/**
 * @brief Função utilitária para extrair o ID de uma linha de dados (registro).
 * * Assume que o ID é o primeiro campo em uma linha formatada.
 * @param line A string que representa um registro de dados.
 * @return long O ID extraído.
 */
long getIdFromLine(const std::string& line);

#endif