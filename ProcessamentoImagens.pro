# created by Heitor Adao Junior

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProcessamentoImagens
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colormatrix.cpp \
    configdialog.cpp

HEADERS  += mainwindow.h \
    colormatrix.h \
    configdialog.h \
    colorfrequency.h

FORMS    += mainwindow.ui \
    configdialog.ui
