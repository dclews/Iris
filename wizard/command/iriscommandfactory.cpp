#include "%CommandName%Factory.%CppHeaderSuffix%"
#include "%CommandName%.%CppHeaderSuffix%"

namespace iris
{
    %CommandName%Factory::%CommandName%Factory()
    {
    }
    ASyncCommand* %CommandName%Factory::newCommand()
    {
        return new %CommandName%;
    }

    QString %CommandName%Factory::syntax()
    {
        return QString("%CommandName:u% <command> (<args...>)");
    }
}
