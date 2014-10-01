#-------------------------------------------------
#
# Project created by QtCreator 2014-02-25T22:08:27
#
#-------------------------------------------------

QT       += core network xml
QT       -= gui

QMAKE_CXXFLAGS += -fPIC

include(iris_common.pri)

TARGET = Iris
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = lib

SOURCES += src/Bot.cpp \
           src/ASyncCommand.cpp \
           src/ListenerCommand.cpp \
           src/AuthenticationManager.cpp

HEADERS += src/Bot.hpp \
    src/CommandPlugin.hpp \
    src/ASyncCommand.hpp \
    src/PluginInterfaces.hpp \
    src/CommandPluginFactory.hpp \
    src/iris.hpp \
    src/ListenerCommand.hpp \
    src/AuthenticationManager.hpp

unix {
    target.path = /usr/local/lib/
    headers.path = /usr/local/include/iris
    headers.files += $$HEADERS
#    mics.path = /usr/local/share/iris
#    mics.files += $$PWD/iris_common.pri

    INSTALLS += target headers misc
}
