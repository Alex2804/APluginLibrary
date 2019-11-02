#ifndef APLUGINLIBRARY_PLUGINAPI_IPP
#define APLUGINLIBRARY_PLUGINAPI_IPP

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

#endif //APLUGINLIBRARY_PLUGINAPI_IPP
