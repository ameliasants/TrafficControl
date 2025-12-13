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

    bool ok1 = false, ok2 = false, ok3 = false, ok4 = false, ok5 = false;

    // 1) Criação bem-sucedida
    std::cout << "1. Semaforo valido... ";
    {
        SemaforoContexto s1(0.0);
        ok1 = true;
        std::cout << "OK\n";
    }

    // 2) Carregamento de configuração existente
    std::cout << "2. Carregamento de configuracao (ficheiro existe?)... ";
    try {
        GestorConfiguracao::obterInstancia().carregarConfiguracao("../../../src/core/config.json");
        ok2 = true;
        std::cout << "OK\n";
    } catch (const ErroConfiguracao& e) {
        std::cout << "FALHOU: " << e.what() << '\n';
    }

    // 3) Falha esperada: tempo negativo no semáforo
    std::cout << "3. Semaforo tempo negativo... ";
    try {
        SemaforoContexto s2(-1.0);
        std::cout << "FALHOU (nao lancou excecao)\n";
    } catch (const EstadoInvalidoException&) {
        ok3 = true;
        std::cout << "OK\n";
    }

    // 4) Falha esperada: ficheiro de configuração inexistente
    std::cout << "4. Configuracao ficheiro inexistente... ";
    try {
        GestorConfiguracao::obterInstancia().carregarConfiguracao("___config_que_nao_existe____.json");
        std::cout << "FALHOU\n";
    } catch (const ErroConfiguracao&) {
        ok4 = true;
        std::cout << "OK\n";
    }

    // 5) Transição Verde -> Amarelo
    std::cout << "5. Transicao Verde -> Amarelo... ";
    {
        auto& cfg = GestorConfiguracao::obterInstancia();
        cfg.definirModoTeste(true);
        cfg.definirHorarioEscolarForcado(false);

        SemaforoContexto s3(0.0);
        s3.definirEstado(std::make_unique<EstadoVerde>());

        // Escolhe um deltaTempo grande o suficiente para ultrapassar tempoVerdeAtual()
        double dt = cfg.tempoVerdeAtual() + 1.0;
        s3.atualizar(dt);

        if (s3.estadoAtualNome() == "AMARELO") {
            ok5 = true;
            std::cout << "OK\n";
        } else {
            std::cout << "FALHOU (estado atual = " << s3.estadoAtualNome() << ")\n";
        }
    }

    std::cout << "\n";
    if (ok1 && ok2 && ok3 && ok4 && ok5) {
        std::cout << "TODOS OS TESTES PASSARAM.\n";
    } else {
        std::cout << "ALGUNS TESTES FALHARAM.\n";
    }

    return (ok1 && ok2 && ok3 && ok4 && ok5) ? 0 : 1;
}
