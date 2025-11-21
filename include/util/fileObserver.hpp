#ifndef FILE_OBSERVER_HPP
#define FILE_OBSERVER_HPP

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace fs = std::filesystem;
/**
 * @brief Alias de tipo para representar o timestamp de modificação de um
 * arquivo.
 */
using FileTime = fs::file_time_type;

/**
 * @brief Classe responsável por observar e detectar alterações em um conjunto
 * de arquivos de dados.
 * * Usada para implementar o mecanismo de invalidação de cache baseado em
 * modificações no disco.
 */
class FileObserver {
   private:
    /**
     * @brief Diretório base onde os arquivos de dados são esperados.
     * * Usado para construir o caminho completo dos arquivos observados.
     */
    static constexpr const char* BASE_DIR = "data/";

    /**
     * @brief Extensão esperada para os arquivos de dados.
     */
    static constexpr const char* EXTENSION = ".csv";

    /**
     * @brief Mapa que armazena o timestamp da última modificação conhecida
     * para cada arquivo.
     * * Chave: O caminho completo do arquivo (string).
     * * Valor: O timestamp da última verificação (FileTime).
     */
    std::map<std::string, FileTime> fileTimestamps;

   public:
    /**
     * @brief Construtor da classe FileObserver.
     * * Inicializa o observador registrando e salvando o timestamp atual
     * de todos os arquivos fornecidos.
     * @param filenames Um vetor de strings contendo os nomes base dos arquivos
     * a serem observados (o caminho completo é construído internamente).
     */
    FileObserver(const std::vector<std::string>& filenames);

    /**
     * @brief Verifica se algum dos arquivos observados foi modificado desde
     * a última chamada ou inicialização.
     * * Se uma mudança for detectada, o timestamp é atualizado e true é
     * retornado.
     * @return bool True se pelo menos um arquivo foi alterado; false caso
     * contrário.
     */
    bool hasFileChanged();
};

#endif