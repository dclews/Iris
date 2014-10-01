#include "Bot.hpp"

#include <IrcCommand>
#include <QDebug>
#include <QThread>
#include <QEventLoop>
#include "CommandPluginFactory.hpp"
#include "ASyncCommand.hpp"
#include "ListenerCommand.hpp"

namespace iris
{
	Bot::Bot(QObject *parent) :
		IrcConnection(parent), mAuthorisationManager(NULL), mPrint(true)
	{
		connect(this, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(processPrivateMessage(IrcPrivateMessage*)));
	//    parser.addCommand(IrcCommand::CtcpAction, "ACT [target] <message...>");
		parser.addCommand(IrcCommand::Nick, "NICK <nick>");
		parser.addCommand(IrcCommand::Join, "JOIN <#channel> (<key>)");
	//    parser.addCommand(IrcCommand::Part, "PART (<#channel>) (<message...>)");
		parser.addCommand(IrcCommand::Quit, "QUIT (<message...>)");
		parser.addCommand(IrcCommand::Message, "SAY [target] <message...>");
		parser.addCommand(IrcCommand::Custom, "PARROT <target>");
		parser.addCommand(IrcCommand::Custom, "LISTEN <target> <state>");
		parser.addCommand(IrcCommand::Custom, "SAYIN <target> <words...>");
		parser.addCommand(IrcCommand::Custom, "PRINT <state>");
		parser.addCommand(IrcCommand::Custom, "CMDS");
	}
	void Bot::registerCommandPlugin(CommandPlugin* plugin)
	{
		mPlugins.insert(plugin);
		for(CommandPluginFactory* customCommandFactory : plugin->commands())
		{
			QString commandName = customCommandFactory->syntax().section(' ', 0, 0);

			if(customCommandFactory->commandType() == command_type::AuthManager)
			{
				ASyncCommand* authBase = customCommandFactory->newCommand();
				AuthenticationManager* authManTemp = qobject_cast<AuthenticationManager*>(authBase);
				if(authManTemp)
				{
					mAuthorisationManager = authManTemp;
					qDebug() << "Registered new authentication manager:" << mAuthorisationManager->syntax();
					parser.addCommand(IrcCommand::Custom, customCommandFactory->syntax());
					pluginCommandFactories[commandName] = customCommandFactory;
					mAuthorisationManager->exec(); //Initialise it.
				}
				else
				{
					qDebug() << "[Error] Class falsely indentifies as an authentication manager. Not loading.";
					delete authBase;
				}
			}
			else if(customCommandFactory->commandType() == command_type::Listener)
			{
				ASyncCommand* listenerBase = customCommandFactory->newCommand();
				ListenerCommand* listener = qobject_cast<ListenerCommand*>(listenerBase);

				if(listener)
				{
					listenerCommands[commandName] = listener;
					connect(this, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), listener, SLOT(messageRecieved(IrcPrivateMessage*)));
					connect(listener, SIGNAL(printLine(QString,QString)), this, SLOT(dispatchMessage(QString,QString)));

					parser.addCommand(IrcCommand::Custom, customCommandFactory->syntax());
					qDebug() << "Registered listener class:" << customCommandFactory->syntax();
					pluginCommandFactories[commandName] = customCommandFactory;
					listener->exec(); //Initialise it.
				}
				else
				{
					qDebug() << "[Error] Class falsely indentifies as a listener. Not loading.";
					delete listenerBase;
				}
			}
			else if(customCommandFactory->commandType() == OneShot)
			{
				parser.addCommand(IrcCommand::Custom, customCommandFactory->syntax());
				qDebug() << "Registered one shot command class. Class provides:" << customCommandFactory->syntax();
				pluginCommandFactories[commandName] = customCommandFactory;
			}
			else
			{
				qWarning() << "Ignoring class as it does not have an indentifiable type:" << customCommandFactory->syntax();
			}
		}
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
			QString command = cmd->parameters().value(0);

			if(authoriseCommand(nick, command))
			{
				performCommand(target, nick, cmd);
			}
			else
			{
				dispatchMessage(target, "Sorry " + nick + ", you are not authorised to use that command.");
			}
		}
	}

	void Bot::performCommand(QString target, QString nick, IrcCommand* cmd)
	{
		QStringList params = cmd->parameters();
		QString command = cmd->parameters().value(0);

		if(cmd->type() == IrcCommand::Custom)
		{
			if(command == "AUTH")
			{
				mAuthorisationManager->loginUser(nick, params) ? dispatchMessage(nick, "Authentication successful.") : dispatchMessage(nick, "Authentication failed.");
			}
			else if(command == "CMDS")
			{
				foreach(QString s, parser.commands())
				{
					dispatchMessage(target, s);
				}
				foreach(CommandPlugin* plugin, mPlugins)
				{
					foreach(CommandPluginFactory* factory, plugin->commands())
					{
						dispatchMessage(target, "[" + plugin->name() + "] " + factory->syntax());
					}
				}
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
				dispatchMessage(params.at(1), params.join(' ').section(' ', 2));
			}
			else if(command == "PRINT")
			{
				mPrint = params.value(1) == "ON";
				qDebug() << "Print set to " << mPrint;
			}
			else
			{
				CommandPluginFactory* factory = pluginCommandFactories[command];
				if(factory)
				{
					ASyncCommand* asyncCmd = factory->newCommand();
					QThread* thread = new QThread;
					if(asyncCmd)
					{
						asyncCmd->setCommand(cmd);
						asyncCmd->setTarget(target);

						connect(asyncCmd, SIGNAL(printLine(QString,QString)), this, SLOT(dispatchMessage(QString,QString)));
						connect(thread, SIGNAL(started()), asyncCmd, SLOT(exec()));
						connect(asyncCmd, SIGNAL(commandDone(int)), thread, SLOT(quit()));
						connect(asyncCmd, SIGNAL(commandDone(int)), asyncCmd, SLOT(deleteLater()));
						connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

						thread->start();
					}
					else
					{
						qDebug() << "Error. ayncCmd is null!";
					}
				}
			}
		}
		else
		{
			sendCommand(cmd);
		}
	}

	void Bot::dispatchMessage(QString target, QString message)
	{
		qDebug() << "iris >>" << target << ":" << message;
		if(mPrint)
		{
			this->sendCommand(IrcCommand::createMessage(target, message));
		}
	}

	bool Bot::authoriseCommand(QString user, QString command)
	{
		if(mAuthorisationManager == NULL)
		{
			qWarning() << "No authorisation manager installed. Denying access to command."; //Make this configurable.
			return false;
		}

		return mAuthorisationManager->checkUserHasPermissionsForCommand(user, command);
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
		qDebug() << "[Bot] Joining channel" << channel;
		sendCommand(IrcCommand::createJoin(channel));
	}
	void Bot::setListen(QString user, bool listen)
	{
		bool ignored = ignoredUsers.contains(user);

		if(listen && ignored)
		{
			qDebug() << "Listening to user" << user;
			ignoredUsers.removeOne(user);
		}
		else if(!listen && !ignored)
		{
			qDebug() << "Ignoring user" << user;
			ignoredUsers.append(user);
		}
	}
}
