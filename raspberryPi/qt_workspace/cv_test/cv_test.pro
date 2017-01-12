#-------------------------------------------------
#
# Project created by QtCreator 2014-03-11T22:30:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cv_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp
INCLUDEPATH += /home/pi/opencv_include



LIBS +=-lopencv_core

LIBS +=-lopencv_highgui
LIBS +=-lopencv_imgproc

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

