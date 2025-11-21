#ifndef MOCK_CONNECTION_HPP
#define MOCK_CONNECTION_HPP

#include <string>
#include <vector>

/**
 * @brief Simula uma conexão de banco de dados genérica (DAL).
 *
 * Esta classe é responsável por todas as operações de I/O em arquivos CSV,
 * tratando cada arquivo como uma "tabela".
 * * * Esta classe será injetada nos Services (e.g., AlunoService), permitindo
 * que o Service especifique a tabela (arquivo CSV) em cada operação.
 */
class MockConnection {
   public:
    /**
     * @brief [SQL: INSERT] Insere um novo registro na tabela (arquivo CSV)
     * especificada.
     * * @param table_name O nome da "tabela" (caminho do arquivo CSV).
     * @param data A string CSV contendo os dados do registro (excluindo o ID).
     * @return O novo ID gerado para o registro inserido.
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    long insert(const std::string& table_name, const std::string& data) const;

    /**
     * @brief [SQL: SELECT] Busca um único registro pelo ID na tabela
     * especificada.
     * * @param table_name O nome da "tabela" (caminho do arquivo CSV).
     * @param id O ID do registro a ser buscado.
     * @return A linha completa do registro encontrado (ID,dados).
     * @throws std::invalid_argument Se o ID não for encontrado ou se houver
     * duplicidade.
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    std::string selectOne(const std::string& table_name, long id) const;

    /**
     * @brief [SQL: SELECT] Busca registros na tabela especificada onde uma
     * coluna corresponde a um valor.
     * * @param table_name O nome da "tabela" (caminho do arquivo CSV).
     * @param index O índice (base 0) da coluna a ser comparada.
     * @param value O valor a ser buscado na coluna.
     * @return Um vetor contendo todas as linhas CSV que satisfazem a condição
     * (pode ser vazio).
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    std::vector<std::string> selectByColumn(const std::string& table_name,
                                            size_t index,
                                            const std::string& value) const;

    /**
     * @brief [SQL: SELECT ALL] Lista todos os registros na tabela especificada.
     * * @param table_name O nome da "tabela" (caminho do arquivo CSV).
     * @return Um vetor contendo todas as linhas de dados (excluindo o
     * cabeçalho).
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    std::vector<std::string> selectAll(const std::string& table_name) const;

    /**
     * @brief [SQL: UPDATE] Atualiza os dados de um registro existente na tabela
     * especificada.
     * * @param table_name O nome da "tabela" (caminho do arquivo CSV).
     * @param id O ID do registro a ser atualizado.
     * @param data A nova string CSV contendo os dados do registro (excluindo o
     * ID).
     * @throws std::invalid_argument Se o ID do registro não for encontrado.
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    void update(const std::string& table_name, long id,
                const std::string& data) const;

    /**
     * @brief [SQL: DELETE] Remove o registro associado ao ID fornecido na
     * tabela especificada.
     * * @param table_name O nome da "tabela" (caminho do arquivo CSV).
     * @param id O ID do registro a ser removido.
     * @throws std::invalid_argument Se o ID do registro não for encontrado.
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    void deleteRecord(const std::string& table_name, long id) const;

    /**
     * @brief [SQL: DELETE] Remove o registro onde uma
     * coluna corresponde a um valor.
     * @param index O índice (base 0) da coluna a ser comparada.
     * @param value O valor a ser buscado na coluna.
     * @return O número de linhas deletadas.
     * @throws std::runtime_error Em caso de falha de I/O.
     */
    size_t deleteByColumn(const std::string& table_name, size_t index,
                          const std::string& value) const;
};

long getIdFromLine(const std::string& line);

#endif