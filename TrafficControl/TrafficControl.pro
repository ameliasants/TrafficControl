QT += core gui widgets

CONFIG += console
CONFIG += c++17
CONFIG -= app_bundle


INCLUDEPATH += $$PWD/include
DEPENDPATH  += $$PWD/include


SOURCES += \
    src/core/autocarro_escolar.cpp \
    src/core/carro_comum.cpp \
    src/core/configuracao.cpp \
    src/core/estado_semaforo.cpp \
    src/core/fabrica_veiculos.cpp \
    src/core/semaforo.cpp \
    src/core/veiculo.cpp \
    #testes/tst_semaforo.cpp
    src/ui/main_console.cpp


HEADERS += \
    include/autocarro_escolar.h \
    include/carro_comum.h \
    include/configuracao.h \
    include/estado_amarelo.h \
    include/estado_intermitente_escolar.h \
    include/estado_semaforo.h \
    include/estado_verde.h \
    include/estado_vermelho.h \
    include/excecoes.h \
    include/fabrica_veiculos.h \
    include/semaforo.h \
    include/veiculo.h

DISTFILES += \
    src/core/config.json

