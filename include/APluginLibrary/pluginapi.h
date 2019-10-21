#ifndef APLUGINLIBRARY_PLUGINAPI_H
#define APLUGINLIBRARY_PLUGINAPI_H

#include <vector>
#include <mutex>
#include <iostream>

#include "plugininfos.h"

#ifdef _WIN32
# define A_PLUGIN_API  __declspec( dllexport )
#else
# define A_PLUGIN_API
#endif

namespace apl {
    namespace detail
    {
        /**
         * interface to interact with the plugin
         */
        extern "C" // C linkage because the functions get loaded dynamically from a shared library
        {
            A_PLUGIN_API size_t getPluginFeatureCount();
            A_PLUGIN_API const apl::PluginFeatureInfo* getPluginFeatureInfo(size_t i);
            A_PLUGIN_API const apl::PluginFeatureInfo * const* getPluginFeatureInfos();
    
            A_PLUGIN_API size_t getPluginClassCount();
            A_PLUGIN_API const apl::PluginClassInfo* getPluginClassInfo(size_t i);
            A_PLUGIN_API const apl::PluginClassInfo* const* getPluginClassInfos();
        }
        
        class FeatureManager
        {
        public:
            FeatureManager() = default;
            FeatureManager(const FeatureManager &other) = delete;
            FeatureManager(FeatureManager &&other) noexcept = delete;
            ~FeatureManager();

            FeatureManager &operator=(const FeatureManager &other) = delete;
            FeatureManager &operator=(FeatureManager &&other) noexcept = delete;

            static PluginFeatureInfo *registerFeature(const char *featureGroup, const char *featureName,
                                                      const char *returnType, const char *argumentList,
                                                      void *functionPointer);
            size_t getFeatureCount() const;
            const PluginFeatureInfo *getFeatureInfo(size_t i) const;
            const PluginFeatureInfo *const *getFeatureInfos() const;

            static PluginClassInfo *registerClass(const char *interfaceClassName, const char *featureClassName,
                                                  void *createInstance, void *deleteInstance);
            size_t getClassCount() const;
            const PluginClassInfo *getClassInfo(size_t i) const;
            const PluginClassInfo *const *getClassInfos() const;

        private:
            std::vector<PluginFeatureInfo *> feature_infos;
            std::vector<PluginClassInfo *> class_infos;
        };

        FeatureManager featureManagerInstance; // no singleton because there should be one instance per shared library (plugin)
    } // namespace detail
} // namespace apl


/**
 * Interface implementation
 */
size_t apl::detail::getPluginFeatureCount()
{
    return apl::detail::featureManagerInstance.getFeatureCount();
}
const apl::PluginFeatureInfo* apl::detail::getPluginFeatureInfo(size_t i)
{
    return apl::detail::featureManagerInstance.getFeatureInfo(i);
}
const apl::PluginFeatureInfo * const* apl::detail::getPluginFeatureInfos()
{
    return apl::detail::featureManagerInstance.getFeatureInfos();
}

size_t apl::detail::getPluginClassCount()
{
    return apl::detail::featureManagerInstance.getClassCount();
}
const apl::PluginClassInfo* apl::detail::getPluginClassInfo(size_t i)
{
    return apl::detail::featureManagerInstance.getClassInfo(i);
}
const apl::PluginClassInfo* const* apl::detail::getPluginClassInfos()
{
    return apl::detail::featureManagerInstance.getClassInfos();
}


/**
 * AFeatureManager implementation
 */
apl::detail::FeatureManager::~FeatureManager()
{
    for(PluginFeatureInfo* info : feature_infos) {
        delete info;
    }
    for(PluginClassInfo* info : class_infos) {
        delete info;
    }
}

apl::PluginFeatureInfo* apl::detail::FeatureManager::registerFeature(const char *featureGroup, const char *featureName,
                                                                     const char *returnType, const char *argumentList,
                                                                     void *functionPointer)
{
    auto info = new PluginFeatureInfo();
    info->featureGroup = featureGroup;
    info->featureName = featureName;
    info->returnType = returnType;
    info->argumentList = argumentList;
    info->functionPointer = functionPointer;
    featureManagerInstance.feature_infos.push_back(info);
    return info;
}
size_t apl::detail::FeatureManager::getFeatureCount() const
{
    return feature_infos.size();
}
const apl::PluginFeatureInfo* apl::detail::FeatureManager::getFeatureInfo(size_t i) const
{
    return feature_infos.at(i);
}
const apl::PluginFeatureInfo* const* apl::detail::FeatureManager::getFeatureInfos() const
{
    return feature_infos.data();
}

apl::PluginClassInfo* apl::detail::FeatureManager::registerClass(const char *interfaceClassName,
                                                                 const char *featureClassName,
                                                                 void* createInstance, void* deleteInstance)
{
    auto info = new PluginClassInfo();
    info->interfaceClassName = interfaceClassName;
    info->className = featureClassName;
    info->createInstance = createInstance;
    info->deleteInstance = deleteInstance;
    featureManagerInstance.class_infos.push_back(info);
    return info;
}
size_t apl::detail::FeatureManager::getClassCount() const
{
    return class_infos.size();
}
const apl::PluginClassInfo* apl::detail::FeatureManager::getClassInfo(size_t i) const
{
    return class_infos.at(i);
}
const apl::PluginClassInfo* const* apl::detail::FeatureManager::getClassInfos() const
{
    return class_infos.data();
}


#define A_PLUGIN_FEATURE_NAMESPACE(featureGroup, featureName) \
    featureGroup##_feature_group::featureName##_feature_name::
#define A_PLUGIN_FEATURE_OPEN_NAMESPACE(featureGroup, featureName) \
    namespace featureGroup##_feature_group { namespace featureName##_feature_name {
#define A_PLUGIN_FEATURE_CLOSE_NAMESPACE \
    }}

#define A_PLUGIN_FEATURE_NAME(featureGroup, featureName) \
    APluginFeature_##featureGroup##_##featureName

#define A_PLUGIN_FEATURE_REGISTRATION(returnType, featureGroup, featureName, ...)                                      \
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



#define A_PLUGIN_CLASS_OPEN_NAMESPACE(interfaceName, className) \
    namespace interfaceName##_interface_name { namespace className##_class_name {
#define A_PLUGIN_CLASS_CLOSE_NAMESPACE \
    }}

#define A_PLUGIN_CLASS_NAME(interfaceName, className) \
    APluginClass_##interfaceName##_##className

#define A_PLUGIN_CLASS_REGISTRATION(interfaceName, className)                                                          \
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
