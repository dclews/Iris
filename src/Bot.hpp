#ifndef BOT_HPP
#define BOT_HPP

#include <QObject>
#include <QString>
#include <QMap>
#include <IrcConnection>
#include <IrcCommandParser>

#include "CommandPlugin.hpp"

namespace iris
{
    class CommandPluginFactory;
    class ListenerCommand;

    class Bot : public IrcConnection
    {
    private:
        Q_OBJECT
        QList<QString> authenticatedUsers;
        QList<QString> ignoredUsers;
        QMap<QString, ListenerCommand*> listenerCommands;
        QMap<QString, CommandPluginFactory*> pluginCommandFactories;

        bool authenticate(QString nick, QString pass);
        bool isAuthorised(QString nick);
        bool print;
        IrcCommandParser parser;

    public:
        explicit Bot(QObject *parent = 0);
        void registerCommandPlugin(CommandPlugin* plugin);
    signals:

    public slots:
        void processPrivateMessage(IrcPrivateMessage* message);
        void SendPrivateMessage(QString nick, QString message);
        void parrot(IrcPrivateMessage* message);
        void joinChannel(QString room);
        QString toLeet(QString string);
        void sendAuthFail(QString target, QString nick);
        void sendAuthSuccess(QString target, QString nick);
        void setListen(QString user, bool listen);
        void deauth(QString user);
    };
}
#endif // BOT_HPP
