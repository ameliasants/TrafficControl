#include "semaforo.h"
#include "estado_vermelho.h"
#include "excecoes.h"

SemaforoContexto::SemaforoContexto()
    : estadoAtual_(nullptr), tempoNoEstado_(0.0), horarioEscolar_(false)
{
    definirEstado(std::make_unique<EstadoVermelho>());
}

SemaforoContexto::SemaforoContexto(double tempoInicial)
    : estadoAtual_(nullptr), tempoNoEstado_(tempoInicial), horarioEscolar_(false)
{
    if (tempoInicial < 0.0)
        throw EstadoInvalidoException("tempo inicial não pode ser negativo");
    definirEstado(std::make_unique<EstadoVermelho>());
}

void SemaforoContexto::definirEstado(std::unique_ptr<EstadoSemaforo> novoEstado)
{
    if (!novoEstado)
        throw EstadoInvalidoException("novo estado nulo");

    if (estadoAtual_)               // usa o ponteiro, não o getter
        estadoAtual_->sair(*this);

    estadoAtual_ = std::move(novoEstado);
    estadoAtual_->entrar(*this);
}
void SemaforoContexto::atualizar(double deltaTempo)
{
    if (estadoAtual_) {
        estadoAtual_->atualizar(*this, deltaTempo);
    }
}
