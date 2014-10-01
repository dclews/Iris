#include "ASyncCommand.hpp"

namespace iris
{
	ASyncCommand::ASyncCommand()
	{
	}
	ASyncCommand::~ASyncCommand()
	{
	}
	void ASyncCommand::setCommand(IrcCommand* command)
	{
		this->mParameters = QStringList(command->parameters());
	}
	void ASyncCommand::setTarget(QString target)
	{
		this->mTarget = target;
	}
}
