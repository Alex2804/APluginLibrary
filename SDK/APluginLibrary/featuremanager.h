#ifndef APLUGINLIBRARY_FEATUREMANAGER_H
#define APLUGINLIBRARY_FEATUREMANAGER_H

#include "plugininfos.h"

namespace apl
{
    namespace detail
    {
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
            std::vector<PluginFeatureInfo*> feature_infos;
            std::vector<PluginClassInfo*> class_infos;
        };

        FeatureManager featureManagerInstance; // no singleton because there should be one instance per shared library (plugin)
    }
}

#include "implementation/featuremanager.ipp"

#endif //APLUGINLIBRARY_FEATUREMANAGER_H
