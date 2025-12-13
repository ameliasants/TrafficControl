#ifndef EXCECOES_H
#define EXCECOES_H

#include <exception>
#include <string>

class SemaforoException : public std::exception {
    std::string msg;
public:
    explicit SemaforoException(const std::string& m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class ConfiguracaoInvalidaException : public SemaforoException {
public:
    explicit ConfiguracaoInvalidaException(const std::string& m)
        : SemaforoException("Configuração inválida: " + m) {}
};

class EstadoInvalidoException : public SemaforoException {
public:
    explicit EstadoInvalidoException(const std::string& m)
        : SemaforoException("Estado inválido: " + m) {}
};

class VeiculoInvalidoException : public SemaforoException {
public:
    explicit VeiculoInvalidoException(const std::string& m)
        : SemaforoException("Veículo inválido: " + m) {}
};

#endif // EXCECOES_H
