#include "gestor_configuracao.h"
#include <fstream>
#include <sstream>
#include <ctime>

GestorConfiguracao* GestorConfiguracao::instancia_ = nullptr;

ErroConfiguracao::ErroConfiguracao(const std::string& mensagem)
    : mensagem_(mensagem) {}

const char* ErroConfiguracao::what() const noexcept {
    return mensagem_.c_str();
}

// ==================== CONSTRUTOR ====================

GestorConfiguracao::GestorConfiguracao()
    : tempoVerdeNormal_(30),
    tempoVerdeEscolar_(45),
    modoTeste_(false),
    horarioEscolarForcado_(false) {}

GestorConfiguracao::~GestorConfiguracao() = default;

// ==================== SINGLETON ====================

GestorConfiguracao& GestorConfiguracao::obterInstancia() {
    if (!instancia_) {
        instancia_ = new GestorConfiguracao();
    }
    return *instancia_;
}

// ==================== CARREGAR CONFIG ====================

void GestorConfiguracao::carregarConfiguracao(const std::string& caminhoFicheiro) {
    std::ifstream ficheiro(caminhoFicheiro);
    if (!ficheiro.is_open()) {
        throw ErroConfiguracao("Ficheiro config.json não encontrado: " + caminhoFicheiro);
    }

    std::stringstream buffer;
    buffer << ficheiro.rdbuf();
    std::string conteudo = buffer.str();

    if (conteudo.find("\"zona_escolar\"") == std::string::npos) {
        throw ErroConfiguracao("JSON inválido: falta objeto zona_escolar");
    }

    // Parsing real ficará na camada UI com Qt JSON
}

// ==================== NOVO: CONTROLO DE TESTE ====================

void GestorConfiguracao::definirModoTeste(bool ativo) {
    modoTeste_ = ativo;
}

void GestorConfiguracao::definirHorarioEscolarForcado(bool emHorario) {
    horarioEscolarForcado_ = emHorario;
}

// ==================== HORÁRIO ESCOLAR ====================

bool GestorConfiguracao::estaEmHorarioEscolar() const {
    // 1) Se estivermos em modo de teste, devolve sempre o valor forçado
    if (modoTeste_) {
        return horarioEscolarForcado_;
    }

    // 2) Caso contrário, usa o relógio real do sistema
    std::time_t now = std::time(nullptr);
    std::tm* lt = std::localtime(&now);
    int hora = lt->tm_hour;
    int wday = lt->tm_wday;  // 0=Dom, 1=Seg, ..., 6=Sáb

    // Dias úteis (Seg-Sex) E (entrada 7-8h OU saída 17-18h)
    return (wday >= 1 && wday <= 5) &&
           ((hora >= 7 && hora < 8) || (hora >= 17 && hora < 18));
}

// ==================== TEMPO DE VERDE ====================

int GestorConfiguracao::tempoVerdeAtual() const {
    // Em modo de teste, usamos um tempo menor para ver o ciclo completo mais rápido
    if (modoTeste_) {
        return 5; // por exemplo, 5 segundos de verde no teste
    }

    // Comportamento normal
    return estaEmHorarioEscolar() ? tempoVerdeEscolar_ : tempoVerdeNormal_;
}
