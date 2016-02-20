QT += core multimedia
QT -= gui

TARGET = TESTqAudioLib
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L$$PWD/../build-qAudiolib-Unnamed-Debug/ -lqAudiolib

INCLUDEPATH += $$PWD/../qAudiolib
DEPENDPATH += $$PWD/../qAudiolib


HEADERS += \
    stopobject.h

