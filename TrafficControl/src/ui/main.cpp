#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <limits>

#include "fabrica_veiculos.h"
#include "autocarro_escolar.h"
#include "carro_comum.h"
#include "semaforo.h"
#include "configuracao.h"
#include "estado_verde.h"
#include "estado_amarelo.h"
#include "estado_vermelho.h"
#include "estado_intermitente_escolar.h"

int main()
{
    std::cout << "===== TESTE CORE: VEICULOS E SEMAFORO COM HORARIO ESCOLAR =====\n";

    // 1) TESTE DA FACTORY
    std::unique_ptr<Veiculo> vEscolar = FabricaVeiculos::criar("escolar", 0.0);
    std::unique_ptr<Veiculo> vPais    = FabricaVeiculos::criar("pais", 0.0);

    std::cout << "\n[Factory] Veiculos criados:\n";
    std::cout << " - Tipo: " << vEscolar->tipo()
              << " | prioridade = " << vEscolar->prioridade() << '\n';
    std::cout << " - Tipo: " << vPais->tipo()
              << " | prioridade = " << vPais->prioridade() << '\n';

    std::cout << "[Factory] Prioridade escolar < pais ? "
              << ( (*vEscolar) < (*vPais) ? "SIM (escolar tem PRIORIDADE)" : "NAO" )
              << '\n';

    // 2) TESTE DE POLIMORFISMO
    std::cout << "\n[Teste Polimorfismo] Movimento dos veiculos:\n";
    double deltaTempo = 1.0;
    vEscolar->atualizar(deltaTempo);
    vPais->atualizar(deltaTempo);

    std::cout << " - " << vEscolar->tipo()
              << " nova posicaoX = " << vEscolar->posicaoX() << " m\n";
    std::cout << " - " << vPais->tipo()
              << " nova posicaoX = " << vPais->posicaoX() << " m\n";

    // 3) TESTE DO SEMAFORO COM HORARIO ESCOLAR
    std::cout << "\n[Teste Semaforo] Transicoes de estado com PRIORIDADE ESCOLAR:\n";

    GestorConfiguracao& config = GestorConfiguracao::obterInstancia();

    // ATIVA MODO TESTE PARA USAR HORA SIMULADA
    config.definirModoTeste(true);

    // Pergunta a hora simulada
    int hora;
    std::cout << "\n--- SIMULACAO DE HORARIO ---\n";
    std::cout << "Informe a HORA simulada (0-23): ";
    std::cin >> hora;

    // Calcula se essa hora é "horario escolar"
    bool ehHorarioEscolarSimulado =
        ((hora == 7 || hora == 8) || (hora == 17 || hora == 18));

    // Força esse valor no gestor
    config.definirHorarioEscolarForcado(ehHorarioEscolarSimulado);

    // Limpa o \n do buffer para o std::cin.get() do final funcionar
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Carrega config.json (se existir)
    try {
        config.carregarConfiguracao("config.json");
        std::cout << "Config.json carregado.\n";
    } catch (const ErroConfiguracao& e) {
        std::cout << "Falha ao carregar: " << e.what() << " (usa defaults)\n";
    }

    // Agora estaEmHorarioEscolar() usa o valor simulado
    bool ehEscolar = config.estaEmHorarioEscolar();
    std::cout << ">>> HORARIO ESCOLAR ATUAL? "
              << (ehEscolar ? "SIM" : "NAO")
              << " (hora simulada: " << hora << "h)\n";

    SemaforoContexto semaforo;
    semaforo.definirHorarioEscolar(ehEscolar);

    double passoTempo = 1.0;
    for (int i = 0; i < 30; ++i) {
        std::cout << "Iteracao " << i
                  << " | Estado = " << semaforo.estadoAtualNome()
                  << " | Carros pode passar = "
                  << (semaforo.permitePassagem() ? "SIM" : "NAO")
                  << " | Horario escolar = " << (semaforo.estaEmHorarioEscolar() ? "SIM" : "NAO")
                  << " | Tempo no estado = " << semaforo.tempoNoEstado() << "s"
                  << std::endl;

        semaforo.atualizar(passoTempo);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "\n===== FIM DO TESTE (prioridade veicular + horario escolar OK!) =====\n";
    std::cout << "Pressione ENTER para sair...\n";
    std::cin.get();
    return 0;
}
