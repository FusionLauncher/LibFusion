#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T23:12:49
#
#-------------------------------------------------

QT       += gui
QT	 += sql

TARGET = LibFusion
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11
DEFINES += LIBFUSION_LIBRARY

SOURCES += libfusion.cpp \
    fgame.cpp \
    fdb.cpp \
    flibrary.cpp \
    fupdater.cpp \
    fcrawler.cpp

HEADERS += libfusion.h\
    libfusion_global.h \
    fgame.h \
    fdb.h \
    flibrary.h \
    fupdater.h \
    fcrawler.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
