#ifndef COMMANDPLUGIN_HPP
#define COMMANDPLUGIN_HPP

#include <QObject>
#include <QList>
#include "CommandPluginFactory.hpp"

namespace iris
{
    class ASyncCommand;
}

class CommandPlugin
{
public:
    virtual ~CommandPlugin() {}
    virtual QList<iris::CommandPluginFactory*> commands() const = 0;
};

#define CommandPlugin_iid "xclueless.ircbot.CommandPlugin"
Q_DECLARE_INTERFACE(CommandPlugin, CommandPlugin_iid)

#endif // COMMANDPLUGIN_HPP
