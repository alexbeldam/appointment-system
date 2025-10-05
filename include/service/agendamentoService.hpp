#ifndef AGENDAMENTO_SERVICE_HPP
#define AGENDAMENTO_SERVICE_HPP

#include <vector>

#include "data/mockConnection.hpp"
#include "model/agendamento.hpp"

/**
 * @brief CLASSE INCOMPLETA: Simula a Camada de Serviço para a entidade
 * Agendamento.
 *
 * * Esta classe foi criada apenas para satisfazer as dependências e permitir a
 * compilação e o desenvolvimento da classe AlunoService e AlunoController.
 * * A implementação atual não interage com a Camada de Acesso a Dados (DAL)
 * de forma funcional.
 */
class AgendamentoService {
   private:
    const MockConnection&
        connection;  ///< Conexão simulada com o banco de dados

   public:
    AgendamentoService(const MockConnection& connection);

    /**
     * @brief Lista todos os Agendamentos associados a um Aluno específico.
     * * * Esta função é utilizada para injetar dados relacionados ao Modelo
     * Aluno.
     * * **NOTA DE IMPLEMENTAÇÃO:** Atualmente, a função não possui lógica de
     * busca e retorna sempre um vetor vazio (std::vector<Agendamento>{}) como
     * placeholder.
     * * @param id O identificador único (ID) do Aluno.
     * @return Um vetor **vazio** de Agendamentos.
     */
    std::vector<Agendamento> listByIdAluno(long id) const;
};

#endif