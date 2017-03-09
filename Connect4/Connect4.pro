#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T07:20:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Connect4
TEMPLATE = app


SOURCES += main.cpp\
        Connect4.cpp \
    Ball.cpp \
    Game.cpp

HEADERS  += Connect4.h \
    Ball.h \
    Game.h

FORMS    += Connect4.ui

RESOURCES += \
    Connect4.qrc
