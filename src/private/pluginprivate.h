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
        typedef size_t(*getFeatureCountFunction)();
        typedef const PluginFeatureInfo*(*getFeatureInfoFunction)(size_t);
        typedef const PluginFeatureInfo*const*(*getFeatureInfosFunction)();

        typedef size_t(*getClassCountFunction)();
        typedef const PluginClassInfo*(*getClassInfoFunction)(size_t);
        typedef const PluginClassInfo*const*(*getClassInfosFunction)();

        struct APLUGINLIBRARY_NO_EXPORT PluginPrivate
        {
            std::string libraryPath;
            library_handle libraryHandle;

            getFeatureCountFunction getFeatureCount;
            getFeatureInfoFunction getFeatureInfo;
            getFeatureInfosFunction getFeatureInfos;

            getClassCountFunction getClassCount;
            getClassInfoFunction getClassInfo;
            getClassInfosFunction getClassInfos;
        };
    }
}


#endif //APLUGINLIBRARY_PLUGINPRIVATE_H
