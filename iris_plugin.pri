include(/usr/local/share/iris/iris_common.pri)

TEMPLATE = lib

HEADERS += $$INCLUDEPATH/iris/plugin_interfaces.hpp>

unix {
    target.path = /usr/local/share/iris/plugins/
    INSTALLS += target
}

LIBS += -lIris

CONFIG +=  c++11 plugin
OTHER_FILES += plugin.json

