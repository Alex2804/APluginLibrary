#include "APluginLibrary/pluginmanager.h"
#include "private/pluginmanagerprivate.h"

#include <unordered_set>
#include <exception>

apl::PluginManager::PluginManager()
    : d_ptr(new detail::PluginManagerPrivate())
{}
apl::PluginManager::PluginManager(const apl::PluginManager &other)
    : PluginManager()
{
    d_ptr->pluginInstances = other.d_ptr->pluginInstances;
}
apl::PluginManager::PluginManager(apl::PluginManager &&other) noexcept
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
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
    return d_ptr->loadPlugin(std::move(path));
}

size_t apl::PluginManager::getLoadedPluginCount()
{
    return d_ptr->pluginInstances.size();
}
std::vector<apl::Plugin*> apl::PluginManager::getLoadedPlugins()
{
    std::vector<Plugin*> plugins;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        plugins.push_back(pluginInstance->plugin);
    }
    return plugins;
}

void apl::PluginManager::unload(apl::Plugin* plugin)
{
    d_ptr->unloadPlugin(plugin);
}
void apl::PluginManager::unloadAll()
{
    d_ptr->pluginInstances.clear();
}

inline const char* filterFeatureInfo(const apl::PluginFeatureInfo* info, const apl::PluginFeatureFilter& filter)
{
    if(filter == apl::PluginFeatureFilter::FeatureGroup)
        return info->featureGroup;
    else if(filter == apl::PluginFeatureFilter::FeatureName)
        return info->featureName;
    else if(filter == apl::PluginFeatureFilter::ArgumentList)
        return info->argumentList;
    else if(filter == apl::PluginFeatureFilter::ReturnType)
        return info->returnType;
    else
        throw std::runtime_error("Unimplemented apl::PluginFeatureFilter");
}
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures()
{
    std::vector<const PluginFeatureInfo*> features;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        features.insert(features.end(), pluginInstance->featureInfos, pluginInstance->featureInfos + pluginInstance->featureCount);
    }
    return features;
}
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures(const std::string& s, const apl::PluginFeatureFilter& f)
{
    std::vector<const PluginFeatureInfo*> features;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->featureCount; i++) {
            if(s == filterFeatureInfo(pluginInstance->featureInfos[i], f))
                features.push_back(pluginInstance->featureInfos[i]);
        }
    }
    return features;
}
std::vector<std::string> apl::PluginManager::getFeatureProperties(const apl::PluginFeatureFilter& f)
{
    std::unordered_set<std::string> propertiesSet;
    const char* property;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->featureCount; i++) {
            property = filterFeatureInfo(pluginInstance->featureInfos[i], f);
            if(propertiesSet.find(property) == propertiesSet.end())
                propertiesSet.emplace(property);
        }
    }
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}

inline const char* filterClassInfo(const apl::PluginClassInfo* info, const apl::PluginClassFilter & filter)
{
    if(filter == apl::PluginClassFilter::InterfaceName)
        return info->interfaceName;
    else if(filter == apl::PluginClassFilter::ClassName)
        return info->className;
    else
        throw std::runtime_error("Unimplemented apl::PluginFeatureFilter");
}
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses()
{
    std::vector<const PluginClassInfo*> classes;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        classes.insert(classes.end(), pluginInstance->classInfos, pluginInstance->classInfos + pluginInstance->classCount);
    }
    return classes;
}
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses(const std::string& s, const apl::PluginClassFilter& f)
{
    std::vector<const PluginClassInfo*> classes;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->classCount; i++) {
            if(s == filterClassInfo(pluginInstance->classInfos[i], f))
                classes.push_back(pluginInstance->classInfos[i]);
        }
    }
    return classes;
}
std::vector<std::string> apl::PluginManager::getClassProperties(const apl::PluginClassFilter& f)
{
    std::unordered_set<std::string> propertiesSet;
    const char* property;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->classCount; i++) {
            property = filterClassInfo(pluginInstance->classInfos[i], f);
            if(propertiesSet.find(property) == propertiesSet.end())
                propertiesSet.emplace(property);
        }
    }
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}
