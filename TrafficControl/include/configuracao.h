#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include <exception>
#include <string>

class ErroConfiguracao : public std::exception {
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

    // Modo de teste: usar valor forçado em vez do relógio real
    void definirModoTeste(bool ativo);
    void definirHorarioEscolarForcado(bool emHorario);

private:
    GestorConfiguracao();
    ~GestorConfiguracao();

    static GestorConfiguracao* instancia_;

    int tempoVerdeNormal_;
    int tempoVerdeEscolar_;

    // Estado de teste
    bool modoTeste_;
    bool horarioEscolarForcado_;
};

#endif // CONFIGURACAO_H
