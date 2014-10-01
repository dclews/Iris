#ifndef PLUGINFACTORY_HPP
#define PLUGINFACTORY_HPP

#include "ASyncCommand.hpp"

class QString;

namespace iris
{
	enum command_type
	{
		OneShot,
		AuthManager,
		Listener
	};

	class CommandPluginFactory
	{
	public:
		virtual ~CommandPluginFactory() {}
		virtual ASyncCommand* newCommand()=0;
		virtual QString syntax()=0;
		virtual command_type commandType()=0;
	};
}

#endif // PLUGINFACTORY_HPP
