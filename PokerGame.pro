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
    pokerscene.cpp \
    pokercarditem.cpp \
    pokercardmodel.cpp \
    modeldan.cpp \
    modeldui.cpp \
    modelhong.cpp \
    modelzha.cpp \
    modellong.cpp \
    modelliandui.cpp \
    modelsanliandui.cpp \
    settingstab.cpp

HEADERS  += pokergame.h \
    pokerscene.h \
    pokercarditem.h \
    pokercardmodel.h \
    modeldan.h \
    modeldui.h \
    modelhong.h \
    modelzha.h \
    modellong.h \
    modelliandui.h \
    modelsanliandui.h \
    settingstab.h

RESOURCES += \
    pokerimages.qrc

OTHER_FILES +=

RC_FILE = \
icon.rc

FORMS += \
    settingstab.ui
