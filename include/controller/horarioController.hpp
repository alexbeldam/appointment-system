#ifndef HORARIO_CONTROLLER_HPP
#define HORARIO_CONTROLLER_HPP

#include "service/horarioService.hpp"
#include "util/utils.hpp"

/**
 * @brief Controlador responsável por mediar a criação e listagem de horários.
 */
class HorarioController {
 private:
   HorarioService& service;

 public:
  HorarioController(HorarioService& service);

Horario cadastrarHorario(long idProfessor, const std::string& inicio,
                         const std::string& fim);


  std::vector<Horario> listarPorProfessor(long idProfessor) const;
};

#endif
