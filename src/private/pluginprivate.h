#ifndef APLUGINLIBRARY_PLUGINPRIVATE_H
#define APLUGINLIBRARY_PLUGINPRIVATE_H

#include "APluginLibrary/plugin.h"

#ifdef APLUGINLIBRARY_TEST
# undef APLUGINLIBRARY_NO_EXPORT
# define APLUGINLIBRARY_NO_EXPORT APLUGINLIBRARY_EXPORT
#endif

namespace apl {
    namespace detail
    {
        class APLUGINLIBRARY_NO_EXPORT PluginPrivate
        {
        public:
            std::string libraryPath;
            library_handle libraryHandle = nullptr;

            const PluginInfo* pluginInfo = nullptr;
            void(*initPlugin)() = nullptr;
            void(*finiPlugin)() = nullptr;

            void reset();
        };
    }
}


#endif //APLUGINLIBRARY_PLUGINPRIVATE_H
