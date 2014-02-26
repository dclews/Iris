#ifndef BOT_HPP
#define BOT_HPP

#include <QObject>
#include <QString>
#include <IrcConnection>

class Bot : public IrcConnection
{
private:
    Q_OBJECT
    QList<QString> authenticatedUsers;
    bool authenticate(QString nick, QString pass);
    void parrot(IrcPrivateMessage* message);
    bool isAuthorised(QString nick);

public:
    explicit Bot(QObject *parent = 0);
signals:

public slots:
    void processPrivateMessage(IrcPrivateMessage* privMessage);
    void processCommand(QString nick, QString content);
    void SendPrivateMessage(QString nick, QString message);
};

#endif // BOT_HPP
