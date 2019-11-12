#include "APluginLibrary/pluginmanager.h"
#include "private/pluginmanagerprivate.h"

#include <unordered_set>
#include <exception>
#include <cstring>

#include "tinydir/tinydir.h"

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
 * Loads all plugins in the directory at path into this PluginManager.
 *
 * @param path The path to the directory.
 * @param recursive If the directory should be searched recursive.
 *
 * @return The count of loaded plugins.
 */
size_t apl::PluginManager::loadDirectory(const std::string& path, bool recursive)
{
    tinydir_dir dir;
    tinydir_file file;
    size_t count = 0;
    std::string filePath;

    tinydir_open(&dir, path.c_str());
    while (dir.has_next)
    {
        tinydir_readfile(&dir, &file);

        filePath = file.path;
        if(strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0) {
            if (file.is_dir && recursive) {
                count += loadDirectory(filePath, recursive);
            } else if (!file.is_dir && strcmp(file.extension, apl::LibraryLoader::libExtension()) == 0
                       && load(filePath.erase(filePath.size() - 1 - strlen(file.extension)))) {
                ++count;
            }
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
    return count;
}

/**
 * @return The count of loaded Plugins int this PluginManager.
 */
size_t apl::PluginManager::getLoadedPluginCount() const
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

/**
 * @return The PluginFeatureInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures() const
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
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures(const std::string& string, const PluginFeatureFilter& filter) const
{
    std::vector<const PluginFeatureInfo*> features;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->featureCount; i++) {
            if(string == detail::filterFeatureInfo(pluginInstance->featureInfos[i], filter))
                features.push_back(pluginInstance->featureInfos[i]);
        }
    }
    return features;
}
/**
 * @param filter The filter to use.
 * @return A vector with all filtered feature properties contained in the plugins loaded by this PluginManager.
 */
std::vector<std::string> apl::PluginManager::getFeatureProperties(const PluginFeatureFilter& filter) const
{
    std::unordered_set<std::string> propertiesSet;
    const char* property;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->featureCount; i++) {
            property = detail::filterFeatureInfo(pluginInstance->featureInfos[i], filter);
            if(propertiesSet.find(property) == propertiesSet.end())
                propertiesSet.emplace(property);
        }
    }
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}

/**
 * @return The PluginClassInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses() const
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
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses(const std::string& string, const PluginClassFilter& filter) const
{
    std::vector<const PluginClassInfo*> classes;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->classCount; i++) {
            if(string == detail::filterClassInfo(pluginInstance->classInfos[i], filter))
                classes.push_back(pluginInstance->classInfos[i]);
        }
    }
    return classes;
}
/**
 * @param filter The filter to use.
 * @return A vector with all filtered class properties contained in the plugins loaded by this PluginManager.
 */
std::vector<std::string> apl::PluginManager::getClassProperties(const PluginClassFilter& filter) const
{
    std::unordered_set<std::string> propertiesSet;
    const char* property;
    for(const auto& pluginInstance : d_ptr->pluginInstances) {
        for(size_t i = 0; i < pluginInstance->classCount; i++) {
            property = detail::filterClassInfo(pluginInstance->classInfos[i], filter);
            if(propertiesSet.find(property) == propertiesSet.end())
                propertiesSet.emplace(property);
        }
    }
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}
