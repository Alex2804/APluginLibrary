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

    enum class PluginInfoFilter
    {
        PluginName,
        PluginVersion,
        ApiVersion
    };
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
        size_t loadDirectory(const std::string& path, bool recursive);

        size_t getLoadedPluginCount() const;
        std::vector<Plugin*> getLoadedPlugins();

        void unload(Plugin* plugin);
        void unload(const std::string& path);
        void unloadAll();

        std::vector<const PluginInfo*> getPluginInfos() const;
        std::vector<const PluginInfo*> getPluginInfos(const std::string& string, PluginInfoFilter = PluginInfoFilter::PluginName) const;
        std::vector<std::string> getPluginProperties(PluginInfoFilter filter) const;

        std::vector<const PluginFeatureInfo*> getFeatures() const;
        std::vector<const PluginFeatureInfo*> getFeatures(const std::string& string, PluginFeatureFilter filter = PluginFeatureFilter::FeatureGroup) const;
        std::vector<std::string> getFeatureProperties(PluginFeatureFilter filter) const;

        std::vector<const PluginClassInfo*> getClasses() const;
        std::vector<const PluginClassInfo*> getClasses(const std::string& string, PluginClassFilter filter = PluginClassFilter::InterfaceName) const;
        std::vector<std::string> getClassProperties(PluginClassFilter filter) const;

    private:
        detail::PluginManagerPrivate* d_ptr;
    };
}

#endif //APLUGINLIBRARY_PLUGINMANAGER_H
