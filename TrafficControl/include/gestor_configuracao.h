#ifndef GESTOR_CONFIGURACAO_H
#define GESTOR_CONFIGURACAO_H

#include <string>
#include <exception>


class ErroConfiguracao : public std::exception {
private:
    std::string mensagem_;
public:
    explicit ErroConfiguracao(const std::string& mensagem);
    const char* what() const noexcept override;
};

class GestorConfiguracao {
public:
    static GestorConfiguracao& obterInstancia();

    void carregarConfiguracao(const std::string& caminhoFicheiro);

    bool estaEmHorarioEscolar() const;
    int tempoVerdeAtual() const;

    // === NOVO: API de teste ===
    void definirModoTeste(bool ativo);
    void definirHorarioEscolarForcado(bool emHorario);

private:
    GestorConfiguracao();
    ~GestorConfiguracao();

    static GestorConfiguracao* instancia_;

    int tempoVerdeNormal_;
    int tempoVerdeEscolar_;

    // === NOVO: estado de teste ===
    bool modoTeste_;
    bool horarioEscolarForcado_;
};


#endif
