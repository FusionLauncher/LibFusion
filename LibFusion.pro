#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T23:12:49
#
#-------------------------------------------------

QT       += gui
QT       += core
QT	 += sql
QT       += network

TARGET = LibFusion
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11
DEFINES += LIBFUSION_LIBRARY

SOURCES += libfusion.cpp \
    fgame.cpp \
    fdb.cpp \
    flibrary.cpp \
    fcrawler.cpp \
    fartmanager.cpp \
    ffiledownloader.cpp \
    fdbupdater.cpp \
    fclientupdater.cpp \
    flauncher.cpp \
    fwatchedfolder.cpp \
    ffilesync.cpp \
    flogger.cpp

HEADERS += libfusion.h\
    libfusion_global.h \
    fgame.h \
    fdb.h \
    flibrary.h \
    fcrawler.h \
    fartmanager.h \
    ffiledownloader.h \
    thegamedbstorage.h \
    fdbupdater.h \
    fclientupdater.h \
    flauncher.h \
    fwatchedfolder.h \
    ffilesync.h \
    fexception.h \
    f_dbg.h \
    flogger.h \
    buildno.h

unix {
    target.path = /usr/lib
    headers.path = /usr/include
    headers.files = $$HEADERS
    INSTALLS += target headers
}
