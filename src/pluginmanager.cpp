#include "APluginLibrary/pluginmanager.h"
#include "private/pluginmanagerprivate.h"

#include "APluginLibrary/pluginmanagerobserver.h"

#include <unordered_set>
#include <algorithm>
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
    other.d_ptr->localMutex.lock();
    d_ptr->plugins = other.d_ptr->plugins;
    for(auto plugin : d_ptr->plugins)
        detail::PluginManagerPrivate::loadPlugin(plugin);
    other.d_ptr->localMutex.unlock();
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
    if(this != &other) {
        d_ptr->localMutex.lock();
        unloadAll();
        d_ptr->observers.clear();
        other.d_ptr->localMutex.lock();
        d_ptr->plugins = other.d_ptr->plugins;
        for(auto plugin : d_ptr->plugins)
            detail::PluginManagerPrivate::loadPlugin(plugin);
        other.d_ptr->localMutex.unlock();
        d_ptr->localMutex.unlock();
    }
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
    d_ptr->localMutex.lock();
    other.d_ptr->localMutex.lock();
    swap(d_ptr, other.d_ptr);
    other.d_ptr->localMutex.unlock();
    d_ptr->localMutex.unlock();
    return *this;
}

/**
 * Loads a plugin into this PluginManager if not already loaded and notifies the observers about the new plugin.
 *
 * @param path The path to the shared library containing the plugin.
 *
 * @return The plugin if it was loaded successfully and nullptr if not.
 */
apl::Plugin* apl::PluginManager::load(std::string path)
{
    d_ptr->localMutex.lock();
    Plugin* plugin = detail::PluginManagerPrivate::loadPlugin(std::move(path));
    if(plugin != nullptr && std::find(d_ptr->plugins.begin(), d_ptr->plugins.end(), plugin) == d_ptr->plugins.end()) {
        d_ptr->plugins.push_back(plugin);
        for(auto observer : d_ptr->observers)
            observer->pluginLoaded(this, plugin);
    } else if(plugin != nullptr) {
        detail::PluginManagerPrivate::unloadPlugin(plugin);
    }
    d_ptr->localMutex.unlock();
    return plugin;
}
/**
 * Loads all plugins in the directory at path into this PluginManager.
 *
 * @param path The path to the directory.
 * @param recursive If the directory should be searched recursive.
 *
 * @return The loaded plugins.
 */
std::vector<apl::Plugin*> apl::PluginManager::loadDirectory(const std::string& path, bool recursive)
{
    tinydir_dir dir;
    tinydir_file file;
    std::vector<apl::Plugin*> plugins, tmpPlugins;
    apl::Plugin* tmpPlugin;
    std::string filePath;

    tinydir_open(&dir, path.c_str());
    d_ptr->localMutex.lock();
    while (dir.has_next) {
        tinydir_readfile(&dir, &file);
        filePath = file.path;
        if(strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0) {
            if (file.is_dir && recursive) {
                tmpPlugins = loadDirectory(filePath, recursive);
                plugins.insert(plugins.end(), tmpPlugins.begin(), tmpPlugins.end());
            } else if (!file.is_dir && strcmp(file.extension, apl::LibraryLoader::libExtension()) == 0
                       && (tmpPlugin = load(filePath.erase(filePath.size() - 1 - strlen(file.extension)))) != nullptr) {
                plugins.emplace_back(tmpPlugin);
            }
        }
        tinydir_next(&dir);
    }
    tinydir_close(&dir);
    d_ptr->localMutex.unlock();
    return plugins;
}

/**
 * @return The count of loaded Plugins int this PluginManager.
 */
size_t apl::PluginManager::getLoadedPluginCount() const
{
    std::lock_guard<std::recursive_mutex> lockGuard(d_ptr->localMutex);
    return d_ptr->plugins.size();
}
/**
 * @return The loaded Plugin with the given @p path as constant pointers in this PluginManager or nullptr if no plugin
 * with the given path exists in this PluginManager.
 */
const apl::Plugin* apl::PluginManager::getLoadedPlugin(const std::string& path) const
{
    std::lock_guard<std::recursive_mutex> lockGuard(d_ptr->localMutex);
    for(auto plugin : d_ptr->plugins) {
        if(plugin->getPath() == path)
            return plugin;
    }
    return nullptr;
}
/**
 * @return The loaded Plugin iwth the given @p path in this PluginManager or nullptr if no plugin with the given path
 * exists in this PluginManager.
 */
