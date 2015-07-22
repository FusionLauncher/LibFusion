#############################################################################
# Makefile for building: libLibFusion.so.1.0.0
# Generated by qmake (2.01a) (Qt 4.8.6) on: Tue Jul 21 23:30:53 2015
# Project:  LibFusion.pro
# Template: lib
# Command: /usr/lib/i386-linux-gnu/qt4/bin/qmake -o Makefile LibFusion.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DLIBFUSION_LIBRARY -DQT_NO_DEBUG -DQT_SQL_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -std=c++11 -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtSql -I/usr/include/qt4 -I.
LINK          = g++
LFLAGS        = -Wl,-O1 -shared -Wl,-soname,libLibFusion.so.1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtSql -lQtGui -lQtNetwork -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/lib/i386-linux-gnu/qt4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = libfusion.cpp \
		fgame.cpp \
		fdb.cpp \
		flibrary.cpp \
		fcrawler.cpp \
		fartmanager.cpp \
		ffiledownloader.cpp \
		fdbupdater.cpp \
		fclientupdater.cpp moc_flibrary.cpp \
		moc_fartmanager.cpp \
		moc_ffiledownloader.cpp \
		moc_fdbupdater.cpp \
		moc_fclientupdater.cpp
OBJECTS       = libfusion.o \
		fgame.o \
		fdb.o \
		flibrary.o \
		fcrawler.o \
		fartmanager.o \
		ffiledownloader.o \
		fdbupdater.o \
		fclientupdater.o \
		moc_flibrary.o \
		moc_fartmanager.o \
		moc_ffiledownloader.o \
		moc_fdbupdater.o \
		moc_fclientupdater.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/dll.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		LibFusion.pro
QMAKE_TARGET  = LibFusion
DESTDIR       = 
TARGET        = libLibFusion.so.1.0.0
TARGETA       = libLibFusion.a
TARGETD       = libLibFusion.so.1.0.0
TARGET0       = libLibFusion.so
TARGET1       = libLibFusion.so.1
TARGET2       = libLibFusion.so.1.0

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile  $(TARGET)

$(TARGET):  $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)



staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS)

Makefile: LibFusion.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/dll.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtSql.prl \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtNetwork.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile LibFusion.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/dll.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtSql.prl:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtNetwork.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile LibFusion.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/LibFusion1.0.0 || $(MKDIR) .tmp/LibFusion1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/LibFusion1.0.0/ && $(COPY_FILE) --parents libfusion.h libfusion_global.h fgame.h fdb.h flibrary.h fcrawler.h fartmanager.h ffiledownloader.h thegamedbstorage.h fdbupdater.h fclientupdater.h .tmp/LibFusion1.0.0/ && $(COPY_FILE) --parents libfusion.cpp fgame.cpp fdb.cpp flibrary.cpp fcrawler.cpp fartmanager.cpp ffiledownloader.cpp fdbupdater.cpp fclientupdater.cpp .tmp/LibFusion1.0.0/ && (cd `dirname .tmp/LibFusion1.0.0` && $(TAR) LibFusion1.0.0.tar LibFusion1.0.0 && $(COMPRESS) LibFusion1.0.0.tar) && $(MOVE) `dirname .tmp/LibFusion1.0.0`/LibFusion1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/LibFusion1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA)
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_flibrary.cpp moc_fartmanager.cpp moc_ffiledownloader.cpp moc_fdbupdater.cpp moc_fclientupdater.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_flibrary.cpp moc_fartmanager.cpp moc_ffiledownloader.cpp moc_fdbupdater.cpp moc_fclientupdater.cpp
moc_flibrary.cpp: fdb.h \
		fgame.h \
		libfusion_global.h \
		flibrary.h
	/usr/lib/i386-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) flibrary.h -o moc_flibrary.cpp

moc_fartmanager.cpp: fgame.h \
		libfusion_global.h \
		thegamedbstorage.h \
		fartmanager.h
	/usr/lib/i386-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) fartmanager.h -o moc_fartmanager.cpp

moc_ffiledownloader.cpp: ffiledownloader.h
	/usr/lib/i386-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ffiledownloader.h -o moc_ffiledownloader.cpp

moc_fdbupdater.cpp: fdb.h \
		fgame.h \
		libfusion_global.h \
		fdbupdater.h
	/usr/lib/i386-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) fdbupdater.h -o moc_fdbupdater.cpp

moc_fclientupdater.cpp: fclientupdater.h
	/usr/lib/i386-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) fclientupdater.h -o moc_fclientupdater.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

libfusion.o: libfusion.cpp libfusion.h \
		libfusion_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o libfusion.o libfusion.cpp

fgame.o: fgame.cpp fgame.h \
		libfusion_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fgame.o fgame.cpp

fdb.o: fdb.cpp fdb.h \
		fgame.h \
		libfusion_global.h \
		fdbupdater.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fdb.o fdb.cpp

flibrary.o: flibrary.cpp flibrary.h \
		fdb.h \
		fgame.h \
		libfusion_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o flibrary.o flibrary.cpp

fcrawler.o: fcrawler.cpp fcrawler.h \
		fdb.h \
		fgame.h \
		libfusion_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fcrawler.o fcrawler.cpp

fartmanager.o: fartmanager.cpp fartmanager.h \
		fgame.h \
		libfusion_global.h \
		thegamedbstorage.h \
		ffiledownloader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fartmanager.o fartmanager.cpp

ffiledownloader.o: ffiledownloader.cpp ffiledownloader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ffiledownloader.o ffiledownloader.cpp

fdbupdater.o: fdbupdater.cpp fdbupdater.h \
		fdb.h \
		fgame.h \
		libfusion_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fdbupdater.o fdbupdater.cpp

fclientupdater.o: fclientupdater.cpp fclientupdater.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fclientupdater.o fclientupdater.cpp

moc_flibrary.o: moc_flibrary.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_flibrary.o moc_flibrary.cpp

moc_fartmanager.o: moc_fartmanager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_fartmanager.o moc_fartmanager.cpp

moc_ffiledownloader.o: moc_ffiledownloader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ffiledownloader.o moc_ffiledownloader.cpp

moc_fdbupdater.o: moc_fdbupdater.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_fdbupdater.o moc_fdbupdater.cpp

moc_fclientupdater.o: moc_fclientupdater.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_fclientupdater.o moc_fclientupdater.cpp

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/lib/ || $(MKDIR) $(INSTALL_ROOT)/usr/lib/ 
	-$(INSTALL_PROGRAM) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET)"
	-$(STRIP) --strip-unneeded "$(INSTALL_ROOT)/usr/lib/$(TARGET)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET0)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET1)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET2)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET)" 
	 -$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET0)" 
	 -$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET1)" 
	 -$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET2)"
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/lib/ 


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:

