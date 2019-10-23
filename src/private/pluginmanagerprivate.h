#ifndef APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
#define APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <vector>
#include <unordered_map>
#include <memory>

#include "APluginLibrary/pluginmanager.h"
#include "APluginLibrary/plugin.h"
#include "APluginLibrary/plugininfos.h"

#ifdef APLUGINLIBRARY_TEST
# undef APLUGINLIBRARY_NO_EXPORT
# define APLUGINLIBRARY_NO_EXPORT APLUGINLIBRARY_EXPORT
#endif

namespace apl
{
    namespace detail
    {
        struct APLUGINLIBRARY_NO_EXPORT PluginInstance
        {
            explicit PluginInstance(std::unique_ptr<Plugin> plugin);
            ~PluginInstance();

            std::unique_ptr<Plugin> plugin;
            size_t featureCount;
            size_t classCount;
            const PluginFeatureInfo* const* featureInfos;
            const PluginClassInfo* const* classInfos;
        };

        struct APLUGINLIBRARY_NO_EXPORT PluginManagerPrivate
        {
            std::vector<std::shared_ptr<PluginInstance>> pluginInstances;
            static std::unordered_map<Plugin*, PluginInstance*> loadedPlugins;
        };
    }
}

#endif //APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
