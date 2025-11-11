#ifndef HORARIO_HPP
#define HORARIO_HPP

#include <ctime>
#include <string>

/**
 * @brief Representa a entidade Horário de disponibilidade de um professor.
 */
class Horario {
   private:
    long id;
    long idProfessor;
    std::time_t inicio;
    std::time_t fim;
    bool disponivel;

   public:
    Horario() = default;

    Horario(long id, long idProf, std::time_t inicio, std::time_t fim,
            bool disp);

    long getId() const;
    long getProfessorId() const;
    std::time_t getInicio() const;
    std::time_t getFim() const;
    bool isDisponivel() const;

    std::string getInicioStr() const;
    std::string getFimStr() const;

    void setId(long id);
    void setProfessorId(long idProfessor);
    void setInicio(std::time_t inicio);
    void setFim(std::time_t fim);
    void setDisponivel(bool disponivel);

    bool operator<(const Horario& other) const;
};

#endif
