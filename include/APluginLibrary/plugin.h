#ifndef APLUGINLIBRARY_PLUGIN_H
#define APLUGINLIBRARY_PLUGIN_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <string>

#include "APluginLibrary/libraryloader.h"
#include "APluginSDK/plugininfos.h"

namespace apl
{
    namespace detail
    {
        struct PluginPrivate;
    }

    class APLUGINLIBRARY_EXPORT Plugin
    {
    public:
        Plugin(const Plugin& other) = delete; ///< @private
        Plugin(Plugin&& other) noexcept = delete; ///< @private
        ~Plugin();

        Plugin& operator=(const Plugin& other) = delete; ///< @private
        Plugin& operator=(Plugin&& other) noexcept = delete; ///< @private

        static Plugin* load(std::string path);
        void unload();
        bool isLoaded() const;

        std::string getPath() const;
        const_library_handle getHandle() const;

        void* allocateMemory(size_t bytes) const;
        bool freeMemory(void* ptr) const;

        size_t getFeatureCount() const;
        const PluginFeatureInfo* getFeatureInfo(size_t index) const;
        const PluginFeatureInfo* const* getFeatureInfos() const;

        size_t getClassCount() const;
        const PluginClassInfo* getClassInfo(size_t index) const;
        const PluginClassInfo* const* getClassInfos() const;

    private:
        explicit Plugin(std::string path, library_handle handle);

        detail::PluginPrivate* d_ptr;
    };
}


#endif //APLUGINLIBRARY_PLUGIN_H
