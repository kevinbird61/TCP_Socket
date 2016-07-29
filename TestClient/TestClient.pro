QT += core network
QT -= gui

CONFIG += c++11

TARGET = TestClient
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    clientusage.cpp

HEADERS += \
    clientusage.h
