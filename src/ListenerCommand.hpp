#include "ASyncCommand.hpp"

#ifndef LISTENERCOMMAND_HPP
#define LISTENERCOMMAND_HPP

class IrcPrivateMessage;

namespace iris
{
	class ListenerCommand : public ASyncCommand
	{
	private:
		Q_OBJECT
	public:
		ListenerCommand();
		virtual ~ListenerCommand() {}
	signals:

	public slots:
		virtual void messageRecieved(IrcPrivateMessage* msg)=0;
	};
}


#endif // LISTENERCOMMAND_HPP
