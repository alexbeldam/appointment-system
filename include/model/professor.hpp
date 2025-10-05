#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include <vector>

#include "model/horario.hpp"
#include "model/usuario.hpp"

/**
 * @brief Representa a entidade Professor, que herda as propriedades básicas de
 * Usuario.
 * * Adiciona atributos específicos de domínio como o número de matrícula e uma
 * lista de horarios associados.
 */
class Professor : public Usuario {
   private:
    std::string disciplina;  ///< A dsiciplina do Professor.
    std::vector<Horario>
        horarios;  ///< Lista de Horarios associados a este Professor.

   public:
    /**
     * @brief Construtor padrão. Inicializa o objeto Professor com valores
     * default.
     */
    Professor();

    /**
     * @brief Construtor completo do Professor.
     * * Inicializa os campos herdados (ID, nome, email, disciplina) e os campos
     * específicos (matrícula, horarios).
     * @param id O identificador único.
     * @param nome O nome do Professor.
     * @param email O e-mail do Professor.
     * @param disciplina A disciplina do Professor.
     * @param disciplina A disciplina do Professor.
     * @param horarios Uma lista de Horarios associados.
     */
    Professor(long id, const std::string& nome, const std::string& email,
              const std::string& senha, std::string disciplina,
              const std::vector<Horario>& horarios);

    /**
     * @brief Obtém a disciplina do Usuário.
     * * @return Uma referência constante para a string da disciplina.
     */
    const std::string& getDisciplina() const;

    /**
     * @brief Define a disciplina do Usuário.
     * * @param disciplina A nova disciplina.
     */
    void setDisciplina(const std::string& disciplina);

    /**
     * @brief Obtém a lista de Horarios associados a este Professor.
     * * @return Uma referência constante para o vetor de Horarios.
     */
    const std::vector<Horario>& getHorarios() const;

    /**
     * @brief Define a lista de Horarios.
     * * * Nota: No contexto de mapeamento (mapper), esta lista é frequentemente
     * preenchida pela camada de Serviço.
     * * @param horarios O novo vetor de Horarios.
     */
    void setHorarios(const std::vector<Horario>& horarios);
};

#endif