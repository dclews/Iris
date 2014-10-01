#ifndef %CommandName:u%FACTORY_%CppHeaderSuffix:u%
#define %CommandName:u%FACTORY_%CppHeaderSuffix:u%

#include <QObject>
#include <iris/CommandPluginFactory.hpp>

namespace iris
{
    class %CommandName%Factory : public QObject, public CommandPluginFactory
    {
        Q_OBJECT
    public:
        explicit %CommandName%Factory();
    public slots:
        ASyncCommand* newCommand();
        QString syntax();
    };
}


#endif // %CommandName:u%FACTORY_%CppHeaderSuffix:u%
