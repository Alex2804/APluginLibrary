#ifndef APLUGINLIBRARY_PLUGIN_H
#define APLUGINLIBRARY_PLUGIN_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <string>

#include "APluginLibrary/plugininfos.h"
#include "APluginLibrary/libraryloader.h"

namespace apl
{
    namespace detail
    {
        struct PluginPrivate;
    }

    class APLUGINLIBRARY_EXPORT Plugin
    {
    public:
        Plugin(const Plugin& other) = delete;
        Plugin(Plugin&& other) noexcept = delete;
        virtual ~Plugin();

        Plugin& operator=(const Plugin& other) = delete;
        Plugin& operator=(Plugin&& other) noexcept = delete;

        static Plugin* load(const std::string& path);
        static Plugin* load(std::string path, library_handle handle);
        void unload();
        bool isLoaded() const;

        std::string getPath() const;
        const_library_handle getHandle() const;

        size_t getFeatureCount() const;
        const PluginFeatureInfo* getFeatureInfo(size_t i) const;
        const PluginFeatureInfo* const* getFeatureInfos() const;

        size_t getClassCount() const;
        const PluginClassInfo* getClassInfo(size_t i) const;
        const PluginClassInfo* const* getClassInfos() const;

    private:
        explicit Plugin(std::string path, library_handle handle);

        detail::PluginPrivate* d_ptr;
    };
}


#endif //APLUGINLIBRARY_PLUGIN_H
