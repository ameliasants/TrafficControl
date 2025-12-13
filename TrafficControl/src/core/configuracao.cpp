#include "configuracao.h"
#include <fstream>
#include <sstream>
#include <ctime>

// Inicialização do singleton
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
    horarioEscolarForcado_(false)
{
    // validação básica dos tempos
    if (tempoVerdeNormal_ <= 0 || tempoVerdeEscolar_ <= 0) {
        throw ConfiguracaoInvalidaException("tempos de verde devem ser positivos");
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
        // Erro específico de configuração
        throw ConfiguracaoInvalidaException("ficheiro não encontrado: " + caminhoFicheiro);
    }

    std::stringstream buffer;
    buffer << ficheiro.rdbuf();
    std::string conteudo = buffer.str();

    // Validação mínima do JSON (estrutura esperada)
    if (conteudo.find("zona_escolar") == std::string::npos) {
        throw ConfiguracaoInvalidaException("JSON inválido: falta objeto zona_escolar");
    }

    // Parsing real do JSON fica na camada UI (Qt JSON)
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

int GestorConfiguracao::tempoVerdeAtual() const
{
    return estaEmHorarioEscolar() ? tempoVerdeEscolar_ : tempoVerdeNormal_;
}
