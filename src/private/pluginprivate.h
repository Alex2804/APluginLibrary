#ifndef APLUGINLIBRARY_PLUGINPRIVATE_H
#define APLUGINLIBRARY_PLUGINPRIVATE_H

#include <string>

#include "plugininfos.h"

namespace apl {
    namespace detail
    {
        typedef size_t(*getFeatureCountFunction)();
        typedef const PluginFeatureInfo*(*getFeatureInfoFunction)(size_t);
        typedef const PluginFeatureInfo*const*(*getFeatureInfosFunction)();

        typedef size_t(*getClassCountFunction)();
        typedef const PluginClassInfo*(*getClassInfoFunction)(size_t);
        typedef const PluginClassInfo*const*(*getClassInfosFunction)();

        struct PluginPrivate
        {
            std::string path;

            void *handle;

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
