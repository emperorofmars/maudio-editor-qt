#-------------------------------------------------
#
# Project created by QtCreator 2015-09-17T23:31:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maudio-editor-qt
TEMPLATE = app

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/ui

SOURCES += src/main.cpp\
        src/mainwindow.cpp

HEADERS  += include/mainwindow.h

FORMS    += ui/mainwindow.ui