apl::Plugin* apl::PluginManager::getLoadedPlugin(const std::string& path)
{
    std::lock_guard<std::recursive_mutex> lockGuard(d_ptr->localMutex);
    for(auto plugin : d_ptr->plugins) {
        if(plugin->getPath() == path)
            return plugin;
    }
    return nullptr;
}
/**
 * @return The loaded Plugins as constant pointers in this PluginManager.
 */
std::vector<const apl::Plugin*> apl::PluginManager::getLoadedPlugins() const
{
    std::vector<const Plugin*> plugins;
    d_ptr->localMutex.lock();
    plugins.reserve(d_ptr->plugins.size());
    for(auto plugin : d_ptr->plugins)
        plugins.push_back(plugin);
    d_ptr->localMutex.unlock();
    return plugins;
}
/**
 * @return The loaded Plugins in this PluginManager.
 */
std::vector<apl::Plugin*> apl::PluginManager::getLoadedPlugins()
{
    std::lock_guard<std::recursive_mutex> lockGuard(d_ptr->localMutex);
    return d_ptr->plugins;
}

/**
 * Unloads a specific plugin from this PluginManager and notifies the observer about the removed plugin.
 *
 * @param plugin The plugin to unload.
 */
void apl::PluginManager::unload(apl::Plugin* plugin)
{
    d_ptr->localMutex.lock();
    auto iterator = std::remove(d_ptr->plugins.begin(), d_ptr->plugins.end(), plugin);
    if(iterator != d_ptr->plugins.end()) {
        d_ptr->plugins.erase(iterator);
        for(auto observer : d_ptr->observers)
            observer->pluginUnloaded(this, plugin);
        detail::PluginManagerPrivate::unloadPlugin(plugin);
    }
    d_ptr->localMutex.unlock();
}
/**
 * Unloads all plugins in this PluginManager.
 */
void apl::PluginManager::unloadAll()
{
    d_ptr->localMutex.lock();
    while(!d_ptr->plugins.empty()) {
        Plugin* plugin = d_ptr->plugins.back();
        d_ptr->plugins.pop_back();
        for(auto observer : d_ptr->observers)
            observer->pluginUnloaded(this, plugin);
        detail::PluginManagerPrivate::unloadPlugin(plugin);
    }
    d_ptr->localMutex.unlock();
}

/**
 * @return The PluginInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginInfo*> apl::PluginManager::getPluginInfos() const
{
    std::vector<const apl::PluginInfo*> infos;
    d_ptr->localMutex.lock();
    infos.reserve(d_ptr->plugins.size());
    for(const auto plugin : d_ptr->plugins)
        infos.emplace_back(plugin->getPluginInfo());
    d_ptr->localMutex.unlock();
    return infos;
}
/**
 * @param string The string to filter for.
 * @param filter The filter to use.
 *
 * @return The filtered PluginInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginInfo*> apl::PluginManager::getPluginInfos(const std::string& string, PluginInfoFilter filter) const
{
    std::vector<const apl::PluginInfo*> infos;
    const PluginInfo* info;
    d_ptr->localMutex.lock();
    infos.reserve(d_ptr->plugins.size());
    for(const auto plugin : d_ptr->plugins) {
        info = plugin->getPluginInfo();
        if(string == detail::filterPluginInfo(info, filter))
            infos.emplace_back(info);
    }
    d_ptr->localMutex.unlock();
    return infos;
}
/**
 * @param filter The filter to use.
 * @return A vector with all filtered plugin info properties contained in the plugins loaded by this PluginManager.
 */
std::vector<std::string> apl::PluginManager::getPluginProperties(PluginInfoFilter filter) const
{
    std::unordered_set<std::string> propertiesSet;
    d_ptr->localMutex.lock();
    propertiesSet.reserve(d_ptr->plugins.size());
    for(const auto plugin : d_ptr->plugins)
        propertiesSet.emplace(detail::filterPluginInfo(plugin->getPluginInfo(), filter));
    d_ptr->localMutex.unlock();
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}

