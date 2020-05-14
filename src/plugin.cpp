#include "APluginLibrary/plugin.h"
#include "private/pluginprivate.h"

#include "APluginSDK/pluginapi.h"
#include "APluginSDK/private/privateplugininfos.h"

namespace
{
    bool isValid(const apl::PluginInfo *info)
    {
        return info != nullptr && info->privateInfo != nullptr && info->privateInfo->constructPluginInternals != nullptr
            && info->privateInfo->destructPluginInternals != nullptr;
    }
    bool requiresInitAPluginFunction(const apl::PluginInfo *info)
    {
        return info == nullptr;
    }
    bool requiresFiniAPluginFunction(const apl::PluginInfo *info)
    {
        return info == nullptr || info->pluginLanguage == APLUGINLIBRARY_NAMESPACE APluginLanguage::C;
    }
}

PRIVATE_APLUGINSDK_OPEN_PRIVATE_NAMESPACE
    extern void* private_APluginSDK_initAPluginFunctionPtr;
    extern void* private_APluginSDK_finiAPluginFunctionPtr;
PRIVATE_APLUGINSDK_CLOSE_PRIVATE_NAMESPACE

/**
 * @class apl::Plugin
 *
 * @brief A Plugin object manages a shared library which contains a plugin.
 *
 * Using a plugin object simplifies the loading and unloading of plugins (shared libraries with the plugin api) using
 * the @ref load and @ref unload method.\n
 * Also the features and classes of loaded Plugin's can queried.
 */

apl::Plugin::Plugin(std::string path, library_handle handle)
    : d_ptr(new detail::PluginPrivate())
{
    if(handle == nullptr && !path.empty())
        return;
    d_ptr->libraryPath = std::move(path);
    d_ptr->libraryHandle = handle;

    if(handle != nullptr) {
        auto getAPluginInfo = LibraryLoader::getSymbol<const PluginInfo*(*)()>(d_ptr->libraryHandle, "APluginSDK_getPluginInfo");
        if(getAPluginInfo != nullptr && (d_ptr->pluginInfo = getAPluginInfo()) != nullptr) {
            d_ptr->initPlugin = LibraryLoader::getSymbol<void(*)()>(d_ptr->libraryHandle, "APluginSDK_initPlugin");
            d_ptr->finiPlugin = LibraryLoader::getSymbol<void(*)()>(d_ptr->libraryHandle, "APluginSDK_finiPlugin");
        }
    } else {
        d_ptr->pluginInfo = PRIVATE_APLUGINSDK_API_NAMESPACE APluginSDK_getPluginInfo();
        if(PRIVATE_APLUGINSDK_PRIVATE_NAMESPACE private_APluginSDK_initAPluginFunctionPtr != nullptr)
            d_ptr->initPlugin = reinterpret_cast<void(*)()>(PRIVATE_APLUGINSDK_PRIVATE_NAMESPACE private_APluginSDK_initAPluginFunctionPtr);
        if(PRIVATE_APLUGINSDK_PRIVATE_NAMESPACE private_APluginSDK_finiAPluginFunctionPtr != nullptr)
            d_ptr->finiPlugin = reinterpret_cast<void(*)()>(PRIVATE_APLUGINSDK_PRIVATE_NAMESPACE private_APluginSDK_finiAPluginFunctionPtr);
    }
    if(!isValid(d_ptr->pluginInfo) || (requiresInitAPluginFunction(d_ptr->pluginInfo) && d_ptr->initPlugin == nullptr)
       || (requiresFiniAPluginFunction(d_ptr->pluginInfo) && d_ptr->finiPlugin == nullptr))
    {
        d_ptr->pluginInfo = nullptr;
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
    library_handle handle = nullptr;
    if(!path.empty()) {
        handle = LibraryLoader::load(path);
        if (handle == nullptr)
            return nullptr;
    }
    auto plugin = new Plugin(std::move(path), handle);
    if(plugin->d_ptr->pluginInfo == nullptr) {
        delete plugin;
        return nullptr;
    }
    plugin->d_ptr->pluginInfo->privateInfo->constructPluginInternals(plugin->d_ptr->initPlugin);
    return plugin;
}
/**
 * Unloads the plugin and releases the shared library.
 */
void apl::Plugin::unload()
{
    if(d_ptr->pluginInfo != nullptr)
        d_ptr->pluginInfo->privateInfo->destructPluginInternals(d_ptr->finiPlugin);
    LibraryLoader::unload(d_ptr->libraryHandle);
    d_ptr->reset();
}
/**
 * @return True if the plugin is loaded or false if not.
 */
bool apl::Plugin::isLoaded() const
{
    return d_ptr != nullptr && d_ptr->pluginInfo != nullptr;
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
const apl::PluginInfo* apl::Plugin::getPluginInfo() const
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
    return isLoaded() ? d_ptr->pluginInfo->getFeatureCount() : 0;
}
/**
 * @param index The index of the PluginFeatureInfo
 * @return The PluginFeatureInfo or nullptr if @p is no valid index.
 *
 * @see getFeatureCount()
 */
const apl::PluginFeatureInfo *apl::Plugin::getFeatureInfo(size_t index) const
{
    size_t featureCount = getFeatureCount();
    if(featureCount == 0 || index > featureCount - 1)
        return nullptr;
    return d_ptr->pluginInfo->getFeatureInfo(index);
}
/**
 * @return A PluginFeatureInfo array, with all PluginFeatureInfo's of the plugin.
 *
 * @see getFeatureCount()
 */
const apl::PluginFeatureInfo* const* apl::Plugin::getFeatureInfos() const
{
    return isLoaded() ? d_ptr->pluginInfo->getFeatureInfos() : nullptr;
}

/**
 * @return The count of PluginClassInfo's (classes) contained in this plugin.
 */
size_t apl::Plugin::getClassCount() const
{
    return isLoaded() ? d_ptr->pluginInfo->getClassCount() : 0;
}
/**
 * @param index The index of the PluginClassInfo
 * @return The PluginClassInfo or nullptr if @p is no valid index.
 *
 * @see getClassCount()
 */
const apl::PluginClassInfo *apl::Plugin::getClassInfo(size_t index) const
{
    size_t classCount = getClassCount();
    if(classCount == 0 || index > classCount - 1)
        return nullptr;
    return d_ptr->pluginInfo->getClassInfo(index);
}
/**
 * @return A PluginClassInfo array, with all PluginClassInfo's of the plugin.
 *
 * @see getClassCount()
 */
const apl::PluginClassInfo *const *apl::Plugin::getClassInfos() const
{
    return isLoaded() ? d_ptr->pluginInfo->getClassInfos() : nullptr;
}
