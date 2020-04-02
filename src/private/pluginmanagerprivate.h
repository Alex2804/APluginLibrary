#ifndef APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
#define APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

#include "APluginLibrary/pluginmanager.h"
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
        class APLUGINLIBRARY_NO_EXPORT PluginManagerPrivate
        {
        public:
            std::vector<Plugin*> plugins;
            std::vector<PluginManagerObserver*> observers;
            std::recursive_mutex localMutex;

            static std::unordered_map<std::string, std::pair<size_t, Plugin*>> allPlugins;
            static std::mutex staticMutex;
            static Plugin* loadPlugin(std::string absolutePath);
            static void loadPlugin(Plugin *plugin);
            static void unloadPlugin(Plugin *plugin);
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

        std::string getAbsolutePath(const std::string& path);
    }
}

#endif //APLUGINLIBRARY_PLUGINMANAGERPRIVATE_H
