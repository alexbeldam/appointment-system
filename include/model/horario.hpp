#ifndef HORARIO_HPP
#define HORARIO_HPP

#include <string>

/**
 * @brief Representa a entidade Horário de disponibilidade de um professor.
 */
class Horario {
 private:
  long id;
  long idProfessor;
  std::string inicio;
  std::string fim;
  bool disponivel;

 public:
  Horario() = default;

    Horario(long id, long idProf, const std::string& inicio,
            const std::string& fim, bool disp)
        : id(id), idProfessor(idProf), inicio(inicio), fim(fim), disponivel(disp) {}


  long getId() const { return id; }
  long getProfessorId() const { return idProfessor; }
  const std::string& getInicio() const { return inicio; }
  const std::string& getFim() const { return fim; }
  bool isDisponivel() const { return disponivel; }

  void setId(long id) { this->id = id; }
  void setProfessorId(long idProfessor) { this->idProfessor = idProfessor; }
  void setInicio(const std::string& inicio) { this->inicio = inicio; }
  void setFim(const std::string& fim) { this->fim = fim; }
  void setDisponivel(bool disponivel) { this->disponivel = disponivel; }
};

#endif
