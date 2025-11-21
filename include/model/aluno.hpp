#ifndef ALUNO_HPP
#define ALUNO_HPP

#include <vector>

#include "model/usuario.hpp"

class Agendamento;  // Declaração forward

/**
 * @brief Representa a entidade Aluno, que é um tipo de Usuário.
 * * Um Aluno possui uma matrícula e está associado a uma lista de Agendamentos.
 * * A lista de agendamentos é carregada sob demanda (Lazy Loading) usando
 * EntityList.
 */
class Aluno : public Usuario {
   private:
    long matricula; /**< O número de matrícula único do aluno. */

    /**
     * @brief Lista de agendamentos associados a este aluno.
     * * Armazenada como EntityList para suportar carregamento preguiçoso.
     */
    EntityList<Agendamento> agendamentos;

   public:
    /**
     * @brief Alias para o tipo EntityList de Agendamento.
     */
    using AgendamentoList = EntityList<Agendamento>;

    /**
     * @brief Alias para o vetor de ponteiros inteligentes de Agendamento.
     */
    using AgendamentoVector = AgendamentoList::EntityVector;

    /**
     * @brief Alias para a função de carregamento que fornece a lista de
     * agendamentos.
     */
    using AgendamentosLoader = ListLoaderFunction<Agendamento>;

    /**
     * @brief Construtor da classe Aluno.
     * * Inicializa os atributos do Usuário e do Aluno, e injeta a função de
     * carregamento dos agendamentos.
     * @param id O identificador único do usuário.
     * @param nome O nome do aluno.
     * @param email O email do aluno.
     * @param senha A senha do aluno.
     * @param matricula O número de matrícula do aluno.
     * @param loader A função de carregamento para os agendamentos do aluno.
     */
    Aluno(long id, const std::string& nome, const std::string& email,
          const std::string& senha, long matricula,
          const AgendamentosLoader& loader);

    /**
     * @brief Retorna o número de matrícula do aluno.
     * @return long O número de matrícula.
     */
    long getMatricula() const;

    /**
     * @brief Define um novo número de matrícula para o aluno.
     * @param matricula O novo número de matrícula.
     */
    void setMatricula(long matricula);

    /**
     * @brief Retorna a lista de Agendamentos do aluno.
     * * O carregamento dos agendamentos é disparado na primeira chamada a esta
     * função ou se o EntityList for acessado.
     * @return AgendamentoList& A referência para a lista de agendamentos.
     */
    AgendamentoList& getAgendamentos();

    /**
     * @brief Retorna uma lista filtrada de Agendamentos que podem ser
     * cancelados.
     * * Esta função carrega todos os agendamentos e aplica a lógica de
     * filtragem.
     * @return AgendamentoVector Um vetor contendo apenas os agendamentos que
     * satisfazem a regra de cancelamento.
     */
    AgendamentoVector getAgendamentosCancelaveis();
};

#endif