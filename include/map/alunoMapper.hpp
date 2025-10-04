#ifndef ALUNO_MAPPER_HPP
#define ALUNO_MAPPER_HPP

#include "dto/alunoDTO.hpp"
#include "model/aluno.hpp"

/**
 * @brief Classe responsável por mapear (converter) objetos entre o Data
 * Transfer Object (DTO) e o Modelo de Domínio (Model).
 *
 * * O Mapper atua na camada de Serviço, garantindo a separação de
 * responsabilidades entre o formato de dados externos (CSV/DTO) e a
 * representação interna do domínio (Aluno).
 */
class AlunoMapper {
   public:
    /**
     * @brief Mapeia um objeto AlunoDTO para um objeto Aluno (Modelo de
     * Domínio).
     * * * Usado tipicamente na camada de Serviço para converter dados de
     * entrada (DTO) em uma entidade manipulável pela lógica de negócio.
     * * @param dto O DTO contendo os dados básicos do Aluno.
     * @return O objeto Aluno (Modelo) com os campos básicos preenchidos.
     */
    Aluno map(const AlunoDTO& dto) const;

    /**
     * @brief Mapeia um objeto Aluno (Modelo de Domínio) para um objeto
     * AlunoDTO.
     * * * Usado para preparar o retorno de dados da camada de Serviço para a
     * camada de Controle/Apresentação, expondo apenas os campos de interesse.
     * * @param aluno O objeto Aluno (Modelo).
     * @return Um objeto AlunoDTO com os campos básicos preenchidos.
     */
    AlunoDTO map(const Aluno& aluno) const;

    /**
     * @brief Converte os dados do AlunoDTO em uma string formatada para
     * inserção ou atualização no arquivo CSV.
     * * * Esta função não inclui o ID, apenas os campos de dados.
     * * @param aluno O DTO com os dados a serem serializados.
     * @return Uma string no formato CSV (ex: "nome,email,senha,matricula").
     */
    std::string toCsvData(const AlunoDTO& aluno) const;

    /**
     * @brief Converte uma linha CSV completa em um objeto Aluno (Modelo de
     * Domínio).
     * * * Esta função é usada pela camada de Serviço para desserializar
     * registros lidos do arquivo, criando um novo objeto Aluno pronto para
     * manipulação.
     * * @param line A linha CSV completa, incluindo o ID (ex:
     * "ID,nome,email,...").
     * @return O objeto Aluno (Modelo) preenchido.
     * @throws std::invalid_argument Se a linha CSV estiver malformada (campos
     * inválidos ou ausentes).
     */
    Aluno fromCsvLine(const std::string& line) const;
};

#endif