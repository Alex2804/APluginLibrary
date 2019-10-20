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
    class AFeatureManager
    {
    public:
        AFeatureManager() = default;
        AFeatureManager(const AFeatureManager &other) = delete;
        AFeatureManager(AFeatureManager &&other) noexcept = delete;
        ~AFeatureManager();

        AFeatureManager& operator=(const AFeatureManager &other) = delete;
        AFeatureManager& operator=(AFeatureManager &&other) noexcept = delete;

        static APluginFeatureInfo* registerFeature(const char *featureGroup, const char *featureName,
                                                   const char *returnType, const char *argumentList,
                                                   void *functionPointer);
        int getFeatureCount() const;
        const APluginFeatureInfo* getFeatureInfo(int i) const;
        const APluginFeatureInfo* const* getFeatureInfos() const;

        static APluginClassInfo* registerClass(const char *interfaceClassName, const char *featureClassName,
                                               void *createInstance, void *deleteInstance);
        int getClassCount() const;
        const APluginClassInfo* getClassInfo(int i) const;
        const APluginClassInfo* const* getClassInfos() const;

    private:
        std::vector<APluginFeatureInfo*> feature_infos;
        std::vector<APluginClassInfo*> class_infos;

        static std::mutex feature_infos_mutex;
        static std::mutex class_infos_mutex;
    };

    std::mutex apl::AFeatureManager::feature_infos_mutex;
    std::mutex apl::AFeatureManager::class_infos_mutex;

    AFeatureManager featureManagerInstance;
} // namespace apl

apl::AFeatureManager::~AFeatureManager()
{
    for(APluginFeatureInfo* info : feature_infos) {
        delete info;
    }
    for(APluginClassInfo* info : class_infos) {
        delete info;
    }
}

apl::APluginFeatureInfo* apl::AFeatureManager::registerFeature(const char *featureGroup, const char *featureName,
                                                               const char *returnType, const char *argumentList,
                                                               void *functionPointer)
{
    auto info = new APluginFeatureInfo();
    info->featureGroup = featureGroup;
    info->featureName = featureName;
    info->returnType = returnType;
    info->argumentList = argumentList;
    info->functionPointer = functionPointer;
    feature_infos_mutex.lock();
    featureManagerInstance.feature_infos.push_back(info);
    feature_infos_mutex.unlock();
    return info;
}
int apl::AFeatureManager::getFeatureCount() const
{
    std::lock_guard<std::mutex> lockGuard(feature_infos_mutex);
    return feature_infos.size();
}
const apl::APluginFeatureInfo* apl::AFeatureManager::getFeatureInfo(int i) const
{
    std::lock_guard<std::mutex> lockGuard(feature_infos_mutex);
    return feature_infos.at(i);
}
const apl::APluginFeatureInfo* const* apl::AFeatureManager::getFeatureInfos() const
{
    std::lock_guard<std::mutex> lockGuard(feature_infos_mutex);
    return feature_infos.data();
}

apl::APluginClassInfo* apl::AFeatureManager::registerClass(const char *interfaceClassName,
                                                           const char *featureClassName,
                                                           void* createInstance, void* deleteInstance)
{
    auto info = new APluginClassInfo();
    info->interfaceClassName = interfaceClassName;
    info->className = featureClassName;
    info->createInstance = createInstance;
    info->deleteInstance = deleteInstance;
    class_infos_mutex.lock();
    featureManagerInstance.class_infos.push_back(info);
    class_infos_mutex.unlock();
    return info;
}
int apl::AFeatureManager::getClassCount() const
{
    std::lock_guard<std::mutex> lockGuard(class_infos_mutex);
    return class_infos.size();
}
const apl::APluginClassInfo* apl::AFeatureManager::getClassInfo(int i) const
{
    std::lock_guard<std::mutex> lockGuard(class_infos_mutex);
    return class_infos.at(i);
}
const apl::APluginClassInfo* const* apl::AFeatureManager::getClassInfos() const
{
    std::lock_guard<std::mutex> lockGuard(class_infos_mutex);
    return class_infos.data();
}

namespace apl
{
    extern "C"
    {
        A_PLUGIN_API int getPluginFeatureCount();
        A_PLUGIN_API const APluginFeatureInfo* getPluginFeatureInfo(int i);
        A_PLUGIN_API const APluginFeatureInfo* const* getPluginFeatureInfos();

        A_PLUGIN_API int getPluginClassCount();
        A_PLUGIN_API const APluginClassInfo* getPluginClassInfo(int i);
        A_PLUGIN_API const APluginClassInfo* const* getPluginClassInfos();
    }
} // namespace apl

int apl::getPluginFeatureCount() {
    return apl::featureManagerInstance.getFeatureCount();
}
const apl::APluginFeatureInfo* apl::getPluginFeatureInfo(int i) {
    return apl::featureManagerInstance.getFeatureInfo(i);
}
const apl::APluginFeatureInfo * const* apl::getPluginFeatureInfos() {
    return apl::featureManagerInstance.getFeatureInfos();
}

int apl::getPluginClassCount() {
    return apl::featureManagerInstance.getClassCount();
}
const apl::APluginClassInfo* apl::getPluginClassInfo(int i) {
    return apl::featureManagerInstance.getClassInfo(i);
}
const apl::APluginClassInfo* const* apl::getPluginClassInfos() {
    return apl::featureManagerInstance.getClassInfos();
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
            apl::AFeatureManager::registerFeature(#featureGroup, #featureName, #returnType, #__VA_ARGS__,              \
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
            apl::AFeatureManager::registerClass(#interfaceName, #className,                                            \
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
