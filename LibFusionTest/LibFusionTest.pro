#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T15:56:07
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui



DESTDIR = ../../OUT
MOC_DIR = BUILD
OBJECTS_DIR = BUILD


TARGET = FusionTestsuit
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_fusiontestsuit.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -lLibFusion
LIBS += -L$$PWD/../BUILD -lLibFusion

INCLUDEPATH += $$PWD/../LibFusion
DEPENDPATH += $$PWD/../LibFusion

