#include "ProcessRunner.hpp"
#include <QDebug>

ProcessRunner::ProcessRunner(QString nick, QStringList rawCommand, QObject *parent) :
    QProcess(parent), nick(nick), rawCommand(rawCommand)
{
    connect(this, SIGNAL(finished(int)), this, SLOT(commandFinished(int)));
    connect(this, SIGNAL(readyRead()), this, SLOT(readOutput()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(printError(QProcess::ProcessError)));
}

void ProcessRunner::commandFinished(int exitCode)
{
    QString finished = "Command \"" + rawCommand.at(0) + "\" finished with exit code " + QString::number(exitCode);
    qDebug() << finished;

    while(this->canReadLine())
    {
        readOutput();
    }
    emit(printLine(nick, finished));
    emit(commandDone(exitCode));
}
void ProcessRunner::run()
{
    qDebug() << "[PR] run()";
    qDebug() << "[Bot] Starting process " << rawCommand;

    QStringList args = rawCommand;
    args.removeFirst();

    this->start(rawCommand.at(0), args);
}

void ProcessRunner::readOutput()
{
    qDebug() << "[PR] Read()";
    emit(printLine(nick, QString(this->readLine())));
}

void ProcessRunner::printError(QProcess::ProcessError perr)
{
    QString error = "[ERROR] Command: " + rawCommand.at(0) + " Process ";
    switch(perr)
    {
        case QProcess::FailedToStart: error += "failed to start."; break;
        case QProcess::Crashed: error += "crashed."; break;
        case QProcess::Timedout: error += "timed out."; break;
        case QProcess::WriteError: error += "encountered a write error."; break;
        case QProcess::ReadError: error += "encountered a read error."; break;
        case QProcess::UnknownError: error += "encountered an unknown error."; break;
    }
    emit(printLine(nick, error));
}
