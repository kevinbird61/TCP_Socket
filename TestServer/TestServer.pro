QT += core network
QT -= gui

CONFIG += c++11

TARGET = TestServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    connectserver.cpp \
    tcpthread.cpp

HEADERS += \
    connectserver.h \
    tcpthread.h
