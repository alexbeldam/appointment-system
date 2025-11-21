#ifndef HORARIO_CONTROLLER_HPP
#define HORARIO_CONTROLLER_HPP

#include "service/horarioService.hpp"
#include "util/utils.hpp"

/**
 * @brief Controlador responsável por mediar a criação e listagem de horários.
 */
class HorarioController {
   private:
    const std::shared_ptr<HorarioService>& service;

   public:
    HorarioController(const std::shared_ptr<HorarioService>& service);

    ~HorarioController() = default;

    std::shared_ptr<Horario> cadastrarHorario(long idProfessor,
                                              Timestamp inicio, Timestamp fim);

    bool excluirTodosPorProfessor(long idProfessor);
    bool excluirPorId(long idHorario);
};

#endif
