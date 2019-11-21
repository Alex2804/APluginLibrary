#ifndef APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
#define APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

#include "APluginLibrary/plugin.h"
#include "APluginLibrary/libraryloader.h"
#include "APluginSDK/plugininfos.h"

#ifdef APLUGINLIBRARY_TEST
# undef APLUGINLIBRARY_NO_EXPORT
# define APLUGINLIBRARY_NO_EXPORT APLUGINLIBRARY_EXPORT
#endif

namespace apl
{
    namespace detail
    {
        class APLUGINLIBRARY_NO_EXPORT PluginInstance
        {
        public:
            explicit PluginInstance(Plugin* plugin);
            ~PluginInstance();

            Plugin* plugin;
        };

        class APLUGINLIBRARY_NO_EXPORT PluginManagerPrivate
        {
        public:
            std::vector<std::shared_ptr<PluginInstance>> pluginInstances; // loaded plugins of this PluginManager instance
            static std::unordered_map<const_library_handle, std::weak_ptr<PluginInstance>> loadedPlugins; // loaded plugins of ALL PluginManager instances
            static std::mutex mutex;

            apl::Plugin* loadPlugin(std::string path); // loads a plugin into this PluginManager (tries to prevent duplicates)
            void unloadPlugin(Plugin* plugin); // unload a plugin from this PluginManager instance
        };
    }

    enum class PluginInfoFilter;
    enum class PluginFeatureFilter;
    enum class PluginClassFilter;

    namespace detail
    {
        std::string filterPluginInfo(const PluginInfo* info, PluginInfoFilter filter);
        const char* filterFeatureInfo(const PluginFeatureInfo* info, PluginFeatureFilter filter);
        const char* filterClassInfo(const PluginClassInfo* info, PluginClassFilter filter);
    }
}

#endif //APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
