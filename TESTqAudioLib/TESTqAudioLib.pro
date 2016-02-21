QT += core multimedia
QT -= gui

TARGET = TESTqAudioLib
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L$$PWD/../ -lqAudiolib

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../


HEADERS += \
    stopobject.h
    
    

