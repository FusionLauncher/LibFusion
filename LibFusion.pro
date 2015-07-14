#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T23:12:49
#
#-------------------------------------------------

QT       += gui
QT       += core
QT	 += sql
QT       += network
QT       += webkit


#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QT       += webkitwidgets
#endif

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
    fclientupdater.cpp

HEADERS += libfusion.h\
    libfusion_global.h \
    fgame.h \
    fdb.h \
    flibrary.h \
    fcrawler.h \
    fartmanager.h \
    ffiledownloader.h \
    thegamedbstorage.h \
    fclientupdater.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
