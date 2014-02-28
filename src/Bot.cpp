#include "Bot.hpp"
#include <IrcCommand>
#include <QDebug>
#include "ProcessRunner.hpp"
#include <QThread>
#include <lastfm/User.h>

Bot::Bot(QObject *parent) :
    IrcConnection(parent)
{
    connect(this, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(processPrivateMessage(IrcPrivateMessage*)));
    print = true;
//    parser.addCommand(IrcCommand::CtcpAction, "ACT [target] <message...>");
    parser.addCommand(IrcCommand::Nick, "NICK <nick>");
    parser.addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
//    parser.addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
    parser.addCommand(IrcCommand::Quit, "QUIT (<message...>)");
    parser.addCommand(IrcCommand::Message, "SAY [target] <message...>");

    parser.addCommand(IrcCommand::Custom, "PARROT <target>");
    parser.addCommand(IrcCommand::Custom, "EXEC <command> (<args...>)");
    parser.addCommand(IrcCommand::Custom, "LEET <words...>");
    parser.addCommand(IrcCommand::Custom, "AUTH <password>");
    parser.addCommand(IrcCommand::Custom, "DEAUTH <password>");
    parser.addCommand(IrcCommand::Custom, "LISTEN <target> <state>");
    parser.addCommand(IrcCommand::Custom, "SAYIN <target> <words...>");
    parser.addCommand(IrcCommand::Custom, "PRINT <state>");
    parser.addCommand(IrcCommand::Custom, "CMDS");
}
void Bot::processPrivateMessage(IrcPrivateMessage* message)
{
    if(ignoredUsers.contains(message->nick()))
    {
        return;
    }
    if (message->isPrivate()) {
        qDebug() << message->nick() << ":" << message->content();

        // private message: reply to the message sender
        // => triggers: "!<cmd> <params>" and "<cmd> <params>"
        parser.setTarget(message->nick());
        parser.setTriggers(QStringList() << "!" << "");
    } else {
        // channel message: reply to the target channel
        // => triggers: "!<cmd> <params>" and "bot: <cmd> <params>"
        qDebug() << message->target() << ">" << message->nick() << ":" << message->content();

        parser.setTarget(message->target());
        parser.setTriggers(QStringList() << "!" << nickName().append(":"));
    }

    IrcCommand* cmd = parser.parse(message->content());
    if(cmd) //Got a valid command.
    {
        QString target = parser.target();
        QString nick = message->nick();
        QStringList params = cmd->parameters();
        QString command = cmd->parameters().value(0);

        if(cmd->type() == IrcCommand::Custom)
        {
            if(command == "LEET")
            {
                SendPrivateMessage(target, toLeet(cmd->parameters().join(' ').section(' ', 1)));
            }
            else if(command == "AUTH")
            {
                authenticate(target, cmd->parameters().value(1)) ? sendAuthSuccess(target, nick) : sendAuthFail(target, nick);
            }
            else if(command == "CMDS")
            {
                foreach(QString s, parser.commands())
                {
                    SendPrivateMessage(target, s);
                }
            }
            else //All of these commands require auth.
            {
                if(!isAuthorised(nick))
                {
                    sendAuthFail(target, nick);
                }
                else if(command == "DEAUTH")
                {
                    deauth(params.value(1));
                }
                else if(command == "EXEC")
                {
                    QStringList execCommand = cmd->parameters();
                    execCommand.removeFirst();
                    exec(message->nick(), target, execCommand);
                }
                else if(command == "PARROT")
                {
                    connect(this, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(parrot(IrcPrivateMessage*)));
                }
                else if(command == "LISTEN")
                {
                    bool listen = cmd->parameters().value(2) == "ON";
                    setListen(params.value(1), listen);
                }
                else if(command == "SAYIN")
                {
                    SendPrivateMessage(params.at(1), params.join(' ').section(' ', 2));
                }
                else if(command == "RESERVE")
                {

                }
                else if(command == "PRINT")
                {
                    print = params.value(1) == "ON";
                    qDebug() << "Print set to " << print;
                }
            }
        }
        else if(isAuthorised(nick))
        {
            sendCommand(cmd);
        }
    }
}
void Bot::parrot(IrcPrivateMessage* message)
{
    if(IrcPrivateMessage* privMessage = qobject_cast<IrcPrivateMessage*>(message))
    {
        sendCommand(IrcCommand::createMessage(privMessage->nick(), privMessage->content()));
    }
}
void Bot::joinChannel(QString channel)
{
    qDebug() << "[Bot] Joining channel " << channel;
    sendCommand(IrcCommand::createJoin(channel));
}
void Bot::SendPrivateMessage(QString nick, QString message)
{
    if(print)
    {
        this->sendCommand(IrcCommand::createMessage(nick, message));
    }
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
    else if(pass == "alpine")
    {
        authenticatedUsers << nick;
        authentic = true;
    }
    return authentic;
}
void Bot::deauth(QString user)
{
    if(authenticatedUsers.contains(user))
    {
        authenticatedUsers.removeAll(user);
    }
}
void Bot::exec(QString nick, QString target, QStringList command)
{
    if(!isAuthorised(nick))
    {
        SendPrivateMessage(target, "Authorisation required.");
    }
    else
    {
        QThread* thread = new QThread;
        ProcessRunner* pr = new ProcessRunner(target, command);
        pr->moveToThread(thread);
        connect(pr, SIGNAL(printLine(QString, QString)), this, SLOT(SendPrivateMessage(QString ,QString)));
        connect(thread, SIGNAL(started()), pr, SLOT(run()));
        connect(pr, SIGNAL(commandDone(int)), thread, SLOT(quit()));
        connect(pr, SIGNAL(commandDone(int)), pr, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
    }
}
QString Bot::toLeet(QString string)
{
    return string.replace('e', '3').replace('a', '4').replace('t', '7').replace('l', '1');
}
void Bot::setListen(QString user, bool listen)
{
    bool ignored = ignoredUsers.contains(user);

    if(listen && ignored)
    {
        qDebug() << "Listening to user " << user;
        ignoredUsers.removeOne(user);
    }
    else if(!listen && !ignored)
    {
        qDebug() << "Ignoring user " << user;
        ignoredUsers.append(user);
    }
}
void Bot::sendAuthFail(QString target, QString nick)
{
    SendPrivateMessage(target, "That requires authentication " + nick);
}
void Bot::sendAuthSuccess(QString target, QString nick)
{
    SendPrivateMessage(target, "You are now authenticated " + nick);
}
