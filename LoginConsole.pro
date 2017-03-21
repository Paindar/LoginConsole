#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T01:16:09
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoginConsole
TEMPLATE = app
RC_FILE = app.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    globals.cpp

HEADERS  += mainwindow.h \
    globals.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    app.rc
