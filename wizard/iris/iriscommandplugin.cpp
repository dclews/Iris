#include "%PluginName%Plugin.%CppHeaderSuffix%"

namespace iris
{
    %PluginName%Plugin::%PluginName%Plugin()
    {
    }

    %PluginName%Plugin::~%PluginName%Plugin()
    {
    }

    QList<CommandPluginFactory*> %PluginName%Plugin::commands() const
    {
        QList<CommandPluginFactory*> commands;
        //commands << new AppendCommandFactoryHere;
        return commands;
    }
}
