#ifndef APLUGINLIBRARY_PLUGINMANAGER_H
#define APLUGINLIBRARY_PLUGINMANAGER_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <vector>

#include "APluginLibrary/plugin.h"
#include "APluginSDK/plugininfos.h"

namespace apl
{
    namespace detail
    {
        class PluginManagerPrivate;
    }

    enum class PluginFeatureFilter
    {
        FeatureGroup,
        FeatureName,
        ReturnType,
        ParameterList,
        ParameterTypes,
        ParameterNames
    };
    enum class PluginClassFilter
    {
        InterfaceName,
        ClassName
    };

    class APLUGINLIBRARY_EXPORT PluginManager
    {
    public:
        PluginManager();
        PluginManager(const PluginManager& other);
        PluginManager(PluginManager&& other) noexcept;
        ~PluginManager();

        PluginManager& operator=(const PluginManager& other);
        PluginManager& operator=(PluginManager&& other) noexcept;

        bool load(std::string path);
        int loadDirectory(const std::string& path, bool recursive);

        size_t getLoadedPluginCount();
        std::vector<Plugin*> getLoadedPlugins();

        void unload(Plugin* plugin);
        void unloadAll();

        std::vector<const PluginFeatureInfo*> getFeatures();
        std::vector<const PluginFeatureInfo*> getFeatures(const std::string& string, const PluginFeatureFilter& filter = PluginFeatureFilter::FeatureGroup);
        std::vector<std::string> getFeatureProperties(const PluginFeatureFilter& filter);

        std::vector<const PluginClassInfo*> getClasses();
        std::vector<const PluginClassInfo*> getClasses(const std::string& string, const PluginClassFilter& filter = PluginClassFilter::InterfaceName);
        std::vector<std::string> getClassProperties(const PluginClassFilter& filter);

    private:
        detail::PluginManagerPrivate* d_ptr;
    };
}

#endif //APLUGINLIBRARY_PLUGINMANAGER_H
