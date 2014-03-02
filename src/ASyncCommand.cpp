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
        this->parameters = QStringList(command->parameters());
    }
    void ASyncCommand::setTarget(QString target)
    {
        this->target = target;
    }
}
