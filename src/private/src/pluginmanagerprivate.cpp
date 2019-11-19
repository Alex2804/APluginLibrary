#include "../pluginmanagerprivate.h"

#include <algorithm>

#include "APluginLibrary/pluginmanager.h"

apl::detail::PluginInstance::PluginInstance(Plugin* plugin)
    : plugin(plugin)
{}
apl::detail::PluginInstance::~PluginInstance()
{
    PluginManagerPrivate::mutex.lock();
    PluginManagerPrivate::loadedPlugins.erase(plugin->getHandle());
    PluginManagerPrivate::mutex.unlock();
    delete plugin;
}


std::unordered_map<apl::const_library_handle, std::weak_ptr<apl::detail::PluginInstance>>
                                                                    apl::detail::PluginManagerPrivate::loadedPlugins;
std::mutex apl::detail::PluginManagerPrivate::mutex;

bool apl::detail::PluginManagerPrivate::loadPlugin(std::string path)
{
    library_handle tmpHandle = LibraryLoader::load(path);
    if (!tmpHandle)
        return false;
    mutex.lock();
    auto iterator = loadedPlugins.find(tmpHandle);
    LibraryLoader::unload(tmpHandle);
    if(iterator != loadedPlugins.end()) {
        std::shared_ptr<PluginInstance> instance = iterator->second.lock();
        if(std::find(pluginInstances.begin(), pluginInstances.end(), instance) == pluginInstances.end()) {
            pluginInstances.push_back(instance);
        }
    } else {
        Plugin* plugin = Plugin::load(std::move(path));
        if(plugin == nullptr) {
            mutex.unlock();
            return false;
        }
        std::shared_ptr<PluginInstance> instance = std::make_shared<PluginInstance>(plugin);
        pluginInstances.push_back(instance);
        loadedPlugins.insert({instance->plugin->getHandle(), instance});
    }
    mutex.unlock();
    return true;
}
void apl::detail::PluginManagerPrivate::unloadPlugin(apl::Plugin* plugin)
{
    if(plugin == nullptr)
        return;
    std::shared_ptr<PluginInstance> instance;
    mutex.lock();
    auto iterator = loadedPlugins.find(plugin->getHandle());
    if(iterator != loadedPlugins.end()) {
        instance = iterator->second.lock();
        auto func = [instance] (const std::shared_ptr<detail::PluginInstance>& sharedPtr) -> bool {
            return sharedPtr == instance;
        };
        pluginInstances.erase( std::remove_if(pluginInstances.begin(), pluginInstances.end(), func), pluginInstances.end());
    }
    mutex.unlock();
}

std::string apl::detail::filterPluginInfo(const PluginInfo *info, PluginInfoFilter filter)
{
    if(filter == PluginInfoFilter::PluginName) {
        return info->pluginName;
    } else if(filter == PluginInfoFilter::PluginVersion) {
        return std::to_string(info->pluginVersionMajor).append(".").append(std::to_string(info->pluginVersionMinor)).append(".").append(std::to_string(info->pluginVersionPatch));
    } else if(filter == PluginInfoFilter::ApiVersion) {
        return std::to_string(info->apiVersionMajor).append(".").append(std::to_string(info->apiVersionMinor)).append(".").append(std::to_string(info->apiVersionPatch));
    } else {
        throw std::runtime_error("Unsupported apl::PluginInfoFilter");
    }
}
const char* apl::detail::filterFeatureInfo(const PluginFeatureInfo *info, const PluginFeatureFilter filter)
{
    if(filter == PluginFeatureFilter::FeatureGroup) {
        return info->featureGroup;
    } else if(filter == PluginFeatureFilter::FeatureName) {
        return info->featureName;
    } else if(filter == PluginFeatureFilter::ReturnType) {
        return info->returnType;
    } else if(filter == PluginFeatureFilter::ParameterList) {
        return info->parameterList;
    } else if (filter == PluginFeatureFilter::ParameterTypes) {
        return info->parameterTypes;
    } else if (filter == PluginFeatureFilter::ParameterNames) {
        return info->parameterNames;
    } else {
        throw std::runtime_error("Unsupported apl::PluginFeatureFilter");
    }
}
const char* apl::detail::filterClassInfo(const PluginClassInfo* info, PluginClassFilter filter)
{
    if(filter == PluginClassFilter::InterfaceName) {
        return info->interfaceName;
    } else if(filter == PluginClassFilter::ClassName) {
        return info->className;
    } else {
        throw std::runtime_error("Unimplemented apl::PluginFeatureFilter");
    }
}