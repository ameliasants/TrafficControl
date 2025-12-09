#include "veiculo.h"

Veiculo::Veiculo(double posX, double vel, int prio)
    : posicaoX_(posX),
    velocidade_(vel),
    prioridade_(prio)
{
}

void Veiculo::atualizar(double deltaTempo)
{
    // Movimento simples: x = x + v * t
    posicaoX_ += velocidade_ * deltaTempo;
}

bool Veiculo::operator<(const Veiculo& outro) const
{
    // Maior prioridade deve "ganhar" na fila (ajuste se quiser o inverso)
    return prioridade_ < outro.prioridade_;
}
