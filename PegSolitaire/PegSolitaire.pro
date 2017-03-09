#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T18:59:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PegSolitaire
TEMPLATE = app


SOURCES += main.cpp\
        PegSolitaire.cpp \
    Board.cpp \
    Piece.cpp

HEADERS  += PegSolitaire.h \
    Board.h \
    Piece.h

FORMS    += PegSolitaire.ui

RESOURCES += \
    PegSolitaire.qrc
