#ifndef BOT_HPP
#define BOT_HPP

#include <QObject>
#include <QString>
#include <IrcConnection>
#include <IrcCommandParser>

class Bot : public IrcConnection
{
private:
    Q_OBJECT
    QList<QString> authenticatedUsers;
    QList<QString> ignoredUsers;
    bool authenticate(QString nick, QString pass);
    bool isAuthorised(QString nick);
    bool print;
    IrcCommandParser parser;

public:
    explicit Bot(QObject *parent = 0);
signals:

public slots:
    void processPrivateMessage(IrcPrivateMessage* message);
    void SendPrivateMessage(QString nick, QString message);
    void parrot(IrcPrivateMessage* message);
    void joinChannel(QString room);
    void exec(QString nick, QString target, QStringList command);
    QString toLeet(QString string);
    void sendAuthFail(QString target, QString nick);
    void sendAuthSuccess(QString target, QString nick);
    void setListen(QString user, bool listen);
    void deauth(QString user);
};

#endif // BOT_HPP
