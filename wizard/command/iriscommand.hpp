#ifndef %CommandName:u%_%CppHeaderSuffix:u%
#define %CommandName:u%_%CppHeaderSuffix:u%

#include <QObject>
#include <iris/ASyncCommand.hpp>

namespace iris
{
    class %CommandName% : public ASyncCommand
    {
    private:
        Q_OBJECT
    public:
        explicit %CommandName%();

    signals:
        void printLine(QString nick, QString line);
        void commandDone(int exitCode);

    public slots:
        // ASyncCommand interface
        QString syntax();
        void exec();
    };
}

#endif // %CommandName:u%_%CppHeaderSuffix:u%
