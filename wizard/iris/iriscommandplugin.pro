#-------------------------------------------------
#
# Project created by QtCreator 2014-03-02T03:51:34
#
#-------------------------------------------------

include(/usr/local/share/iris/iris_plugin.pri)

TARGET = $$qtLibraryTarget(xclueless.iris.%PluginName%)

LIBS += -lIris

QT       -= gui

CONFIG +=  c++11 plugin
SOURCES += %PluginName%Plugin.%CppSourceSuffix%
HEADERS += %PluginName%Plugin.%CppHeaderSuffix%