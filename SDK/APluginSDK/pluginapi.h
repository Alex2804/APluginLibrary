#ifndef APLUGINSDK_PLUGINAPI_H
#define APLUGINSDK_PLUGINAPI_H

#include "private/infomanager.h"

PRIVATE_APLUGINLIBRARY_OPEN_NAMESPACE
    const size_t A_PLUGIN_API_VERSION_MAJOR = 3;
    const size_t A_PLUGIN_API_VERSION_MINOR = 0;
    const size_t A_PLUGIN_API_VERSION_PATCH = 0;

    PRIVATE_APLUGINSDK_OPEN_EXTERN_C
        APLUGINSDK_API void* allocateMemory(size_t size);
        APLUGINSDK_API void freeMemory(void* ptr);
    PRIVATE_APLUGINSDK_CLOSE_EXTERN_C
PRIVATE_APLUGINLIBRARY_CLOSE_NAMESPACE

PRIVATE_APLUGINSDK_OPEN_API_NAMESPACE
    PRIVATE_APLUGINSDK_OPEN_EXTERN_C
        APLUGINSDK_API void initAPlugin(); /* existence must be checked */
        APLUGINSDK_API void finiAPlugin(); /* existence must be checked */
        APLUGINSDK_API const struct APluginInfo* getAPluginInfo();
    PRIVATE_APLUGINSDK_CLOSE_EXTERN_C
PRIVATE_APLUGINSDK_CLOSE_API_NAMESPACE

#define A_PLUGIN_INIT PRIVATE_APLUGINSDK_INIT_FUNCTION

#define A_PLUGIN_FINI PRIVATE_APLUGINSDK_FINI_FUNCTION

#define A_PLUGIN_SET_NAME(name) PRIVATE_APLUGINSDK_SET_NAME(name)

#define A_PLUGIN_SET_VERSION(major, minor, patch) PRIVATE_APLUGINSDK_SET_VERSION(major, minor, patch)

#define A_PLUGIN_REGISTER_FEATURE(returnType, featureGroup, featureName, ...) \
    PRIVATE_APLUGINSDK_REGISTER_FEATURE(returnType, featureGroup, featureName, __VA_ARGS__)
#define A_PLUGIN_RECORD_FEATURE(featureGroup, featureName) \
    PRIVATE_APLUGINSDK_RECORD_FEATURE(featureGroup, featureName)

#ifdef __cplusplus
    #define A_PLUGIN_REGISTER_CLASS(interfaceName, className) \
        PRIVATE_APLUGINSDK_REGISTER_CLASS(interfaceName, className)
#endif

#if !APLUGINSDK_EXCLUDE_IMPLEMENTATION
    #ifdef __cplusplus
        #include "src/cpp/pluginapi.cpp"
    #else
        #include "src/c/pluginapi.c"
    #endif
#endif

#endif /* APLUGINSDK_PLUGINAPI_H */
