#include "Bot.hpp"
#include <IrcCommand>
#include <QDebug>
#include "ProcessRunner.hpp"
#include <QThread>

Bot::Bot(QObject *parent) :
    IrcConnection(parent)
{
    connect(this, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(processPrivateMessage(IrcPrivateMessage*)));
}
void Bot::processPrivateMessage(IrcPrivateMessage* privMessage)
{
    qDebug() << privMessage->nick() << ":" << privMessage->content();

    if(privMessage->content().startsWith('!'))
    {
        processCommand(privMessage->nick(), privMessage->content());
    }
}
void Bot::parrot(IrcPrivateMessage* message)
{
    if(IrcPrivateMessage* privMessage = qobject_cast<IrcPrivateMessage*>(message))
    {
        sendCommand(IrcCommand::createMessage(privMessage->nick(), privMessage->content()));
    }
}
void Bot::processCommand(QString nick, QString content)
{
    QString command = content.section(" ", 0);
    QString rawArgs = content.section(" ", 1);
    QStringList args = rawArgs.split(" ");

    if(content == "!parrot")
    {
        qDebug() << "[Bot] Enabling parrot mode.";
        connect(this, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(parrot(IrcPrivateMessage*)));
    }
    else if(content.startsWith("!leet"))
    {
        if(args.isEmpty())
        {
            SendPrivateMessage(nick, "Malformed syntax. Command requires arguments.");
        }
        else
        {
            QString leet = rawArgs;
            leet.replace('e', '3').replace('a', '4');
            SendPrivateMessage(nick, leet);
        }
    }
    else if(content.startsWith("!exec"))
    {
        if(!isAuthorised(nick))
        {
            SendPrivateMessage(nick, "Authorisation required.");
        }
        else
        {
            QStringList cmdArgs;
            for(int i=1;i<args.length();i++)
            {
                cmdArgs << args[i];
            }

            qDebug() << "[Bot] Starting process " << args[0] << cmdArgs;

            QThread* thread = new QThread;
            ProcessRunner* pr = new ProcessRunner(nick, args[0], cmdArgs);
            pr->moveToThread(thread);
            connect(pr, SIGNAL(printLine(QString, QString)), this, SLOT(SendPrivateMessage(QString ,QString)));
            connect(thread, SIGNAL(started()), pr, SLOT(run()));
            connect(pr, SIGNAL(commandDone(int)), thread, SLOT(quit()));
            connect(pr, SIGNAL(commandDone(int)), pr, SLOT(deleteLater()));
            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

            thread->start();
        }
    }
    else if(content.startsWith("!auth"))
    {
        if(args.size() == 1)
        {
            QString resp = authenticate(nick, args[0]) ? "Authentication Sucessful." : "Authentication Failed";
            SendPrivateMessage(nick, resp);
        }
        else
        {
            SendPrivateMessage(nick, "Malformed syntax. Usage: !auth password");
        }
    }
}
void Bot::SendPrivateMessage(QString nick, QString message)
{
    this->sendCommand(IrcCommand::createMessage(nick, message));
}
bool Bot::isAuthorised(QString nick)
{
    return authenticatedUsers.contains(nick);
}
bool Bot::authenticate(QString nick, QString pass)
{
    bool authentic = false;
    if(isAuthorised(nick))
    {
        authentic = true;
    }
    else if(pass == "MAGIC_PASSWORD")
    {
        authenticatedUsers << nick;
        authentic = true;
    }
    return authentic;
}
