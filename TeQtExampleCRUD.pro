#-------------------------------------------------
#
# Project created by QtCreator 2017-11-14T19:26:33
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += DEBUG
CONFIG += c++1z
TARGET = TeQtExampleCRUD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp \
    dao.cpp

HEADERS  += mainwindow.h \
    person.h \
    dao.h \
    interfacedao.h

FORMS    += mainwindow.ui
