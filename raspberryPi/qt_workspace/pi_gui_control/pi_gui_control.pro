#-------------------------------------------------
#
# Project created by QtCreator 2015-04-19T06:16:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pi_gui_control
TEMPLATE = app
INCLUDEPATH +=/usr/local/include
LIBS+=-lwiringPi

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
