#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T16:27:59
#
#-------------------------------------------------

QT       += core gui \
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = needed
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openstatus.cpp \
    common.cpp \
    searchDialog.cpp

HEADERS  += mainwindow.h \
    openstatus.h \
    common.h \
    searchDialog.h
