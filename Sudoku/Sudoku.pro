#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T16:55:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku
TEMPLATE = app

SOURCES += main.cpp\
        Sudoku.cpp \
    Board.cpp \
    Cell.cpp \
    Position.cpp \
    DefaultBoards.cpp

HEADERS  += Sudoku.h \
    Board.h \
    Cell.h \
    Position.h \
    DefaultBoards.h

FORMS    += Sudoku.ui \
    Board.ui

RESOURCES += \
    Sudoku.qrc
