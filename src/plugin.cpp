#include "APluginLibrary/plugin.h"
#include "private/pluginprivate.h"

/**
 * @class apl::Plugin
 *
 * @brief A Plugin object manages a shared library which contains a plugin.
 *
 * Using a plugin object simplifies the loading and unloading of plugins (shared libraries with the plugin api) using
 * the @ref load and @ref unload method.\n
 * From a loaded Plugin you can query the features and classes of that plugin.
 */

apl::Plugin::Plugin(std::string path, library_handle handle)
    : d_ptr(new detail::PluginPrivate())
{
    if(handle) {
        d_ptr->libraryPath = std::move(path);
        d_ptr->libraryHandle = handle;

        d_ptr->getPluginInfo = LibraryLoader::getSymbol<detail::getPluginInfoFunction>(d_ptr->libraryHandle, "getPluginInfo");
        if(d_ptr->getPluginInfo != nullptr)
            d_ptr->pluginInfo = d_ptr->getPluginInfo();
    }
}
/**
 * Destroys the Plugin object and unloads the plugin.
 *
 * @see unload()
 */
apl::Plugin::~Plugin()
{
    unload();
    delete d_ptr;
}

/**
 * Loads a plugin contained in the shared library at path and returns the pointer to the Plugin instance or nullptr if
 * loading of the plugin failed or the shared library doesn't contain a valid plugin api.
 *
 * @param path The path to the shared library.
 * @return The pointer to the created Plugin or nullptr if loading failed.
 */
apl::Plugin* apl::Plugin::load(std::string path)
{
    library_handle handle = LibraryLoader::load(path);
    if (!handle)
        return nullptr;
    auto plugin = new Plugin(std::move(path), handle);
    if(plugin->d_ptr->pluginInfo == nullptr) {
        delete plugin;
        plugin = nullptr;
    }
    return plugin;
}
/**
 * Unloads the plugin and releases the shared library.
 */
void apl::Plugin::unload()
{
    if(isLoaded()) {
        LibraryLoader::unload(d_ptr->libraryHandle);
        d_ptr->libraryHandle = nullptr;
        d_ptr->getPluginInfo = nullptr;
        d_ptr->pluginInfo = nullptr;
    }
}
/**
 * @return True if the plugin is loaded or false if not.
 */
bool apl::Plugin::isLoaded() const
{
    return d_ptr != nullptr && d_ptr->libraryHandle != nullptr && d_ptr->pluginInfo != nullptr;
}

/**
 * @return The path to the shared library of the plugin.
 */
std::string apl::Plugin::getPath() const
{
    return d_ptr == nullptr ? "" : d_ptr->libraryPath;
}
/**
 * @return The handle to the shared library of the Plugin
 */
apl::const_library_handle apl::Plugin::getHandle() const
{
    return isLoaded() ? d_ptr->libraryHandle : nullptr;
}

/**
 * @return The PluginInfo or nullptr if the plugin is not loaded.
 */
const apl::PluginInfo* apl::Plugin::getPluginInfo()
{
    return isLoaded() ? d_ptr->pluginInfo : nullptr;
}

/**
 * Calls the afl::detail::allocatePluginMemory function from the plugin api and returns the allocated memory or nullptr
 * on failure.
 * @param bytes The size of the memory to allocate in bytes.
 * @return A pointer to the allocated memory.
 */
void* apl::Plugin::allocateMemory(size_t bytes) const
{
    if(!isLoaded())
        return nullptr;
    return d_ptr->pluginInfo->allocateMemory(bytes);
}
/**
 * Calls the afl::detail::freePluginMemory function from the plugin api and returns if the function was available.
 * @param ptr The pointer to free.
 * @return If it was possible to call the free function of the Plugin.
 */
bool apl::Plugin::freeMemory(void *ptr) const
{
    if(isLoaded()) {
        d_ptr->pluginInfo->freeMemory(ptr);
        return true;
    }
    return false;
}

/**
 * @return The count of PluginFeatureInfo's (features) contained in this plugin.
 */
size_t apl::Plugin::getFeatureCount() const
{
    return isLoaded() ? d_ptr->pluginInfo->getPluginFeatureCount() : 0;
}
/**
 * @param index The index of the PluginFeatureInfo
 * @return The PluginFeatureInfo or nullptr if @p is no valid index
 *
 * @see getFeatureCount()
 */
const apl::PluginFeatureInfo *apl::Plugin::getFeatureInfo(size_t index) const
{
    return isLoaded() ? d_ptr->pluginInfo->getPluginFeatureInfo(index) : nullptr;
}
/**
 * @return A PluginFeatureInfo array, with all PluginFeatureInfo's of the plugin.
 *
 * @see getFeatureCount()
 */
const apl::PluginFeatureInfo* const* apl::Plugin::getFeatureInfos() const
{
    return isLoaded() ? d_ptr->pluginInfo->getPluginFeatureInfos() : nullptr;
}

/**
 * @return The count of PluginClassInfo's (classes) contained in this plugin.
 */
size_t apl::Plugin::getClassCount() const
{
    return isLoaded() ? d_ptr->pluginInfo->getPluginClassCount() : 0;
}
/**
 * @param index The index of the PluginClassInfo
 * @return The PluginClassInfo or nullptr if @p is no valid index
 *
 * @see getClassCount()
 */
const apl::PluginClassInfo *apl::Plugin::getClassInfo(size_t index) const
{
    return isLoaded() ? d_ptr->pluginInfo->getPluginClassInfo(index) : nullptr;
}
/**
 * @return A PluginClassInfo array, with all PluginClassInfo's of the plugin.
 *
 * @see getClassCount()
 */
const apl::PluginClassInfo *const *apl::Plugin::getClassInfos() const
{
    return isLoaded() ? d_ptr->pluginInfo->getPluginClassInfos() : nullptr;
}
