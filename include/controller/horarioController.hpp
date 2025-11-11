#ifndef HORARIO_CONTROLLER_HPP
#define HORARIO_CONTROLLER_HPP

#include "service/horarioService.hpp"
#include "util/utils.hpp"

/**
 * @brief Controlador responsável por mediar a criação e listagem de horários.
 */
class HorarioController {
   private:
    const HorarioService& service;

   public:
    HorarioController(const HorarioService& service);

    Horario cadastrarHorario(long idProfessor, std::time_t inicio,
                             std::time_t fim) const;

    bool excluirTodosPorProfessor(long idProfessor) const;
    bool excluirPorId(long idHorario) const;

    std::vector<Horario> listarPorProfessor(long idProfessor) const;

    Horario pegarHorario(long id) const;
};

#endif
