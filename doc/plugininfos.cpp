/**
 * @struct apl::PluginFeatureInfo
 *
 * @brief For every feature in a plugin, a new PluginFeatureInfo is generated with infos about this feature.
 *
 * @var apl::PluginFeatureInfo::pluginInfo
 * A pointer to the apl::PluginInfo object of this plugin.
 *
 * @var apl::PluginFeatureInfo::featureGroup
 * The group of this feature.
 * @var apl::PluginFeatureInfo::featureName
 * The name of this feature.
 *
 * @var apl::PluginFeatureInfo::returnType
 * The return type of this feature.
 *
 * @var apl::PluginFeatureInfo::parameterList
 * The parameter list of this feature.
 * @var apl::PluginFeatureInfo::parameterTypes
 * The types of the parameters in the parameter list of this feature.
 * @var apl::PluginFeatureInfo::parameterNames
 * The names of the parameters in the parameter list of this feature.
 *
 * @var apl::PluginFeatureInfo::functionPointer
 * The pointer to the feature function.
 */


/**
 * @struct apl::PluginClassInfo
 *
 * @brief For every class in a plugin, a new PluginClassInfo is generated with infos about this class.
 *
 * @var apl::PluginClassInfo::pluginInfo
 * A pointer to the apl::PluginInfo object of this plugin.
 *
 * @var apl::PluginClassInfo::interfaceName
 * The name of the interface from which this class inherits.
 *
 * @var apl::PluginClassInfo::className
 * The name of the class.
 *
 * @var apl::PluginClassInfo::createInstance
 * The (factory) method to create an instance of the class.
 * @var apl::PluginClassInfo::deleteInstance
 * The method to delete an instance of the class.
 */


/**
 * @struct apl::PluginInfo
 *
 * @brief For every plugin, a new PluginInfo is generated with infos about this plugin.
 *
 * @var apl::PluginInfo::pluginName
 * The name of this plugin (empty if not set)
 *
 * @var apl::PluginInfo::apiVersionMajor
 * The major of the API version of APluginSDK.
 * @var apl::PluginInfo::apiVersionMinor
 * The minor of the API version of APluginSDK.
 * @var apl::PluginInfo::apiVersionPatch
 * The patch of the API version of APluginSDK.
 *
 * @var apl::PluginInfo::pluginVersionMajor
 * The major of the API version of this plugin.
 * @var apl::PluginInfo::pluginVersionMinor
 * The minor of the version of this plugin.
 * @var apl::PluginInfo::pluginVersionPatch
 * The patch of the version of this plugin.
 *
 * @var apl::PluginInfo::allocateMemory
 * The allocate function of this plugin.
 * @var apl::PluginInfo::freeMemory
 * The free function of this plugin.
 *
 * @var apl::PluginInfo::getFeatureCount
 * Function pointer that returns the number of registered features in this plugin.
 * @var apl::PluginInfo::getFeatureInfo
 * Function pointer that returns the feature at the passed index in this plugin.
 * @var apl::PluginInfo::getFeatureInfos
 * Function pointer that returns an array of all registered features in this plugin.
 *
 * @var apl::PluginInfo::getClassCount
 * Function pointer that returns the number of registered classes in this plugin.
 * @var apl::PluginInfo::getClassInfo
 * Function pointer that returns the class at the passed index in this plugin.
 * @var apl::PluginInfo::getClassInfos
 * Function pointer that returns an array of all registered classes in this plugin.
 */