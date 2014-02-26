#include <QCoreApplication>
#include "Bot.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString name("Macaw");
    Bot bot;
    bot.setUserName(name);
    bot.setNickName(name);
    bot.setRealName(name);
    bot.setHost("irc.w3.org");
    bot.setPort(6665);
    bot.open();

    return a.exec();
}
