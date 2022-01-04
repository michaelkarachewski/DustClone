#-------------------------------------------------
#
# Project created by QtCreator 2021-11-23T15:50:57
#
#-------------------------------------------------

QT       += core gui sql
QT       += bluetooth
QT       += network
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DustApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    database_connection.cpp \
    dust_interface.cpp \
    downloader.cpp \
    gallery.cpp \
    searchfilter.cpp

HEADERS += \
        mainwindow.h \
    database_connection.h \
    dust_interface.h \
    downloader.h \
    gallery.h \
    searchfilter.h

FORMS += \
        mainwindow.ui \
    detectiondownloader.ui \
    gallery.ui \
    searchfilter.ui

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lcurl

INCLUDEPATH += $$PWD/../../../../../usr/include/x86_64-linux-gnu/curl
DEPENDPATH += $$PWD/../../../../../usr/include/x86_64-linux-gnu/curl

RESOURCES += \
    resource.qrc

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -ljsoncpp

INCLUDEPATH += $$PWD/../../../../../usr/include/jsoncpp
DEPENDPATH += $$PWD/../../../../../usr/include/jsoncpp
