#ifndef HORARIO_CONTROLLER_HPP
#define HORARIO_CONTROLLER_HPP

#include "service/horarioService.hpp"

/**
 * @brief Controller para gerenciamento de Horários disponíveis.
 * * Esta classe atua como a camada de controle (Controller) do padrão MVC,
 * responsável por receber requisições, delegar a lógica de negócio
 * para a camada de Service e retornar o resultado.
 */
class HorarioController {
   private:
    /**
     * @brief Ponteiro inteligente para o serviço de horários.
     * * Responsável por isolar a lógica de negócio e o acesso aos dados.
     */
    const std::shared_ptr<HorarioService>& service;

   public:
    /**
     * @brief Construtor da classe HorarioController.
     * * @param service O serviço de horários injetado via dependência.
     */
    HorarioController(const std::shared_ptr<HorarioService>& service);

    /**
     * @brief Destrutor padrão.
     */
    ~HorarioController() = default;

    /**
     * @brief Cadastra um novo Horário (Requisição POST).
     * * Cria um novo recurso de Horário, associando-o a um professor.
     * * @param idProfessor O identificador único do professor que terá o
     * horário.
     * @param inicio O timestamp de início do horário.
     * @param fim O timestamp de fim do horário.
     * @return std::shared_ptr<Horario> O objeto Horário recém-cadastrado.
     */
    std::shared_ptr<Horario> cadastrarHorario(long idProfessor,
                                              Timestamp inicio, Timestamp fim);

    /**
     * @brief Exclui todos os Horários de um professor (Requisição DELETE).
     * * Remove todos os recursos de Horário associados ao ID do professor.
     * * @param idProfessor O identificador único do professor cujos horários
     * serão excluídos.
     * @return true Se a exclusão de um ou mais horários for bem-sucedida.
     * @return false Se a operação falhar ou nenhum horário for
     * encontrado/excluído.
     */
    bool excluirTodosPorProfessor(long idProfessor);

    /**
     * @brief Exclui um Horário específico pelo seu ID (Requisição DELETE).
     * * Remove o recurso de Horário com o ID especificado.
     * * @param idHorario O identificador único do horário a ser excluído.
     * @return true Se o horário for encontrado e deletado com sucesso.
     * @return false Se a operação falhar (ex: horário não encontrado).
     */
    bool excluirPorId(long idHorario);
};

#endif