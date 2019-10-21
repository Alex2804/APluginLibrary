#include "pluginmanagerprivate.h"

apl::detail::PluginInstance::PluginInstance(std::unique_ptr<Plugin> plugin)
    : plugin(std::move(plugin))
    , featureCount(this->plugin->getFeatureCount())
    , classCount(this->plugin->getClassCount())
    , featureInfos(featureCount > 0 ? this->plugin->getFeatureInfos() : nullptr)
    , classInfos(classCount > 0 ? this->plugin->getClassInfos() : nullptr)
{
    PluginManagerPrivate::loadedPlugins.insert(std::make_pair<Plugin*, PluginInstance*>(this->plugin.get(), this));
}
apl::detail::PluginInstance::~PluginInstance()
{
    PluginManagerPrivate::loadedPlugins.erase(plugin.get());
}


std::unordered_map<apl::Plugin*, apl::detail::PluginInstance*> apl::detail::PluginManagerPrivate::loadedPlugins;