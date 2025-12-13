#ifndef SEMAFORO_H
#define SEMAFORO_H

#include "estado_semaforo.h"
#include <memory>
#include <string>
#include "excecoes.h"

class SemaforoContexto {
private:
    std::unique_ptr<EstadoSemaforo> estadoAtual_;
    double tempoNoEstado_;
    bool horarioEscolar_;

public:
    SemaforoContexto();
    explicit SemaforoContexto(double tempoInicial);

    void definirEstado(std::unique_ptr<EstadoSemaforo> novoEstado);
    void atualizar(double deltaTempo);
    EstadoSemaforo& estadoAtual() const { return *estadoAtual_; }

    void ativarModoIntermitenteEscolar();
    void desativarModoIntermitenteEscolar();


    // *** ALTERADO: regra especial para horário escolar ***
    bool permitePassagem() const {
        if (!estadoAtual_)              // usa o membro com sublinhado
            return false;

        return estadoAtual_->permitePassagem();
    }



    std::string estadoAtualNome() const {
        return estadoAtual_ ? estadoAtual_->nome() : "DESCONHECIDO";
    }

    void definirHorarioEscolar(bool ativo) { horarioEscolar_ = ativo; }
    bool estaEmHorarioEscolar() const { return horarioEscolar_; }

    double tempoNoEstado() const { return tempoNoEstado_; }
    void incrementarTempoNoEstado(double delta) { tempoNoEstado_ += delta; }
    void reiniciarTempoNoEstado() { tempoNoEstado_ = 0.0; }
};

#endif
