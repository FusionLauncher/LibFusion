#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T23:12:49
#
#-------------------------------------------------

QT       -= gui

TARGET = LibFusion
TEMPLATE = lib

DEFINES += LIBFUSION_LIBRARY

SOURCES += libfusion.cpp \
    fgame.cpp

HEADERS += libfusion.h\
        libfusion_global.h \
    fgame.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
