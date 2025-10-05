#ifndef HORARIO_SERVICE_HPP
#define HORARIO_SERVICE_HPP

#include <vector>

#include "data/mockConnection.hpp"
#include "model/horario.hpp"

/**
 * @brief CLASSE INCOMPLETA: Simula a Camada de Serviço para a entidade
 * Horario.
 *
 * * Esta classe foi criada apenas para satisfazer as dependências e permitir a
 * compilação e o desenvolvimento da classe ProfessorService e
 * ProfessorController.
 * * A implementação atual não interage com a Camada de Acesso a Dados (DAL)
 * de forma funcional.
 */
class HorarioService {
   private:
    const MockConnection&
        connection;  ///< Conexão simulada com o banco de dados

   public:
    HorarioService(const MockConnection& connection);

    /**
     * @brief Lista todos os Horarios associados a um Professor específico, cuja
     * coluna disponivel seja true.
     * * * Esta função é utilizada para injetar dados relacionados ao Modelo
     * Professor.
     * * **NOTA DE IMPLEMENTAÇÃO:** Atualmente, a função não possui lógica de
     * busca e retorna sempre um vetor vazio (std::vector<Horario>{}) como
     * placeholder.
     * * @param id O identificador único (ID) do Professor.
     * @return Um vetor **vazio** de Horarios.
     */
    std::vector<Horario> listDisponivelByIdProfessor(long id) const;
};

#endif