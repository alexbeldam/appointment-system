#include "data/mockConnection.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

// --- CONFIGURAÇÃO E UTILS DE ARQUIVO ---

// Define o prefixo do diretório base onde todos os arquivos/tabelas estão
// localizados.
#define DATA_PATH_PREFIX "data/"

// Constrói o caminho completo do arquivo (tabela) a partir do nome base.
// Ex: "alunos" -> "data/alunos.csv"
string getFullFilePath(const string& table_name) {
    return DATA_PATH_PREFIX + table_name + ".csv";
}

// Utilitário: Extrai o conteúdo de uma coluna específica de uma linha CSV.
// Usado internamente por todos os métodos que buscam/validam dados.
string extractColumnFromLine(const string& line, size_t col) {
    stringstream ss(line);
    string segment;
    size_t current_col = 0;

    // Tokeniza a linha usando a vírgula como delimitador.
    while (getline(ss, segment, ',')) {
        if (current_col == col) {
            return segment;  // Coluna encontrada.
        }
        current_col++;
    }
    // Lança exceção se o índice da coluna for maior que o número total de
    // colunas.
    throw invalid_argument("Coluna " + to_string(col) +
                           " não existe na linha.");
}

// Utilitário: Extrai e converte o ID (coluna 0) para long.
long extractIdFromLine(const string& line) {
    string id_str;
    try {
        // Assume que a coluna 0 é o ID e que existe.
        id_str = extractColumnFromLine(line, 0);
        return stol(id_str);  // Converte para long.
    } catch (const invalid_argument& e) {
        // Captura falha na extração.
        throw invalid_argument("A linha não contém um ID válido na coluna 0.");
    } catch (const exception& e) {
        // Captura falha na conversão numérica (stol).
        throw invalid_argument("O valor do ID '" + id_str +
                               "' não é um número válido.");
    }
}

// Utilitário: Lê todas as linhas do arquivo e trata CRLF.
vector<string> readAllLines(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    if (!file.is_open()) {
        return lines;
    }

    string line;
    while (getline(file, line)) {
        // Remove '\r' se for encontrado (compatibilidade Windows/CRLF).
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

// Utilitário: Sobrescreve o arquivo inteiro (usado para UPDATE/DELETE).
void writeAllLines(const string& filename, const vector<string>& lines) {
    // ios::trunc garante que o arquivo seja limpo antes da escrita.
    ofstream file(filename, ios::trunc);

    if (!file.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo para escrita: '" +
                            filename + "'.");
    }
    for (const string& line : lines) {
        file << line << "\n";
    }
}

// --- MÉTODOS DA CLASSE MOCKCONNECTION (OO/SQL) ---

// [SQL: INSERT] Insere um novo registro e retorna o ID gerado.
long MockConnection::insert(const string& table_name,
                            const string& data) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    long max_id = 0;

    // 1. Geração de ID: Itera para encontrar o maior ID existente (ignora
    // cabeçalho).
    for (size_t i = 1; i < lines.size(); ++i) {
        try {
            long current_id = extractIdFromLine(lines[i]);
            if (current_id > max_id) {
                max_id = current_id;
            }
        } catch (const invalid_argument& ignore) {
            // Ignora registros malformados ao buscar o max_id.
        }
    }

    long new_id = max_id + 1;
    string new_record;

    // 2. Formatação e Escrita: Formata a nova linha (ID,data).
    if (data.empty()) {
        new_record = to_string(new_id);
    } else {
        new_record = to_string(new_id) + "," + data;
    }

    // Tenta anexar a nova linha ao final do arquivo.
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo '" + filename +
                            "' para anexar.");
    }
    file << new_record << "\n";

    // Retorna o novo ID para o Service.
    return new_id;
}

// [SQL: SELECT ONE] Busca um único registro pelo ID, impondo unicidade.
string MockConnection::selectOne(const string& table_name, long id) const {
    // A busca é feita chamando o método genérico de busca por coluna.
    vector<string> lines = selectByColumn(table_name, 0, to_string(id));

    if (lines.empty())
        // ID não encontrado.
        throw invalid_argument("O ID " + to_string(id) +
                               " não existe na tabela " + table_name + ".");
    else if (lines.size() > 1)
        // Falha de integridade: ID duplicado.
        throw runtime_error("Mais de uma linha com ID " + to_string(id) +
                            ". Integridade da tabela " + table_name +
                            " comprometida.");

    return lines.front();
}

