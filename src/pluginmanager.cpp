#include "private/pluginmanagerprivate.h"

#include <algorithm>

apl::PluginManager::PluginManager()
    : d_ptr(new detail::PluginManagerPrivate())
{}
apl::PluginManager::PluginManager(const apl::PluginManager &other)
    : PluginManager()
{
    d_ptr->pluginInstances = other.d_ptr->pluginInstances;
}
apl::PluginManager::PluginManager(apl::PluginManager &&other) noexcept
    : PluginManager()
{
    using std::swap;
    swap(d_ptr, other.d_ptr);
}
apl::PluginManager::~PluginManager()
{
    delete d_ptr;
}

apl::PluginManager &apl::PluginManager::operator=(const apl::PluginManager &other)
{
    d_ptr->pluginInstances = other.d_ptr->pluginInstances;
    return *this;
}
apl::PluginManager &apl::PluginManager::operator=(apl::PluginManager &&other) noexcept
{
    using std::swap;
    swap(d_ptr, other.d_ptr);
    return *this;
}

bool apl::PluginManager::load(std::string path)
{
    auto plugin = Plugin::load(std::move(path));
    if(plugin == nullptr)
        return false;
    d_ptr->pluginInstances.push_back(
            std::make_shared<detail::PluginInstance>(std::unique_ptr<Plugin>(plugin)));
    return true;
}

size_t apl::PluginManager::getLoadedPluginCount()
{
    return d_ptr->pluginInstances.size();
}
std::vector<apl::Plugin*> apl::PluginManager::getLoadedPlugins()
{
    std::vector<Plugin*> plugins;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        plugins.push_back(pluginInstance->plugin.get());
    }
    return plugins;
}

void apl::PluginManager::unload(apl::Plugin *plugin)
{
    if (detail::PluginManagerPrivate::loadedPlugins.find(plugin) != detail::PluginManagerPrivate::loadedPlugins.end()) {
        detail::PluginInstance* pluginInstance = detail::PluginManagerPrivate::loadedPlugins.at(plugin);
        auto func = [pluginInstance] (const std::shared_ptr<detail::PluginInstance>& sharedPtr) -> bool {
            return sharedPtr.get() == pluginInstance;
        };
        d_ptr->pluginInstances.erase(
                std::remove_if(d_ptr->pluginInstances.begin(), d_ptr->pluginInstances.end(), func),
                d_ptr->pluginInstances.end());
    }
}
void apl::PluginManager::unloadAll()
{
    d_ptr->pluginInstances.clear();
}

std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures()
{
    std::vector<const PluginFeatureInfo*> features;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        features.insert(features.end(), pluginInstance->featureInfos, pluginInstance->featureInfos + pluginInstance->featureCount);
    }
    return features;
}
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures(const std::string& s, apl::PluginFeatureFilter f)
{
    std::vector<const PluginFeatureInfo*> features;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->featureCount; i++) {
            if((f == PluginFeatureFilter::FeatureGroup && s == pluginInstance->featureInfos[i]->featureGroup)
               || (f == PluginFeatureFilter::FeatureName && s == pluginInstance->featureInfos[i]->featureName)
               || (f == PluginFeatureFilter::ReturnType && s == pluginInstance->featureInfos[i]->returnType)
               || (f == PluginFeatureFilter::ArgumentList && s == pluginInstance->featureInfos[i]->argumentList))
            {
                features.push_back(pluginInstance->featureInfos[i]);
            }
        }
    }
    return features;
}

std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses()
{
    std::vector<const PluginClassInfo*> classes;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        classes.insert(classes.end(), pluginInstance->classInfos, pluginInstance->classInfos + pluginInstance->classCount);
    }
    return classes;
}
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses(const std::string& s, apl::PluginClassFilter f)
{
    std::vector<const PluginClassInfo*> classes;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->classCount; i++) {
            if((f == PluginClassFilter::InterfaceName && s == pluginInstance->classInfos[i]->interfaceClassName)
               || (f == PluginClassFilter::ClassName && s == pluginInstance->classInfos[i]->className))
            {
                classes.push_back(pluginInstance->classInfos[i]);
            }
        }
    }
    return classes;
}
