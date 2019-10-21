#include "APluginLibrary/plugin.h"
#include "private/pluginprivate.h"

#include <APluginLibrary/libraryloader.h>

#include <utility>


apl::Plugin::Plugin(std::string path)
    : d_ptr(new detail::PluginPrivate())
{
    d_ptr->path = std::move(path);
    d_ptr->handle = LibraryLoader::load(d_ptr->path);
    if(d_ptr->handle == nullptr)
        return;
    d_ptr->getFeatureCount = LibraryLoader::getSymbol<detail::getFeatureCountFunction>(d_ptr->handle, "getPluginFeatureCount");
    d_ptr->getFeatureInfo = LibraryLoader::getSymbol<detail::getFeatureInfoFunction>(d_ptr->handle, "getPluginFeatureInfo");
    d_ptr->getFeatureInfos = LibraryLoader::getSymbol<detail::getFeatureInfosFunction>(d_ptr->handle, "getPluginFeatureInfos");

    d_ptr->getClassCount = LibraryLoader::getSymbol<detail::getClassCountFunction>(d_ptr->handle, "getPluginClassCount");
    d_ptr->getClassInfo = LibraryLoader::getSymbol<detail::getClassInfoFunction>(d_ptr->handle, "getPluginClassInfo");
    d_ptr->getClassInfos = LibraryLoader::getSymbol<detail::getClassInfosFunction>(d_ptr->handle, "getPluginClassInfos");
}
apl::Plugin::~Plugin()
{
    unload();
    delete d_ptr;
}

apl::Plugin *apl::Plugin::load(std::string path)
{
    auto* plugin = new Plugin(std::move(path));
    if(!plugin->d_ptr->handle ||
       !plugin->d_ptr->getFeatureCount || !plugin->d_ptr->getFeatureInfo || !plugin->d_ptr->getFeatureInfos ||
       !plugin->d_ptr->getClassCount || !plugin->d_ptr->getClassInfo || !plugin->d_ptr->getClassInfos)
    {
        delete plugin;
        plugin = nullptr;
    }
    return plugin;
}
void apl::Plugin::unload()
{
    if(isLoaded()) {
        LibraryLoader::unload(d_ptr->handle);
        d_ptr->handle = nullptr;
    }
}
bool apl::Plugin::isLoaded() const
{
    return d_ptr != nullptr && d_ptr->handle != nullptr;
}

std::string apl::Plugin::getPath() const
{
    return d_ptr != nullptr ? d_ptr->path : "";
}

size_t apl::Plugin::getFeatureCount() const
{
    return isLoaded() ? d_ptr->getFeatureCount() : 0;
}
const apl::PluginFeatureInfo *apl::Plugin::getFeatureInfo(size_t i) const
{
    return isLoaded() ? d_ptr->getFeatureInfo(i) : nullptr;
}
const apl::PluginFeatureInfo *const *apl::Plugin::getFeatureInfos() const
{
    return isLoaded() ? d_ptr->getFeatureInfos() : nullptr;
}

size_t apl::Plugin::getClassCount() const
{
    return isLoaded() ? d_ptr->getClassCount() : 0;
}
const apl::PluginClassInfo *apl::Plugin::getClassInfo(size_t i) const
{
    return isLoaded() ? d_ptr->getClassInfo(i) : nullptr;
}
const apl::PluginClassInfo *const *apl::Plugin::getClassInfos() const
{
    return isLoaded() ? d_ptr->getClassInfos() : nullptr;
}