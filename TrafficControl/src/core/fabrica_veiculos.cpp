#include "fabrica_veiculos.h"
#include "autocarro_escolar.h"
#include "carro_comum.h"

std::unique_ptr<Veiculo> FabricaVeiculos::criar(const std::string& tipo, double posX)
{
    if (tipo == "escolar") {
        return std::make_unique<AutocarroEscolar>(posX, 10.0);
    } else if (tipo == "pais") {
        return std::make_unique<CarroComum>(posX, 12.0);
    } else {
        return std::make_unique<CarroComum>(posX, 8.0);
    }
}
