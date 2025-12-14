#include "configuracao.h"
#include <fstream>
#include <sstream>
#include <ctime>

GestorConfiguracao* GestorConfiguracao::instancia_ = nullptr;

// ==================== ErroConfiguracao ====================

ErroConfiguracao::ErroConfiguracao(const std::string& mensagem)
    : mensagem_(mensagem)
{
}

const char* ErroConfiguracao::what() const noexcept {
    return mensagem_.c_str();
}

// ==================== CONSTRUTOR ====================

GestorConfiguracao::GestorConfiguracao()
    : tempoVerdeNormal_(5),
    tempoVerdeEscolar_(5),
    modoTeste_(false),
    horarioEscolarForcado_(false),
    haAlunos_(false)
{

    if (tempoVerdeNormal_ <= 0 || tempoVerdeEscolar_ <= 0) {
        throw ErroConfiguracao("tempos de verde devem ser positivos");
    }
}

GestorConfiguracao::~GestorConfiguracao() = default;

// ==================== SINGLETON ====================

GestorConfiguracao& GestorConfiguracao::obterInstancia()
{
    if (!instancia_) {
        instancia_ = new GestorConfiguracao();
    }
    return *instancia_;
}

// ==================== CARREGAR CONFIG ====================



void GestorConfiguracao::carregarConfiguracao(const std::string& caminhoFicheiro)
{

    std::ifstream ficheiro(caminhoFicheiro);
    if (!ficheiro.is_open()) {

        throw ErroConfiguracao("Ficheiro config.json nao encontrado: " + caminhoFicheiro);
    }


    if (!ficheiro.is_open()) {
        const char* candidatos[] = {
            "../config.json",
            "../../config.json",
            "../../../src/core/config.json",
            "../../src/core/config.json"
        };

        for (const char* c : candidatos) {
            ficheiro.open(c);
            if (ficheiro.is_open()) {
                break;
            }
        }

        if (!ficheiro.is_open()) {
            throw ErroConfiguracao(
                "ficheiro não encontrado: " + caminhoFicheiro
                );
        }
    }

    std::stringstream buffer;
    buffer << ficheiro.rdbuf();
    std::string conteudo = buffer.str();

    if (conteudo.find("zona_escolar") == std::string::npos) {
        throw ErroConfiguracao("JSON inválido: falta objeto zona_escolar");
    }
}

// ==================== CONTROLO DE TESTE ====================

void GestorConfiguracao::definirModoTeste(bool ativo)
{
    modoTeste_ = ativo;
}

void GestorConfiguracao::definirHorarioEscolarForcado(bool emHorario)
{
    horarioEscolarForcado_ = emHorario;
}

// ==================== HORÁRIO ESCOLAR ====================

bool GestorConfiguracao::estaEmHorarioEscolar() const
{
    if (modoTeste_) {
        return horarioEscolarForcado_;
    }

    std::time_t now = std::time(nullptr);
    std::tm* lt = std::localtime(&now);
    int hora = lt->tm_hour;
    int wday = lt->tm_wday;


    return (wday >= 1 && wday <= 5) &&
           ((hora >= 7 && hora < 8) || (hora >= 17 && hora < 18));
}

// ==================== TEMPO DE VERDE ====================

int GestorConfiguracao::tempoVerdeAtual() const
{
    return estaEmHorarioEscolar() ? tempoVerdeEscolar_ : tempoVerdeNormal_;
}