/**
 * @return The PluginFeatureInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures() const
{
    std::vector<const PluginFeatureInfo*> features;
    const PluginFeatureInfo* const* featureInfos;
    d_ptr->localMutex.lock();
    for(const auto plugin : d_ptr->plugins) {
        featureInfos = plugin->getFeatureInfos();
        if(featureInfos != nullptr)
            features.insert(features.end(), featureInfos, featureInfos + plugin->getFeatureCount());
    }
    d_ptr->localMutex.unlock();
    return features;
}
/**
 * @param string The string to filter for.
 * @param filter The filter to use.
 *
 * @return The filtered PluginFeatureInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginFeatureInfo*> apl::PluginManager::getFeatures(const std::string& string, PluginFeatureFilter filter) const
{
    std::vector<const PluginFeatureInfo*> features;
    const PluginFeatureInfo* const* featureInfos;
    d_ptr->localMutex.lock();
    for(const auto plugin : d_ptr->plugins) {
        featureInfos = plugin->getFeatureInfos();
        for(size_t i = 0; i < plugin->getFeatureCount(); i++) {
            if(string == detail::filterFeatureInfo(featureInfos[i], filter))
                features.emplace_back(featureInfos[i]);
        }
    }
    d_ptr->localMutex.unlock();
    return features;
}
/**
 * @param filter The filter to use.
 * @return A vector with all filtered feature properties contained in the plugins loaded by this PluginManager.
 */
std::vector<std::string> apl::PluginManager::getFeatureProperties(PluginFeatureFilter filter) const
{
    std::unordered_set<std::string> propertiesSet;
    const PluginFeatureInfo* const* featureInfos;
    const char* property;
    d_ptr->localMutex.lock();
    for(const auto plugin : d_ptr->plugins) {
        featureInfos = plugin->getFeatureInfos();
        for(size_t i = 0; i < plugin->getFeatureCount(); i++) {
            property = detail::filterFeatureInfo(featureInfos[i], filter);
            propertiesSet.emplace(property);
        }
    }
    d_ptr->localMutex.unlock();
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}

/**
 * @return The PluginClassInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses() const
{
    std::vector<const PluginClassInfo*> classes;
    const PluginClassInfo* const* classInfos;
    d_ptr->localMutex.lock();
    for(const auto plugin : d_ptr->plugins) {
        classInfos = plugin->getClassInfos();
        if(classInfos != nullptr)
            classes.insert(classes.end(), classInfos, classInfos + plugin->getClassCount());
    }
    d_ptr->localMutex.unlock();
    return classes;
}
/**
 * @param string The string to filter for.
 * @param filter The filter to use.
 *
 * @return The filtered PluginClassInfo's of all loaded plugins in this PluginManager.
 */
std::vector<const apl::PluginClassInfo*> apl::PluginManager::getClasses(const std::string& string, PluginClassFilter filter) const
{
    std::vector<const PluginClassInfo*> classes;
    const PluginClassInfo* const* classInfos;
    d_ptr->localMutex.lock();
    for(const auto plugin : d_ptr->plugins) {
        classInfos = plugin->getClassInfos();
        for(size_t i = 0; i < plugin->getClassCount(); i++) {
            if(string == detail::filterClassInfo(classInfos[i], filter))
                classes.emplace_back(classInfos[i]);
        }
    }
    d_ptr->localMutex.unlock();
    return classes;
}
/**
 * @param filter The filter to use.
 * @return A vector with all filtered class properties contained in the plugins loaded by this PluginManager.
 */
std::vector<std::string> apl::PluginManager::getClassProperties(PluginClassFilter filter) const
{
    std::unordered_set<std::string> propertiesSet;
    const PluginClassInfo* const* classInfos;
    const char* property;
    d_ptr->localMutex.lock();
    for(const auto plugin : d_ptr->plugins) {
        classInfos = plugin->getClassInfos();
        for(size_t i = 0; i < plugin->getClassCount(); i++) {
            property = detail::filterClassInfo(classInfos[i], filter);
            propertiesSet.emplace(property);
        }
    }
    d_ptr->localMutex.unlock();
    return std::vector<std::string>(propertiesSet.begin(), propertiesSet.end());
}

/**
 * Adds an observer to this PluginManager instance which gets notified about changes.
 * @param observer The observer to be added (only added if not already an observer of this PluginManager instance).
 */
void apl::PluginManager::addObserver(PluginManagerObserver* observer)
{
    d_ptr->localMutex.lock();
    if(observer != nullptr && std::find(d_ptr->observers.begin(), d_ptr->observers.end(), observer) == d_ptr->observers.end())
        d_ptr->observers.push_back(observer);
    d_ptr->localMutex.unlock();
}
/**
 * Removes an observer from this PluginManager instance.
 * @param observer The observer to be removed.
 */
void apl::PluginManager::removeObserver(PluginManagerObserver* observer)
{
    d_ptr->localMutex.lock();
    auto iterator = std::find(d_ptr->observers.begin(), d_ptr->observers.end(), observer);
    if(iterator != d_ptr->observers.end())
        d_ptr->observers.erase(iterator);
    d_ptr->localMutex.unlock();
}
