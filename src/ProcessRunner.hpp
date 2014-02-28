#ifndef PROCESSRUNNER_HPP
#define PROCESSRUNNER_HPP

#include <QProcess>
#include <QStringList>

class ProcessRunner : public QProcess
{
private:
    Q_OBJECT
    QString nick;
    QStringList rawCommand;

public:
    explicit ProcessRunner(QString nick, QStringList command, QObject *parent = 0);

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
