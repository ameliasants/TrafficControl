#include "autocarro_escolar.h"

AutocarroEscolar::AutocarroEscolar(double posX, double vel)
    : Veiculo(posX, vel, 100)
{
}

std::string AutocarroEscolar::tipo() const
{
    return "Autocarro Escolar";
}
