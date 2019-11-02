#ifndef APLUGINLIBRARY_MACROS_IPP
#define APLUGINLIBRARY_MACROS_IPP

#ifdef _WIN32
# define A_PLUGIN_API  __declspec( dllexport )
#else
# define A_PLUGIN_API
#endif


#define A_PLUGIN_FEATURE_NAMESPACE(featureGroup, featureName) \
    featureGroup##_feature_group::featureName##_feature_name::
#define A_PLUGIN_FEATURE_OPEN_NAMESPACE(featureGroup, featureName) \
    namespace featureGroup##_feature_group { namespace featureName##_feature_name {
#define A_PLUGIN_FEATURE_CLOSE_NAMESPACE \
    }}

#define A_PLUGIN_FEATURE_NAME(featureGroup, featureName) \
    APluginFeature_##featureGroup##_##featureName


#define A_PLUGIN_CLASS_OPEN_NAMESPACE(interfaceName, className) \
    namespace interfaceName##_interface_name { namespace className##_class_name {
#define A_PLUGIN_CLASS_CLOSE_NAMESPACE \
    }}

#define A_PLUGIN_CLASS_NAME(interfaceName, className) \
    APluginClass_##interfaceName##_##className

#endif //APLUGINLIBRARY_MACROS_IPP
