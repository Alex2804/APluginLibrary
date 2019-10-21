#ifndef APLUGINLIBRARY_PLUGININFOS_H
#define APLUGINLIBRARY_PLUGININFOS_H

namespace apl {
    extern "C" {
        struct PluginFeatureInfo
        {
            const char *featureGroup;
            const char *featureName;
            const char *returnType;
            const char *argumentList;
            void *functionPointer;
        };

        struct PluginClassInfo
        {
            const char *interfaceClassName;
            const char *className;
            void *createInstance;
            void *deleteInstance;
        };
    }
}

#endif //APLUGINLIBRARY_PLUGININFOS_H
