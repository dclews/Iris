#include <QCoreApplication>
#include <IrcConnection>
#include "Bot.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString name("POSIX_ME_HARDER");
    Bot bot;
    bot.setUserName(name);
    bot.setNickName(name);
    bot.setRealName(name);
    bot.setHost("irc.w3.org");
    bot.setPort(6665);
    bot.joinChannel("#ec");
    bot.open();

    return a.exec();
}
