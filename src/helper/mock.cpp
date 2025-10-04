#include "helper/mock.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

// Lê todas as linhas do arquivo e retorna como um vetor de strings.
// É a base para todas as operações de leitura e escrita.
vector<string> readAllLines(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    // Se o arquivo não puder ser aberto (ex: não existe), retorna vetor vazio.
    if (!file.is_open()) {
        return lines;
    }

    string line;
    while (getline(file, line)) {
        // Trata a quebra de linha do Windows (CRLF) removendo o '\r' extra.
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        // Ignora linhas vazias após o tratamento de caracteres.
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

// Sobrescreve o arquivo inteiro com o conteúdo do vetor de linhas.
// Usado para operações de UPDATE e DELETE.
void writeAllLines(const string& filename, const vector<string>& lines) {
    // ios::trunc garante que o arquivo seja limpo antes da escrita.
    ofstream file(filename, ios::trunc);

    // Lança exceção em caso de falha crítica na abertura/escrita do arquivo.
    if (!file.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo para escrita: '" +
                            filename + "'.");
    }
    for (const string& line : lines) {
        // Garante a quebra de linha padrão ('\n').
        file << line << "\n";
    }
}

// Extrai o conteúdo de uma coluna específica (índice 'col', começando em 0) de
// uma linha CSV. É uma função utilitária fundamental do DAL.
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

// Extrai e converte o ID (coluna 0) de uma linha CSV para um long.
// Lança exceção se o ID estiver ausente ou não for numérico.
long extractIdFromLine(const string& line) {
    string id_str;

    try {
        // Tenta extrair a coluna 0 e converter para long.
        id_str = extractColumnFromLine(line, 0);
        return stol(id_str);
    } catch (const invalid_argument& e) {
        // Captura falha na extração da coluna 0 (linha vazia ou malformada).
        throw invalid_argument("A linha não contém um ID válido na coluna 0.");
    } catch (const exception& e) {
        // Captura falha na conversão numérica (stol) - ID não é um número.
        throw invalid_argument("O valor do ID '" + id_str +
                               "' não é um número válido.");
    }
}

// Insere um novo registro, gerando o próximo ID automaticamente.
long insert(const string& filename, const string& data) {
    vector<string> lines = readAllLines(filename);
    long max_id = 0;

    // Itera a partir da linha 1 (ignora cabeçalho) para encontrar o maior ID.
    for (size_t i = 1; i < lines.size(); ++i) {
        try {
            long current_id = extractIdFromLine(lines[i]);
            if (current_id > max_id) {
                max_id = current_id;
            }
        } catch (const invalid_argument& ignore) {
            // Ignora linhas que não possuem IDs válidos ou são malformadas.
        }
    }

    long new_id = max_id + 1;
    string new_record;

    // Formata o novo registro: ID ou ID,data.
    if (data.empty()) {
        new_record = to_string(new_id);
    } else {
        new_record = to_string(new_id) + "," + data;
    }

    // Abre o arquivo para anexar (append).
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        // Lança exceção em caso de falha de I/O na escrita.
        throw runtime_error("Não foi possível abrir o arquivo '" + filename +
                            "' para anexar.");
    }
    file << new_record << "\n";
    // Retorna o ID gerado para que a camada de serviço possa usá-lo
    // imediatamente.
    return new_id;
}

// Busca todas as linhas onde o conteúdo de uma coluna corresponde a um valor.
// Retorna um vetor vazio se não encontrar resultados.
vector<string> findByColumn(const string& filename, size_t index,
                            const string& value) {
    vector<string> lines = readAllLines(filename);
    vector<string> results;

    for (size_t i = 1; i < lines.size(); ++i) {  // Ignora cabeçalho.
        const string& line = lines[i];

        try {
            string col = extractColumnFromLine(line, index);
            if (col == value) {
                results.push_back(line);
            }
        } catch (const invalid_argument& ignore) {
            // Ignora linhas que não possuem o índice de coluna solicitado.
            continue;
        }
    }

    return results;  // Retorna o vetor (pode ser vazio).
}

// Busca uma única linha pelo ID (coluna 0).
// Impõe a regra de que o ID deve ser único.
string find(const string& filename, long id) {
    // Usa findByColumn para buscar por ID.
    vector<string> lines = findByColumn(filename, 0, to_string(id));

    if (lines.empty())
        // Lança exceção se o registro não for encontrado.
        throw invalid_argument("O ID " + to_string(id) + " não existe.");
    else if (lines.size() > 1)
        // Lança exceção crítica se houver duplicatas (falha na integridade dos
        // dados).
        throw runtime_error("Mais de uma linha com ID " + to_string(id) +
                            ". Integridade do arquivo comprometida.");

    return lines.front();  // Retorna a linha única encontrada.
}

// Retorna todos os registros de dados, excluindo o cabeçalho.
vector<string> findAll(const string& filename) {
    vector<string> all_lines = readAllLines(filename);

    // Retorna todas as linhas a partir do índice 1, se houver mais de 1 linha.
    if (all_lines.size() > 1) {
        return vector<string>(all_lines.begin() + 1, all_lines.end());
    }
    return {};  // Retorna vetor vazio se o arquivo estiver vazio ou tiver
                // apenas cabeçalho.
}

// Atualiza o registro associado ao ID fornecido.
void update(const string& filename, long id, const string& data) {
    vector<string> lines = readAllLines(filename);
    string id_str = to_string(id);
    string new_record;
    bool updated = false;

    // Formata o novo registro completo.
    if (data.empty()) {
        new_record = id_str;
    } else {
        new_record = id_str + "," + data;
    }

    // Percorre as linhas a partir do cabeçalho.
    for (size_t i = 1; i < lines.size(); ++i) {
        string& line = lines[i];

        try {
            long current_id = extractIdFromLine(line);
            if (current_id == id) {
                line = new_record;  // Substitui a linha antiga pela nova.
                updated = true;
                break;  // O ID é único, então podemos parar.
            }
        } catch (const invalid_argument& e) {
            // Ignora linhas com IDs malformados.
            continue;
        }
    }

    if (!updated) {
        // Lança exceção se o ID não foi encontrado no loop.
        throw invalid_argument("O ID " + to_string(id) +
                               " não existe para ser atualizado.");
    }

    // Reescreve o arquivo com a linha atualizada.
    writeAllLines(filename, lines);
}

// Remove o registro associado ao ID fornecido.
void remove(const string& filename, long id) {
    vector<string> lines = readAllLines(filename);
    size_t initial_size = lines.size();

    // Configura o iterador para começar após o cabeçalho (linha 1).
    auto start_it = lines.begin();
    if (lines.size() > 0) {
        start_it++;
    } else {
        // Se o arquivo estiver vazio, o ID não existe.
        throw invalid_argument("O ID " + to_string(id) + " não existe.");
    }

    // Usa remove_if + erase idiom para remover o registro.
    lines.erase(remove_if(start_it, lines.end(),
                          [&id](const string& line) {
                              try {
                                  // Tenta extrair o ID para comparação.
                                  long current_id = extractIdFromLine(line);
                                  return current_id == id;
                              } catch (const invalid_argument& e) {
                                  // Garante que linhas malformadas não sejam
                                  // removidas por engano.
                                  return false;
                              }
                          }),
                lines.end());

    if (lines.size() >= initial_size) {
        // Se o tamanho não mudou, o ID não foi encontrado.
        throw invalid_argument("O ID " + to_string(id) + " não existe.");
    }

    // Reescreve o arquivo sem a linha removida.
    writeAllLines(filename, lines);
}