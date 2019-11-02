#ifndef APLUGINLIBRARY_PLUGINAPI_H
#define APLUGINLIBRARY_PLUGINAPI_H

#include <vector>
#include <mutex>
#include <iostream>

#include "featuremanager.h"
#include "implementation/macros.ipp"

namespace apl
{
    namespace detail
    {
        extern "C"
        {
            A_PLUGIN_API size_t getPluginFeatureCount();
            A_PLUGIN_API const apl::PluginFeatureInfo* getPluginFeatureInfo(size_t i);
            A_PLUGIN_API const apl::PluginFeatureInfo * const* getPluginFeatureInfos();

            A_PLUGIN_API size_t getPluginClassCount();
            A_PLUGIN_API const apl::PluginClassInfo* getPluginClassInfo(size_t i);
            A_PLUGIN_API const apl::PluginClassInfo* const* getPluginClassInfos();
        }
    }
}

#include "implementation/pluginapi.ipp"

#define A_PLUGIN_REGISTER_FEATURE(returnType, featureGroup, featureName, ...)                                          \
    A_PLUGIN_FEATURE_OPEN_NAMESPACE(featureGroup, featureName)                                                         \
        class A_PLUGIN_FEATURE_NAME(featureGroup, featureName)                                                         \
        {                                                                                                              \
        public:                                                                                                        \
            static apl::PluginFeatureInfo* feature_info;                                                               \
            static returnType featureBody(__VA_ARGS__);                                                                \
        };                                                                                                             \
                                                                                                                       \
        apl::PluginFeatureInfo* A_PLUGIN_FEATURE_NAME(featureGroup, featureName)::feature_info =                       \
            apl::detail::FeatureManager::registerFeature(#featureGroup, #featureName, #returnType, #__VA_ARGS__,       \
                reinterpret_cast<void*>(A_PLUGIN_FEATURE_NAME(featureGroup, featureName)::featureBody));               \
    A_PLUGIN_FEATURE_CLOSE_NAMESPACE                                                                                   \
                                                                                                                       \
    returnType A_PLUGIN_FEATURE_NAMESPACE(featureGroup, featureName)A_PLUGIN_FEATURE_NAME(featureGroup, featureName)   \
                                                                                          ::featureBody(__VA_ARGS__)

#define A_PLUGIN_REGISTER_CLASS(interfaceName, className)                                                              \
    A_PLUGIN_CLASS_OPEN_NAMESPACE(interfaceName, className)                                                            \
        class A_PLUGIN_CLASS_NAME(interfaceName, className)                                                            \
        {                                                                                                              \
        public:                                                                                                        \
            static apl::PluginClassInfo* info;                                                                         \
            static className* createInstance();                                                                        \
            static void deleteInstance(className*);                                                                    \
        };                                                                                                             \
                                                                                                                       \
        apl::PluginClassInfo* A_PLUGIN_CLASS_NAME(interfaceName, className)::info =                                    \
            apl::detail::FeatureManager::registerClass(#interfaceName, #className,                                     \
                reinterpret_cast<void*>(A_PLUGIN_CLASS_NAME(interfaceName, className)::createInstance),                \
                reinterpret_cast<void*>(A_PLUGIN_CLASS_NAME(interfaceName, className)::deleteInstance));               \
                                                                                                                       \
        className* A_PLUGIN_CLASS_NAME(interfaceName, className)::createInstance()                                     \
        {                                                                                                              \
            return new className();                                                                                    \
        }                                                                                                              \
        void A_PLUGIN_CLASS_NAME(interfaceName, className)::deleteInstance(className* ptr)                             \
        {                                                                                                              \
            delete ptr;                                                                                                \
        }                                                                                                              \
    A_PLUGIN_CLASS_CLOSE_NAMESPACE

#endif //APLUGINLIBRARY_PLUGINAPI_H
