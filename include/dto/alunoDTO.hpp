#ifndef ALUNO_DTO_HPP
#define ALUNO_DTO_HPP

#include <vector>

#include "dto/usuarioDTO.hpp"

/**
 * @brief Data Transfer Object (DTO) para a entidade Aluno.
 * * Esta classe é utilizada para transferir dados de e para a camada de
 * serviço, contendo apenas os atributos básicos da entidade Aluno.
 */
class AlunoDTO : public UsuarioDTO {
   private:
    long matricula;  ///< Número de matrícula único do Aluno.

   public:
    /**
     * @brief Obtém o número de matrícula do Aluno.
     * * @return O número de matrícula.
     */
    long getMatricula() const;

    /**
     * @brief Define o número de matrícula do Aluno.
     * * @param matricula O novo número de matrícula.
     */
    void setMatricula(long matricula);
};

#endif