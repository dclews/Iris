#ifndef ASYNC_HPP
#define ASYNC_HPP

#include <QObject>
#include <IrcCommand>
#include <QProcess>
#include <QStringList>

class IrcMessage;

namespace iris
{
    class ASyncCommand : public QObject
    {
    private:
        Q_OBJECT
    protected:
        QStringList parameters;
        QString target;
    public:
        ASyncCommand();
        virtual ~ASyncCommand();
    signals:
        void printLine(QString nick, QString line);
        void commandDone(int exitCode);
    public slots:
        virtual void exec()=0;
        virtual QString syntax()=0;
        virtual void setCommand(IrcCommand* cmd);
        virtual void setTarget(QString target);
    };
}

#endif // ASYNC_HPP
