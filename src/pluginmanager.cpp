#include "APluginLibrary/pluginmanager.h"
#include "private/pluginmanagerprivate.h"

#include <unordered_set>
#include <exception>

/**
 * @class apl::PluginManager
 *
 * @brief A PluginManager manages multiple Plugin objects.
 *
 * With a PluginManager you can load and unload several plugins and query and filter their features and classes.
 */


/**
 * Creates a new PluginManager without any loaded Plugins
 */
apl::PluginManager::PluginManager()
    : d_ptr(new detail::PluginManagerPrivate())
{}
/**
 * Constructs a copy of @p other.
 *
 * @see operator=(const PluginManager& other)
 */
apl::PluginManager::PluginManager(const PluginManager& other)
    : PluginManager()
{
    d_ptr->pluginInstances = other.d_ptr->pluginInstances;
}
/**
 * Move constructs @p other.
 *
 * @see operator=(PluginManager&& other)
 */
apl::PluginManager::PluginManager(PluginManager&& other) noexcept
    : d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}
/**
 * Destroys this PluginManager after unloading all loaded Plugins.
 *
 * @see unloadAll()
 */
apl::PluginManager::~PluginManager()
{
    unloadAll();
    delete d_ptr;
}

/**
 * Assigns a copy of @p other.
 *
 * @return A reference to this
 *
 * @see PluginManager(const PluginManager& other)
 */
apl::PluginManager &apl::PluginManager::operator=(const PluginManager& other)
{
    d_ptr->pluginInstances = other.d_ptr->pluginInstances;
    return *this;
}
/**
 * Move assigns @p other to this PluginManager.
 *
 * @return A reference to this
 *
 * @see PluginManager(PluginManager&& other)
 */
apl::PluginManager &apl::PluginManager::operator=(PluginManager&& other) noexcept
{
    using std::swap;
    swap(d_ptr, other.d_ptr);
    return *this;
}

/**
 * Loads a plugin into this PluginManager.
 *
 * @param path The path to the shared library containing the plugin.
 *
 * @return True if the plugin was loaded successfully and false if not.
 */
bool apl::PluginManager::load(std::string path)
{
    return d_ptr->loadPlugin(std::move(path));
}
/**
 * @return The count of loaded Plugins int this PluginManager.
 */
size_t apl::PluginManager::getLoadedPluginCount()
{
    return d_ptr->pluginInstances.size();
}
/**
 * @return The loaded Plugins in this PluginManager.
 */
std::vector<apl::Plugin*> apl::PluginManager::getLoadedPlugins()
{
    std::vector<Plugin*> plugins;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        plugins.push_back(pluginInstance->plugin);
    }
    return plugins;
}

/**
 * Unloads a specific plugin from this PluginManager.
 *
 * @param plugin The plugin to unload.
 */
void apl::PluginManager::unload(apl::Plugin* plugin)
{
    d_ptr->unloadPlugin(plugin);
}
/**
 * Unloads all plugins in this PluginManager.
 */
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
/**
 * @return The PluginFeatureInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures()
{
    std::vector<const PluginFeatureInfo*> features;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        features.insert(features.end(), pluginInstance->featureInfos, pluginInstance->featureInfos + pluginInstance->featureCount);
    }
    return features;
}
/**
 * @param string The string to filter for.
 * @param filter The filter to use.
 *
 * @return The filtered PluginFeatureInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures(const std::string& string, const PluginFeatureFilter& filter)
{
    std::vector<const PluginFeatureInfo*> features;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->featureCount; i++) {
            if(string == filterFeatureInfo(pluginInstance->featureInfos[i], filter))
                features.push_back(pluginInstance->featureInfos[i]);
        }
    }
    return features;
}
/**
 * @param filter The filter to use.
 * @return A vector with all filtered feature properties contained in the plugins loaded by this PluginManager.
 */
std::vector<std::string> apl::PluginManager::getFeatureProperties(const PluginFeatureFilter& filter)
{
    std::unordered_set<std::string> propertiesSet;
    const char* property;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->featureCount; i++) {
            property = filterFeatureInfo(pluginInstance->featureInfos[i], filter);
            if(propertiesSet.find(property) == propertiesSet.end())
                propertiesSet.emplace(property);
        }
    }
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}

inline const char* filterClassInfo(const apl::PluginClassInfo* info, const apl::PluginClassFilter& filter)
{
    if(filter == apl::PluginClassFilter::InterfaceName)
        return info->interfaceName;
    else if(filter == apl::PluginClassFilter::ClassName)
        return info->className;
    else
        throw std::runtime_error("Unimplemented apl::PluginFeatureFilter");
}
/**
 * @return The PluginClassInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses()
{
    std::vector<const PluginClassInfo*> classes;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        classes.insert(classes.end(), pluginInstance->classInfos, pluginInstance->classInfos + pluginInstance->classCount);
    }
    return classes;
}
/**
 * @param string The string to filter for.
 * @param filter The filter to use.
 *
 * @return The filtered PluginClassInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses(const std::string& string, const PluginClassFilter& filter)
{
    std::vector<const PluginClassInfo*> classes;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->classCount; i++) {
            if(string == filterClassInfo(pluginInstance->classInfos[i], filter))
                classes.push_back(pluginInstance->classInfos[i]);
        }
    }
    return classes;
}
/**
 * @param filter The filter to use.
 * @return A vector with all filtered class properties contained in the plugins loaded by this PluginManager.
 */
std::vector<std::string> apl::PluginManager::getClassProperties(const PluginClassFilter& filter)
{
    std::unordered_set<std::string> propertiesSet;
    const char* property;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->classCount; i++) {
            property = filterClassInfo(pluginInstance->classInfos[i], filter);
            if(propertiesSet.find(property) == propertiesSet.end())
                propertiesSet.emplace(property);
        }
    }
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}
