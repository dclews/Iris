#-------------------------------------------------
#
# Project created by QtCreator 2014-03-02T03:51:34
#
#-------------------------------------------------

//DO-CONFIG-HERE

QT       -= gui

CONFIG +=  c++11 plugin
SOURCES +=  %Name%Plugin.%CppSourceSuffix%

HEADERS += %Name%Plugin.%CppHeaderSuffix%

PRO_PATH=$$PWD
include(/usr/local/share/iris/iris.pri)

//COMMUNI-REQUIRED
CONFIG += communi
COMMUNI += core model util
DEFINES += IRC_SHARED

INCLUDEPATH += /usr/local/include/IrcCore
LIBPATH += /usr/local/lib
LIBS += -lIrcCore
//COMMUNI-REQUIRED


//PLUGIN-REQUIRED
SOURCES += ../Core/src/ASyncCommand.cpp

HEADERS += ../Core/src/ASyncCommand.hpp

OTHER_FILES += \
    plugin.json

TARGET = $$qtLibraryTarget(xclueless.ircbot.CommandPlugin)
TEMPLATE = lib
//PLUGIN-REQUIRED

unix {
    target.path = /usr/local/share/iris/plugins/
    INSTALLS += target
}

