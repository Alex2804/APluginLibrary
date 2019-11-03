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

        d_ptr->getFeatureCount = LibraryLoader::getSymbol<detail::getFeatureCountFunction>(d_ptr->libraryHandle, "getPluginFeatureCount");
        d_ptr->getFeatureInfo = LibraryLoader::getSymbol<detail::getFeatureInfoFunction>(d_ptr->libraryHandle, "getPluginFeatureInfo");
        d_ptr->getFeatureInfos = LibraryLoader::getSymbol<detail::getFeatureInfosFunction>(d_ptr->libraryHandle, "getPluginFeatureInfos");

        d_ptr->getClassCount = LibraryLoader::getSymbol<detail::getClassCountFunction>(d_ptr->libraryHandle, "getPluginClassCount");
        d_ptr->getClassInfo = LibraryLoader::getSymbol<detail::getClassInfoFunction>(d_ptr->libraryHandle, "getPluginClassInfo");
        d_ptr->getClassInfos = LibraryLoader::getSymbol<detail::getClassInfosFunction>(d_ptr->libraryHandle, "getPluginClassInfos");
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
    if(!plugin->d_ptr->getFeatureCount || !plugin->d_ptr->getFeatureInfo || !plugin->d_ptr->getFeatureInfos ||
       !plugin->d_ptr->getClassCount || !plugin->d_ptr->getClassInfo || !plugin->d_ptr->getClassInfos)
    {
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
    }
}
/**
 * @return True if the plugin is loaded or false if not.
 */
bool apl::Plugin::isLoaded() const
{
    return d_ptr != nullptr && d_ptr->libraryHandle != nullptr;
}

/**
 * @return The path to the shared library of the plugin.
 */
std::string apl::Plugin::getPath() const
{
    return d_ptr != nullptr ? d_ptr->libraryPath : "";
}
/**
 * @return The handle to the shared library of the Plugin
 */
apl::const_library_handle apl::Plugin::getHandle() const
{
    return d_ptr->libraryHandle;
}

/**
 * @return The count of PluginFeatureInfo's (features) contained in this plugin.
 */
size_t apl::Plugin::getFeatureCount() const
{
    return isLoaded() ? d_ptr->getFeatureCount() : 0;
}
/**
 * @param index The index of the PluginFeatureInfo
 * @return The PluginFeatureInfo or nullptr if @p is no valid index
 *
 * @see getFeatureCount()
 */
const apl::PluginFeatureInfo *apl::Plugin::getFeatureInfo(size_t index) const
{
    return isLoaded() ? d_ptr->getFeatureInfo(index) : nullptr;
}
/**
 * @return A PluginFeatureInfo array, with all PluginFeatureInfo's of the plugin.
 *
 * @see getFeatureCount()
 */
const apl::PluginFeatureInfo* const *apl::Plugin::getFeatureInfos() const
{
    return isLoaded() ? d_ptr->getFeatureInfos() : nullptr;
}

/**
 * @return The count of PluginClassInfo's (classes) contained in this plugin.
 */
size_t apl::Plugin::getClassCount() const
{
    return isLoaded() ? d_ptr->getClassCount() : 0;
}
/**
 * @param index The index of the PluginClassInfo
 * @return The PluginClassInfo or nullptr if @p is no valid index
 *
 * @see getClassCount()
 */
const apl::PluginClassInfo *apl::Plugin::getClassInfo(size_t index) const
{
    return isLoaded() ? d_ptr->getClassInfo(index) : nullptr;
}
/**
 * @return A PluginClassInfo array, with all PluginClassInfo's of the plugin.
 *
 * @see getClassCount()
 */
const apl::PluginClassInfo *const *apl::Plugin::getClassInfos() const
{
    return isLoaded() ? d_ptr->getClassInfos() : nullptr;
}
