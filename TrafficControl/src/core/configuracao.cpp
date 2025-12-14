#include "configuracao.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <regex>

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

    std::stringstream buffer;
    buffer << ficheiro.rdbuf();
    std::string conteudo = buffer.str();

    // Verifica presença do objeto zona_escolar
    if (conteudo.find("\"zona_escolar\"") == std::string::npos) {
        throw ErroConfiguracao("JSON invalido: falta objeto zona_escolar");
    }

    std::smatch m;

    auto extrairString = [&](const std::string& chave) -> std::string {
        std::regex re("\"" + chave + "\"\\s*:\\s*\"([^\"]+)\"");
        if (!std::regex_search(conteudo, m, re)) {
            throw ErroConfiguracao("JSON invalido: falta chave string \"" + chave + "\"");
        }
        return m[1].str();
    };

    auto extrairInt = [&](const std::string& chave) -> int {
        std::regex re("\"" + chave + "\"\\s*:\\s*([-]?[0-9]+)");
        if (!std::regex_search(conteudo, m, re)) {
            throw ErroConfiguracao("JSON invalido: falta chave numerica \"" + chave + "\"");
        }
        int valor = std::stoi(m[1].str());
        if (valor < 0) {
            throw ErroConfiguracao("JSON invalido: valor negativo em \"" + chave + "\"");
        }
        return valor;
    };

    auto parseHoraMin = [&](const std::string& texto, int& h, int& min) {
        std::regex re("^(\\d{2}):(\\d{2})$");
        if (!std::regex_match(texto, m, re)) {
            throw ErroConfiguracao("Formato de hora invalido: " + texto);
        }
        h   = std::stoi(m[1].str());
        min = std::stoi(m[2].str());
        if (h < 0 || h > 23 || min < 0 || min > 59) {
            throw ErroConfiguracao("Hora fora do intervalo valido: " + texto);
        }
    };

    // Lê e valida horas
    std::string inicioStr = extrairString("inicio");
    std::string fimStr    = extrairString("fim");

    int horaInicio = 0, minutoInicio = 0;
    int horaFim    = 0, minutoFim    = 0;
    parseHoraMin(inicioStr, horaInicio, minutoInicio);
    parseHoraMin(fimStr,    horaFim,    minutoFim);

    // Lê tempos de verde
    int tempoVerdeEscolarJson = extrairInt("tempo_verde_escolar");
    int tempoVerdeNormalJson  = extrairInt("tempo_verde_normal");

    // Atribuição às variáveis da classe
    tempoVerdeEscolar_ = tempoVerdeEscolarJson;
    tempoVerdeNormal_  = tempoVerdeNormalJson;


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
