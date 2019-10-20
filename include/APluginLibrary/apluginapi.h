#ifndef APLUGINLIBRARY_APLUGINAPI_H
#define APLUGINLIBRARY_APLUGINAPI_H

#include <vector>
#include <mutex>
#include <iostream>

#include "aplugininfos.h"

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
            A_PLUGIN_API int getPluginFeatureCount();
            A_PLUGIN_API const apl::APluginFeatureInfo* getPluginFeatureInfo(int i);
            A_PLUGIN_API const apl::APluginFeatureInfo * const* getPluginFeatureInfos();
    
            A_PLUGIN_API int getPluginClassCount();
            A_PLUGIN_API const apl::APluginClassInfo* getPluginClassInfo(int i);
            A_PLUGIN_API const apl::APluginClassInfo* const* getPluginClassInfos();
        }
        
        class AFeatureManager
        {
        public:
            AFeatureManager() = default;
            AFeatureManager(const AFeatureManager &other) = delete;
            AFeatureManager(AFeatureManager &&other) noexcept = delete;
            ~AFeatureManager();

            AFeatureManager &operator=(const AFeatureManager &other) = delete;
            AFeatureManager &operator=(AFeatureManager &&other) noexcept = delete;

            static APluginFeatureInfo *registerFeature(const char *featureGroup, const char *featureName,
                                                       const char *returnType, const char *argumentList,
                                                       void *functionPointer);
            int getFeatureCount() const;
            const APluginFeatureInfo *getFeatureInfo(int i) const;
            const APluginFeatureInfo *const *getFeatureInfos() const;

            static APluginClassInfo *registerClass(const char *interfaceClassName, const char *featureClassName,
                                                   void *createInstance, void *deleteInstance);
            int getClassCount() const;
            const APluginClassInfo *getClassInfo(int i) const;
            const APluginClassInfo *const *getClassInfos() const;

        private:
            std::vector<APluginFeatureInfo *> feature_infos;
            std::vector<APluginClassInfo *> class_infos;
        };

        AFeatureManager featureManagerInstance; // no singleton because there should be one instance per shared library (plugin)
    } // namespace detail
} // namespace apl


/**
 * Interface implementation
 */
int apl::detail::getPluginFeatureCount()
{
    return apl::detail::featureManagerInstance.getFeatureCount();
}
const apl::APluginFeatureInfo* apl::detail::getPluginFeatureInfo(int i)
{
    return apl::detail::featureManagerInstance.getFeatureInfo(i);
}
const apl::APluginFeatureInfo * const* apl::detail::getPluginFeatureInfos()
{
    return apl::detail::featureManagerInstance.getFeatureInfos();
}

int apl::detail::getPluginClassCount()
{
    return apl::detail::featureManagerInstance.getClassCount();
}
const apl::APluginClassInfo* apl::detail::getPluginClassInfo(int i)
{
    return apl::detail::featureManagerInstance.getClassInfo(i);
}
const apl::APluginClassInfo* const* apl::detail::getPluginClassInfos()
{
    return apl::detail::featureManagerInstance.getClassInfos();
}


/**
 * AFeatureManager implementation
 */
apl::detail::AFeatureManager::~AFeatureManager()
{
    for(APluginFeatureInfo* info : feature_infos) {
        delete info;
    }
    for(APluginClassInfo* info : class_infos) {
        delete info;
    }
}

apl::APluginFeatureInfo* apl::detail::AFeatureManager::registerFeature(const char *featureGroup, const char *featureName,
                                                               const char *returnType, const char *argumentList,
                                                               void *functionPointer)
{
    auto info = new APluginFeatureInfo();
    info->featureGroup = featureGroup;
    info->featureName = featureName;
    info->returnType = returnType;
    info->argumentList = argumentList;
    info->functionPointer = functionPointer;
    featureManagerInstance.feature_infos.push_back(info);
    return info;
}
int apl::detail::AFeatureManager::getFeatureCount() const
{
    return feature_infos.size();
}
const apl::APluginFeatureInfo* apl::detail::AFeatureManager::getFeatureInfo(int i) const
{
    return feature_infos.at(i);
}
const apl::APluginFeatureInfo* const* apl::detail::AFeatureManager::getFeatureInfos() const
{
    return feature_infos.data();
}

apl::APluginClassInfo* apl::detail::AFeatureManager::registerClass(const char *interfaceClassName,
                                                           const char *featureClassName,
                                                           void* createInstance, void* deleteInstance)
{
    auto info = new APluginClassInfo();
    info->interfaceClassName = interfaceClassName;
    info->className = featureClassName;
    info->createInstance = createInstance;
    info->deleteInstance = deleteInstance;
    featureManagerInstance.class_infos.push_back(info);
    return info;
}
int apl::detail::AFeatureManager::getClassCount() const
{
    return class_infos.size();
}
const apl::APluginClassInfo* apl::detail::AFeatureManager::getClassInfo(int i) const
{
    return class_infos.at(i);
}
const apl::APluginClassInfo* const* apl::detail::AFeatureManager::getClassInfos() const
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
            static apl::APluginFeatureInfo* feature_info;                                                              \
            static returnType featureBody(__VA_ARGS__);                                                                \
        };                                                                                                             \
                                                                                                                       \
        apl::APluginFeatureInfo* A_PLUGIN_FEATURE_NAME(featureGroup, featureName)::feature_info =                      \
            apl::detail::AFeatureManager::registerFeature(#featureGroup, #featureName, #returnType, #__VA_ARGS__,      \
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
            static apl::APluginClassInfo* info;                                                                        \
            static className* createInstance();                                                                        \
            static void deleteInstance(className*);                                                                    \
        };                                                                                                             \
                                                                                                                       \
        apl::APluginClassInfo* A_PLUGIN_CLASS_NAME(interfaceName, className)::info =                                   \
            apl::detail::AFeatureManager::registerClass(#interfaceName, #className,                                    \
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

#endif //APLUGINLIBRARY_APLUGINAPI_H
