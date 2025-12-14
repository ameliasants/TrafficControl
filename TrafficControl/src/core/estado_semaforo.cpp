#include "estado_semaforo.h"
#include "semaforo.h"
#include "configuracao.h"
#include <memory>

// ==================== ESTADO VERDE ====================

class EstadoVerde : public EstadoSemaforo {
public:
    void entrar(SemaforoContexto& contexto) override {
        contexto.reiniciarTempoNoEstado();
        contexto.definirHorarioEscolar(GestorConfiguracao::obterInstancia().estaEmHorarioEscolar());
    }

    void sair(SemaforoContexto&) override {}

    void atualizar(SemaforoContexto& contexto, double deltaTempo) override;

    std::string nome() const override { return "VERDE"; }

    bool permitePassagem() const override { return true; }
};

// ==================== ESTADO AMARELO ====================

class EstadoAmarelo : public EstadoSemaforo {
public:
    void entrar(SemaforoContexto& contexto) override {
        contexto.reiniciarTempoNoEstado();
    }

    void sair(SemaforoContexto&) override {}

    void atualizar(SemaforoContexto& contexto, double deltaTempo) override;

    std::string nome() const override { return "AMARELO"; }

    bool permitePassagem() const override { return true; } // ou false, se quiser que no amarelo parem
};

// ==================== ESTADO VERMELHO ====================

class EstadoVermelho : public EstadoSemaforo {
public:
    void entrar(SemaforoContexto& contexto) override {
        contexto.reiniciarTempoNoEstado();
    }

    void sair(SemaforoContexto&) override {}

    void atualizar(SemaforoContexto& contexto, double deltaTempo) override;

    std::string nome() const override { return "VERMELHO"; }

    bool permitePassagem() const override { return false; }
};

// ==================== ESTADO INTERMITENTE ESCOLAR ====================

class EstadoIntermitenteEscolar : public EstadoSemaforo {
public:
    void entrar(SemaforoContexto& contexto) override {
        contexto.reiniciarTempoNoEstado();
    }

    void sair(SemaforoContexto&) override {}

    void atualizar(SemaforoContexto& contexto, double deltaTempo) override;

    std::string nome() const override { return "INTERMITENTE_ESCOLAR"; }

    bool permitePassagem() const override { return true; }
};

// ==================== LÓGICAS DE TRANSIÇÃO ====================


void EstadoVerde::atualizar(SemaforoContexto& contexto, double deltaTempo) {
    contexto.incrementarTempoNoEstado(deltaTempo);

    double tempoVerde = GestorConfiguracao::obterInstancia().tempoVerdeAtual();

    if (contexto.tempoNoEstado() >= tempoVerde) {
        contexto.definirEstado(std::make_unique<EstadoAmarelo>());
    }
}

void EstadoAmarelo::atualizar(SemaforoContexto& contexto, double deltaTempo) {
    contexto.incrementarTempoNoEstado(deltaTempo);

    const double TEMPO_AMARELO = 3.0;
    if (contexto.tempoNoEstado() > TEMPO_AMARELO) {
        contexto.definirEstado(std::make_unique<EstadoVermelho>());
    }
}

void EstadoVermelho::atualizar(SemaforoContexto& contexto, double deltaTempo) {
    contexto.incrementarTempoNoEstado(deltaTempo);
    const double TEMPO_VERMELHO = 10.0;

    if (contexto.tempoNoEstado() >= TEMPO_VERMELHO) {
        auto& cfg = GestorConfiguracao::obterInstancia();

        if (cfg.estaEmHorarioEscolar() && cfg.haAlunos()) {
            contexto.definirEstado(std::make_unique<EstadoIntermitenteEscolar>());
        } else {
            contexto.definirEstado(std::make_unique<EstadoVerde>());
        }
    }
}

void EstadoIntermitenteEscolar::atualizar(SemaforoContexto& contexto, double deltaTempo)
{
    contexto.incrementarTempoNoEstado(deltaTempo);

    const double TEMPO_INTERMITENTE = 8.0;
    if (contexto.tempoNoEstado() > TEMPO_INTERMITENTE) {
        contexto.definirEstado(std::make_unique<EstadoVerde>());
    }
}
