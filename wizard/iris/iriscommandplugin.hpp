#ifndef %PluginName%Plugin_%CppHeaderSuffix:u%
#define %PluginName%Plugin_%CppHeaderSuffix:u%

#include <QObject>
#include <QtPlugin>

#include <iris/CommandPlugin.hpp>

namespace iris
{
    class %PluginName%Plugin : public QObject, public CommandPlugin
    {
    private:
        Q_OBJECT
        Q_PLUGIN_METADATA(IID CommandPlugin_iid FILE "plugin.json")
        Q_INTERFACES(CommandPlugin)
    public:
        %PluginName%Plugin();
        // CommandPlugin interface
        ~%PluginName%Plugin();
        QList<CommandPluginFactory*> commands() const;
    };
}

#endif // %PluginName%Plugin_%CppHeaderSuffix:u%"
