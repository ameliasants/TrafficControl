#include <iostream>
#include <cassert>
#include <memory>

#include "semaforo.h"
#include "configuracao.h"
#include "veiculo.h"
#include "excecoes.h"
#include "estado_verde.h"

int main()
{
    std::cout << "=== TESTES CORE TrafficControl ===\n\n";

    // 1) Criação bem-sucedida
    std::cout << "1. Semaforo valido... ";
    SemaforoContexto s1(0.0);
    std::cout << "OK\n";

    std::cout << "2. Carregamento de configuracao (ficheiro existe?)... ";
    try {
        GestorConfiguracao::obterInstancia().carregarConfiguracao("../../../src/core/config.json");
        std::cout << "OK\n";
    } catch (const ConfiguracaoInvalidaException& e) {
        std::cout << "FALHOU: " << e.what() << '\n';
        return 1;
    }


    // 2) Falhas / exceções
    std::cout << "3. Semaforo tempo negativo... ";
    try {
        SemaforoContexto s2(-1.0);
        std::cout << "FALHOU (nao lancou excecao)\n";
        return 1;
    } catch (const EstadoInvalidoException&) {
        std::cout << "OK\n";
    }

    std::cout << "4. Configuracao ficheiro inexistente... ";
    try {
        GestorConfiguracao::obterInstancia().carregarConfiguracao("nao_existe.json");
        std::cout << "FALHOU\n";
        return 1;
    } catch (const ConfiguracaoInvalidaException&) {
        std::cout << "OK\n";
    }

    // 3) Transições de estado
    std::cout << "5. Transicao Verde -> Amarelo... ";
    auto& cfg = GestorConfiguracao::obterInstancia();
    cfg.definirModoTeste(true);
    cfg.definirHorarioEscolarForcado(false);
    // se tiver método para forçar tempoVerde, use-o; senão, ajuste no próprio singleton
    // aqui assumindo que tempoVerdeNormal é acessível ou tem setter na prática

    SemaforoContexto s3(0.0);
    s3.definirEstado(std::make_unique<EstadoVerde>());
    s3.atualizar(10.0); // maior que tempoVerdeNormal

    assert(s3.estadoAtualNome() == "AMARELO");
    std::cout << "OK\n";

    std::cout << "\nTODOS OS TESTES PASSARAM.\n";
    return 0;
}
