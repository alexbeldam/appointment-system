#ifndef ALUNO_HPP
#define ALUNO_HPP

#include <vector>

#include "model/agendamento.hpp"
#include "model/usuario.hpp"

/**
 * @brief Representa a entidade Aluno, que herda as propriedades básicas de
 * Usuario.
 * * Adiciona atributos específicos de domínio como o número de matrícula e uma
 * lista de agendamentos associados.
 */
class Aluno : public Usuario {
   private:
    long matricula;  ///< Número de matrícula único do Aluno.
    std::vector<Agendamento>
        agendametos;  ///< Lista de Agendamentos associados a este Aluno.

   public:
    /**
     * @brief Construtor padrão. Inicializa o objeto Aluno com valores default.
     */
    Aluno();

    /**
     * @brief Construtor completo do Aluno.
     * * Inicializa os campos herdados (ID, nome, email, senha) e os campos
     * específicos (matrícula, agendamentos).
     * @param id O identificador único.
     * @param nome O nome do Aluno.
     * @param email O e-mail do Aluno.
     * @param senha A senha do Aluno.
     * @param matricula O número de matrícula do Aluno.
     * @param agendamentos Uma lista de Agendamentos associados.
     */
    Aluno(long id, const std::string& nome, const std::string& email,
          const std::string& senha, long matricula,
          const std::vector<Agendamento>& agendamentos);

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

    /**
     * @brief Obtém a lista de Agendamentos associados a este Aluno.
     * * @return Uma referência constante para o vetor de Agendamentos.
     */
    const std::vector<Agendamento>& getAgendamentos() const;

    /**
     * @brief Define a lista de Agendamentos.
     * * * Nota: No contexto de mapeamento (mapper), esta lista é frequentemente
     * preenchida pela camada de Serviço.
     * * @param agendamentos O novo vetor de Agendamentos.
     */
    void setAgendamentos(const std::vector<Agendamento>& agendamentos);
};

#endif