#-------------------------------------------------
#
# Project created by QtCreator 2014-11-25T15:18:10
#
#-------------------------------------------------

QT       += core gui svg network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokerGame
TEMPLATE = app


SOURCES += main.cpp\
        pokergame.cpp \
    pokercard.cpp \
    pokercardgraphicssvgitem.cpp \
    pokerscene.cpp

HEADERS  += pokergame.h \
    pokercard.h \
    pokercardgraphicssvgitem.h \
    pokerscene.h

RESOURCES += \
    pokerimages.qrc

OTHER_FILES +=
