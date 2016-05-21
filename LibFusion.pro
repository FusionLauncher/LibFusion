QT       += core sql network

TARGET = LibFusion
TEMPLATE = lib

CONFIG += c++11

DEFINES += LIBFUSION_LIBRARY

DESTDIR = ../OUT
MOC_DIR = ../BUILD
OBJECTS_DIR = ../BUILD

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
    flogger.cpp \
    flogging.cpp \
    ftest.cpp

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
    flogger.h \
    flogging.h \
    ftest.h

unix {
    target.path = /usr/lib
    headers.path = /usr/include
    headers.files = $$HEADERS
    INSTALLS += target headers
}
