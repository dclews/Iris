#ifndef BOT_HPP
#define BOT_HPP

#include <QObject>
#include <QString>
#include <QMap>
#include <IrcConnection>
#include <IrcCommandParser>
#include "AuthenticationManager.hpp"

#include "CommandPlugin.hpp"

namespace iris
{
	class CommandPluginFactory;
	class ListenerCommand;

	class Bot : public IrcConnection
	{
	private:
		Q_OBJECT
		QList<QString> ignoredUsers;
		QMap<QString, ListenerCommand*> listenerCommands;

		QSet<CommandPlugin*> mPlugins;

		QMap<QString, CommandPluginFactory*> pluginCommandFactories;
		AuthenticationManager* mAuthorisationManager;

		bool mPrint;
		IrcCommandParser parser;

	public:
		explicit Bot(QObject *parent = 0);
		void registerCommandPlugin(CommandPlugin* plugin);
		bool authoriseCommand(QString user, QString command);
		void performCommand(QString target, QString nick, IrcCommand* cmd);


	public slots:
		void dispatchMessage(QString target, QString message);
		void processPrivateMessage(IrcPrivateMessage* message);
		void parrot(IrcPrivateMessage* message);
		void joinChannel(QString room);
		void setListen(QString user, bool listen);
	};
}
#endif // BOT_HPP
