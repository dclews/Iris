#include "%CommandName%.%CppHeaderSuffix%"

namespace iris
{
    QString %CommandName%::mSyntax = "%CommandName% <command> (<args...>)";

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
