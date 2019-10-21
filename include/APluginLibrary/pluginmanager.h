#ifndef APLUGINLIBRARY_PLUGINMANAGER_H
#define APLUGINLIBRARY_PLUGINMANAGER_H

#include <vector>

#include "plugin.h"
#include "plugininfos.h"

namespace apl
{
    namespace detail
    {
        struct PluginManagerPrivate;
    }

    enum class PluginFeatureFilter
    {
        FeatureGroup,
        FeatureName,
        ReturnType,
        ArgumentList,
    };
    enum class PluginClassFilter
    {
        InterfaceName,
        ClassName
    };

    class PluginManager
    {
    public:
        PluginManager();
        PluginManager(const PluginManager& other);
        PluginManager(PluginManager&& other) noexcept;
        virtual ~PluginManager();

        PluginManager& operator=(const PluginManager& other);
        PluginManager& operator=(PluginManager&& other) noexcept;

        bool load(std::string path);
        template<typename... Args>
        bool load(std::string path, Args... paths);

        size_t getLoadedPluginCount();
        std::vector<Plugin*> getLoadedPlugins();

        void unload(Plugin* plugin);
        template<typename... Args>
        void unload(Plugin* plugin, Args... plugins);
        void unloadAll();

        std::vector<const PluginFeatureInfo*> getFeatures();
        std::vector<const PluginFeatureInfo*> getFeatures(const std::string& s, PluginFeatureFilter f = PluginFeatureFilter::FeatureGroup);

        std::vector<const PluginClassInfo*> getClasses();
        std::vector<const PluginClassInfo*> getClasses(const std::string& s, PluginClassFilter f = PluginClassFilter::InterfaceName);

    private:
        detail::PluginManagerPrivate* d_ptr;
    };
}

#include "pluginmanager.tpp"

#endif //APLUGINLIBRARY_PLUGINMANAGER_H
