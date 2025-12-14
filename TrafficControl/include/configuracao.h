#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include <string>
#include <exception>

class ErroConfiguracao : public std::exception {
public:
    explicit ErroConfiguracao(const std::string& mensagem);
    const char* what() const noexcept override;

private:
    std::string mensagem_;
};

class GestorConfiguracao {
public:
    static GestorConfiguracao& obterInstancia();

    void carregarConfiguracao(const std::string& caminhoFicheiro);

    void definirModoTeste(bool ativo);
    void definirHorarioEscolarForcado(bool emHorario);

    bool estaEmHorarioEscolar() const;
    int tempoVerdeAtual() const;

    bool haAlunos() const { return haAlunos_; }
    void definirHaAlunos(bool valor) { haAlunos_ = valor; }

private:
    GestorConfiguracao();
    ~GestorConfiguracao();

    static GestorConfiguracao* instancia_;

    int tempoVerdeNormal_;
    int tempoVerdeEscolar_;

    bool modoTeste_;
    bool horarioEscolarForcado_;

    bool haAlunos_ = false;
};

#endif
