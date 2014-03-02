#include "%CommandName%.%CppHeaderSuffix%"

namespace iris
{
    %CommandName%::%CommandName%()
    {
    }
    void %CommandName%::exec()
    {
    }
    QString %CommandName%::syntax()
    {
        return QString("%CommandName% <command> (<args...>)");
    }
}
