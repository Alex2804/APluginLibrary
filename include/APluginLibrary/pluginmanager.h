#ifndef APLUGINLIBRARY_PLUGINMANAGER_H
#define APLUGINLIBRARY_PLUGINMANAGER_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <vector>

#include "APluginLibrary/plugin.h"
#include "APluginLibrary/plugininfos.h"

namespace apl
{
    namespace detail
    {
        struct PluginManagerPrivate;
    }

    enum class APLUGINLIBRARY_EXPORT PluginFeatureFilter
    {
        FeatureGroup,
        FeatureName,
        ReturnType,
        ArgumentList,
    };
    enum class APLUGINLIBRARY_EXPORT PluginClassFilter
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
        std::vector<const PluginFeatureInfo*> getFeatures(const std::string& s, const PluginFeatureFilter& f = PluginFeatureFilter::FeatureGroup);
        std::vector<std::string> getFeatureProperties(const PluginFeatureFilter& f);

        std::vector<const PluginClassInfo*> getClasses();
        std::vector<const PluginClassInfo*> getClasses(const std::string& s, const PluginClassFilter& f = PluginClassFilter::InterfaceName);
        std::vector<std::string> getClassProperties(const PluginClassFilter& f);

    private:
        detail::PluginManagerPrivate* d_ptr;
    };
}

#include "implementation/pluginmanager.tpp"

#endif //APLUGINLIBRARY_PLUGINMANAGER_H