// [SQL: SELECT BY COLUMN] Busca registros por um critério em uma coluna.
vector<string> MockConnection::selectByColumn(const string& table_name,
                                              size_t index,
                                              const string& value) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    vector<string> results;

    // Itera a partir da linha 1 (ignora cabeçalho).
    for (size_t i = 1; i < lines.size(); ++i) {
        const string& line = lines[i];

        try {
            // Tenta extrair a coluna e compara o valor.
            string col = extractColumnFromLine(line, index);
            if (col == value) {
                results.push_back(line);
            }
        } catch (const invalid_argument& ignore) {
            // Se a linha for malformada e não tiver a coluna, apenas ignora.
            continue;
        }
    }

    return results;  // Retorna a lista de resultados (pode ser vazia).
}

// [SQL: SELECT ALL] Retorna todos os registros de uma tabela.
vector<string> MockConnection::selectAll(const string& table_name) const {
    string filename = getFullFilePath(table_name);
    vector<string> all_lines = readAllLines(filename);

    // Retorna todas as linhas a partir do índice 1 (dados), se houver
    // cabeçalho.
    if (all_lines.size() > 1) {
        return vector<string>(all_lines.begin() + 1, all_lines.end());
    }
    return {};
}

// [SQL: UPDATE] Atualiza o registro associado ao ID.
void MockConnection::update(const string& table_name, long id,
                            const string& data) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    string id_str = to_string(id);
    string new_record;
    bool updated = false;

    // Formata a nova linha CSV completa.
    if (data.empty()) {
        new_record = id_str;
    } else {
        new_record = id_str + "," + data;
    }

    // 1. Localiza o registro e o modifica no vetor de linhas.
    for (size_t i = 1; i < lines.size(); ++i) {
        string& line = lines[i];

        try {
            long current_id = extractIdFromLine(line);
            if (current_id == id) {
                line = new_record;  // Substitui a linha.
                updated = true;
                break;
            }
        } catch (const invalid_argument& e) {
            continue;
        }
    }

    if (!updated) {
        // Lança exceção se o ID não foi encontrado.
        throw invalid_argument("O ID " + to_string(id) +
                               " não existe para ser atualizado na tabela " +
                               table_name + ".");
    }

    // 2. Reescreve o arquivo com a linha modificada.
    writeAllLines(filename, lines);
}

/**
 * [SQL: DELETE BY COLUMN] Remove todas as linhas onde o valor da coluna
 * (índice) corresponde ao valor fornecido.
 * @return O número de linhas removidas.
 */
size_t MockConnection::deleteByColumn(const string& table_name, size_t index,
                                      const string& value) const {
    string filename = getFullFilePath(table_name);
    vector<string> lines = readAllLines(filename);
    size_t initial_size = lines.size();

    // Se o arquivo estiver vazio, nada a remover.
    if (initial_size <= 1) {
        return 0;  // 0 ou 1 significa cabeçalho ou vazio.
    }

    // Garante que a iteração comece APÓS o cabeçalho.
    auto start_it = lines.begin() + 1;

    // 1. Remove_if + erase idiom: Remove linhas que correspondem ao critério.
    // O predicado (lambda) retorna 'true' se a linha DEVE ser removida.
    lines.erase(
        remove_if(start_it, lines.end(),
                  [&index, &value](const string& line) {
                      try {
                          string col_value = extractColumnFromLine(line, index);
                          return col_value == value;  // Condição de remoção
                      } catch (const invalid_argument& e) {
                          // Linha malformada (não tem a coluna) não é removida.
                          return false;
                      }
                  }),
        lines.end());

    size_t removed_count = initial_size - lines.size();

    if (removed_count > 0) {
        // 2. Se alguma linha foi removida, reescreve o arquivo.
        writeAllLines(filename, lines);
    }

    return removed_count;
}

// [SQL: DELETE] Remove o registro associado ao ID.
void MockConnection::deleteRecord(const string& table_name, long id) const {
    string id_str = to_string(id);

    // Chama o método genérico, buscando pelo ID (coluna 0).
    // O ID é a chave primária, então a unicidade é esperada.
    size_t removed_count = deleteByColumn(table_name, 0, id_str);

    if (removed_count == 0) {
        // Se 0 registros foram removidos, significa que o ID não foi encontrado
        // ou a tabela estava vazia.
        throw invalid_argument("O ID " + to_string(id) +
                               " não existe na tabela " + table_name + ".");
    }
}