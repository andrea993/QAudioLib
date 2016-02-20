QT       += multimedia

QT       -= gui

TARGET = qAudiolib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += qaudiolib.cpp \
    mathematics.cpp \
    buffers.cpp

HEADERS += qaudiolib.h \
    mathematics.h \
    buffers.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    LGPLv2.1.txt
