#-------------------------------------------------
#
# Project created by QtCreator 2014-02-25T22:08:27
#
#-------------------------------------------------

CONFIG += communi c++11
COMMUNI += core util
DEFINES += IRC_SHARED

INCLUDEPATH += /usr/local/include/IrcCore
INCLUDEPATH += /usr/local/include/IrcModel
INCLUDEPATH += /usr/local/include/IrcUtil

LIBPATH += /usr/local/lib
LIBS += -lIrcCore
LIBS += -lIrcUtil

INCLUDEPATH += /usr/local/include/

IRIS_PLUGINS_DIR = /usr/local/share/iris/plugins/
