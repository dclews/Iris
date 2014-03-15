#-------------------------------------------------
#
# Project created by QtCreator 2014-02-25T22:08:27
#
#-------------------------------------------------

CONFIG += communi c++11
COMMUNI += core util
DEFINES += IRC_SHARED

unix {
    LIBPATH += /usr/local/lib
    INCLUDEPATH += /usr/local/include/IrcCore
    INCLUDEPATH += /usr/local/include/IrcModel
    INCLUDEPATH += /usr/local/include/IrcUtil

    IRIS_SHARE = /usr/local/share/iris
    INCLUDEPATH += /usr/local/include/

}
win {
    IRIS_ROOT = "C:\Program Files\Iris"
    IRIS_SHARE = "$$IRIS_ROOT\share"
    INCLUDEPATH += "$$IRIS_ROOT\include"
}

LIBS += -lIrcCore
LIBS += -lIrcUtil


IRIS_PLUGINS_DIR = $$IRIS_SHARE/plugins/
