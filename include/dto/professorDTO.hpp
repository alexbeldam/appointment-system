#ifndef PROFESSOR_DTO_HPP
#define PROFESSOR_DTO_HPP

#include <vector>

#include "dto/usuarioDTO.hpp"

/**
 * @brief Data Transfer Object (DTO) para a entidade Professor.
 * * Esta classe é utilizada para transferir dados de e para a camada de
 * serviço, contendo apenas os atributos básicos da entidade Professor.
 */
class ProfessorDTO : public UsuarioDTO {
   private:
    std::string disciplina;  ///< A disciplina do Professor.

   public:
    /**
     * @brief Obtém a disciplina do Professor.
     * * @return Uma referência constante para a string da disciplina.
     */
    const std::string& getDisciplina() const;

    /**
     * @brief Define a disciplina do Professor.
     * * @param disciplina A nova disciplina.
     */
    void setDisciplina(const std::string& disciplina);
};

#endif