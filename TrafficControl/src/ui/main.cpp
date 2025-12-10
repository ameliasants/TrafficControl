#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "fabrica_veiculos.h"

#include "semaforo_contexto.h"
#include "gestor_configuracao.h"


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
              << ( (*vEscolar) < (*vPais) ? "SIM (escolar tem PRIORIDADE)" : "NAO" ) << '\n';

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

    // Obter singleton de configuração
    GestorConfiguracao& config = GestorConfiguracao::obterInstancia();

    // *** NOVO: ATIVAR MODO DE TESTE E FORÇAR HORÁRIO ESCOLAR ***
    config.definirModoTeste(true);              // sempre em modo de teste
    config.definirHorarioEscolarForcado(true);  // força "estar em horario escolar"

    // Carregar config (se existir); se não, usa defaults
    try {
        config.carregarConfiguracao("config.json");
        std::cout << "Config.json carregado.\n";
    } catch (const ErroConfiguracao& e) {
        std::cout << "Falha ao carregar: " << e.what() << " (usa defaults)\n";
    }

    // Verifica (agora sob o efeito do modo de teste) se está em horário escolar
    bool ehEscolar = config.estaEmHorarioEscolar();
    std::cout << ">>> HORARIO ESCOLAR ATUAL? ";
    if (ehEscolar) {
        std::cout << "SIM (prioridade ALUNOS ativa! Tempo verde: "
                  << config.tempoVerdeAtual() << "s)" << std::endl;
    } else {
        std::cout << "NAO" << std::endl;
    }

    // Cria o semáforo e aplica a info de horário escolar
    SemaforoContexto semaforo;
    semaforo.definirHorarioEscolar(ehEscolar);  // *** ATIVA PRIORIDADE ***

    double passoTempo = 1.0;
    for (int i = 0; i < 30; ++i) {  // Mais iterações para ver ciclos
        std::cout << "Iteracao " << i
                  << " | Estado = " << semaforo.estadoAtualNome()
                  << " | Carros pode passar = " << (semaforo.permitePassagem() ? "SIM" : "NAO")
                  << " | Horario escolar = " << (semaforo.estaEmHorarioEscolar() ? "SIM" : "NAO")
                  << " | Tempo no estado = " << semaforo.tempoNoEstado() << "s"
                  << std::endl;

        semaforo.atualizar(passoTempo);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Mais lento para visualizar
    }

    std::cin.get();
    return 0;
}
