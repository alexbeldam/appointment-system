#ifndef PROFESSOR_MAPPER_HPP
#define PROFESSOR_MAPPER_HPP

#include "dto/professorDTO.hpp"
#include "model/professor.hpp"

/**
 * @brief Classe responsável por mapear (converter) objetos entre o Data
 * Transfer Object (DTO) e o Modelo de Domínio (Model).
 *
 * * O Mapper atua na camada de Serviço, garantindo a separação de
 * responsabilidades entre o formato de dados externos (CSV/DTO) e a
 * representação interna do domínio (Professor).
 */
class ProfessorMapper {
   public:
    /**
     * @brief Mapeia um objeto ProfessorDTO para um objeto Professor (Modelo de
     * Domínio).
     * * * Usado tipicamente na camada de Serviço para converter dados de
     * entrada (DTO) em uma entidade manipulável pela lógica de negócio.
     * * @param dto O DTO contendo os dados básicos do Professor.
     * @return O objeto Professor (Modelo) com os campos básicos preenchidos.
     */
    Professor map(const ProfessorDTO& dto) const;

    /**
     * @brief Mapeia um objeto Professor (Modelo de Domínio) para um objeto
     * ProfessorDTO.
     * * * Usado para preparar o retorno de dados da camada de Serviço para a
     * camada de Controle/Apresentação, expondo apenas os campos de interesse.
     * * @param professor O objeto Professor (Modelo).
     * @return Um objeto ProfessorDTO com os campos básicos preenchidos.
     */
    ProfessorDTO map(const Professor& professor) const;

    /**
     * @brief Converte os dados do ProfessorDTO em uma string formatada para
     * inserção ou atualização no arquivo CSV.
     * * * Esta função não inclui o ID, apenas os campos de dados.
     * * @param professor O DTO com os dados a serem serializados.
     * @return Uma string no formato CSV (ex: "nome,email,senha,disciplina").
     */
    std::string toCsvData(const ProfessorDTO& professor) const;

    /**
     * @brief Converte uma linha CSV completa em um objeto Professor (Modelo de
     * Domínio).
     * * * Esta função é usada pela camada de Serviço para desserializar
     * registros lidos do arquivo, criando um novo objeto Professor pronto para
     * manipulação.
     * * @param line A linha CSV completa, incluindo o ID (ex:
     * "ID,nome,email,...").
     * @return O objeto Professor (Modelo) preenchido.
     * @throws std::invalid_argument Se a linha CSV estiver malformada (campos
     * inválidos ou ausentes).
     */
    Professor fromCsvLine(const std::string& line) const;
};

#endif