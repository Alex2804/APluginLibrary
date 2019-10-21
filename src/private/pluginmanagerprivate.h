#ifndef APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
#define APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H

#include <vector>
#include <unordered_map>
#include <memory>

#include "APluginLibrary/plugin.h"
#include "APluginLibrary/plugininfos.h"

namespace apl
{
    class PluginManager;

    namespace detail
    {
        struct PluginInstance
        {
            explicit PluginInstance(std::unique_ptr<Plugin> plugin);
            ~PluginInstance();

            std::unique_ptr<Plugin> plugin;
            size_t featureCount;
            size_t classCount;
            const PluginFeatureInfo* const* featureInfos;
            const PluginClassInfo* const* classInfos;
        };

        struct PluginManagerPrivate
        {
            std::vector<std::shared_ptr<PluginInstance>> pluginInstances;
            static std::unordered_map<Plugin*, PluginInstance*> loadedPlugins;
        };
    }
}

#endif //APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
