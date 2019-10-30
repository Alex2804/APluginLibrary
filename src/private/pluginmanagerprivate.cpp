#include "pluginmanagerprivate.h"

#include <algorithm>

apl::detail::PluginInstance::PluginInstance(std::unique_ptr<Plugin> plugin)
    : plugin(std::move(plugin))
    , featureCount(this->plugin->getFeatureCount())
    , classCount(this->plugin->getClassCount())
    , featureInfos(featureCount > 0 ? this->plugin->getFeatureInfos() : nullptr)
    , classInfos(classCount > 0 ? this->plugin->getClassInfos() : nullptr)
{}
apl::detail::PluginInstance::~PluginInstance()
{
    PluginManagerPrivate::mutex.lock();
    PluginManagerPrivate::loadedPlugins.erase(plugin->getHandle());
    PluginManagerPrivate::mutex.unlock();
}


std::unordered_map<apl::const_library_handle, std::weak_ptr<apl::detail::PluginInstance>>
                                                                    apl::detail::PluginManagerPrivate::loadedPlugins;
std::mutex apl::detail::PluginManagerPrivate::mutex;

bool apl::detail::PluginManagerPrivate::loadPlugin(std::string path)
{
    library_handle tmpHandle = LibraryLoader::load(path);
    mutex.lock();
    if(loadedPlugins.find(tmpHandle) != loadedPlugins.end()) {
        pluginInstances.push_back(loadedPlugins.at(tmpHandle).lock());
    } else {
        std::unique_ptr<Plugin> plugin = std::unique_ptr<Plugin>(Plugin::load(std::move(path)));
        if(plugin == nullptr) {
            mutex.unlock();
            return false;
        }
        std::shared_ptr<PluginInstance> instance = std::make_shared<PluginInstance>(std::move(plugin));
        pluginInstances.push_back(instance);
        loadedPlugins.insert({instance->plugin->getHandle(), instance});
    }
    mutex.unlock();
    LibraryLoader::unload(tmpHandle);
    return true;
}
void apl::detail::PluginManagerPrivate::unloadPlugin(apl::Plugin* plugin)
{
    if(plugin == nullptr)
        return;
    std::shared_ptr<PluginInstance> instance;
    mutex.lock();
    if(loadedPlugins.find(plugin->getHandle()) != loadedPlugins.end()) {
        instance = std::shared_ptr<PluginInstance>(loadedPlugins[plugin->getHandle()]);
        auto func = [instance] (const std::shared_ptr<detail::PluginInstance>& sharedPtr) -> bool {
            return sharedPtr == instance;
        };
        pluginInstances.erase( std::remove_if(pluginInstances.begin(), pluginInstances.end(), func), pluginInstances.end());
    }
    mutex.unlock();
}
