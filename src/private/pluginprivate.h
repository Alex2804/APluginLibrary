#ifndef APLUGINLIBRARY_PLUGINPRIVATE_H
#define APLUGINLIBRARY_PLUGINPRIVATE_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <string>

#include "APluginLibrary/libraryloader.h"
#include "APluginSDK/plugininfos.h"

#ifdef APLUGINLIBRARY_TEST
# undef APLUGINLIBRARY_NO_EXPORT
# define APLUGINLIBRARY_NO_EXPORT APLUGINLIBRARY_EXPORT
#endif

namespace apl {
    namespace detail
    {
        typedef const PluginInfo*(*getPluginInfoFunction)();

        struct APLUGINLIBRARY_NO_EXPORT PluginPrivate
        {
            std::string libraryPath;
            library_handle libraryHandle;

            getPluginInfoFunction getPluginInfo;
            const PluginInfo* pluginInfo;
        };
    }
}


#endif //APLUGINLIBRARY_PLUGINPRIVATE_H
