#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp02
TEMPLATE = app


SOURCES += main.cpp\
        banc.cpp \
        obstacle.cpp \
        poisson.cpp \
        predateur.cpp \
        princ.cpp \
        glarea.cpp \
        puff.cpp \
        smoke.cpp

HEADERS  += princ.h \
        banc.h \
        glarea.h \
        obstacle.h \
        poisson.h \
        predateur.h \
        puff.h \
        smoke.h

FORMS    += princ.ui

RESOURCES += \
    tp02.qrc

DISTFILES +=
