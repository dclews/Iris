#ifndef PROCESSRUNNER_HPP
#define PROCESSRUNNER_HPP

#include <QProcess>
#include <QStringList>

class ProcessRunner : public QProcess
{
private:
    Q_OBJECT
    QString nick;
    QString command;
    QStringList args;

public:
    explicit ProcessRunner(QString nick, QString command, QStringList args, QObject *parent = 0);

signals:
    void printLine(QString nick, QString line);
    void commandDone(int exitCode);

public slots:
    void commandFinished(int exitCode);
    void run();
    void readOutput();
    void printError(QProcess::ProcessError perr);
};

#endif // PROCESSRUNNER_HPP
