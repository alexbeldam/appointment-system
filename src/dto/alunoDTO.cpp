#include "dto/alunoDTO.hpp"
using namespace std;

long AlunoDTO::getMatricula() const {
    return matricula;
}

void AlunoDTO::setMatricula(long matricula) {
    this->matricula = matricula;
}