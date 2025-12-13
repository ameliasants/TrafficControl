/*#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H


#include <exception>
#include <string>
#include "excecoes.h"

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
    bool modoTeste_;
    bool horarioEscolarForcado_;
};

#endif // CONFIGURACAO_H
*/

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

    // MODO TESTE
    void definirModoTeste(bool ativo);
    void definirHorarioEscolarForcado(bool emHorario);

    bool estaEmHorarioEscolar() const;
    int tempoVerdeAtual() const;

    // <<< NOVO: flag de alunos >>>
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

    // <<< NOVO: guarda se há alunos >>>
    bool haAlunos_ = false;
};

#endif
