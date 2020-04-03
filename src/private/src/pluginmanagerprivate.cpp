#include "../pluginmanagerprivate.h"

#include <algorithm>
#include <climits>
# include <cstdlib>

#ifdef _WIN32
# define realpath(N,R) _fullpath((R),(N),_MAX_PATH)
# ifndef PATH_MAX
#  define PATH_MAX _MAX_PATH
# endif
#endif

#include "APluginLibrary/pluginmanagerobserver.h"


std::unordered_map<std::string, std::pair<size_t, apl::Plugin*>> apl::detail::PluginManagerPrivate::allPlugins;
std::mutex apl::detail::PluginManagerPrivate::staticMutex;

apl::Plugin* apl::detail::PluginManagerPrivate::loadPlugin(std::string path)
{
    std::string absolutePath = detail::getAbsolutePath((path + ".").append(LibraryLoader::libExtension()));
    std::lock_guard<std::mutex> lockGuard(staticMutex);
    auto iterator = allPlugins.find(absolutePath);
    if(iterator != allPlugins.end()) {
        iterator->second.first += 1;
        return iterator->second.second;
    }
    Plugin* plugin = Plugin::load(std::move(path));
    if(plugin != nullptr)
        allPlugins.emplace(std::move(absolutePath), std::make_pair(1, plugin));
    return plugin;
}

void apl::detail::PluginManagerPrivate::loadPlugin(apl::Plugin* plugin)
{
    if(plugin == nullptr)
        return;
    std::string absolutePath = detail::getAbsolutePath(plugin->getPath().append(".").append(LibraryLoader::libExtension()));
    staticMutex.lock();
    auto iterator = allPlugins.find(absolutePath);
    if(iterator != allPlugins.end())
        iterator->second.first += 1;
    else
        allPlugins.emplace(std::move(absolutePath), std::make_pair(1, plugin));
    staticMutex.unlock();
}

void apl::detail::PluginManagerPrivate::unloadPlugin(apl::Plugin* plugin)
{
    if(plugin == nullptr)
        return;
    std::string absolutePath = detail::getAbsolutePath(plugin->getPath().append(".").append(LibraryLoader::libExtension()));
    staticMutex.lock();
    auto iterator = allPlugins.find(absolutePath);
    if(iterator != allPlugins.end() && (iterator->second.first -= 1) == 0) {
        delete iterator->second.second;
        allPlugins.erase(iterator);
    }
    staticMutex.unlock();
}

std::string apl::detail::filterPluginInfo(const PluginInfo *info, PluginInfoFilter filter)
{
    if(filter == PluginInfoFilter::PluginName) {
        return info->pluginName;
    } else if(filter == PluginInfoFilter::PluginVersion) {
        return std::to_string(info->pluginVersionMajor).append(".").append(std::to_string(info->pluginVersionMinor)).append(".").append(std::to_string(info->pluginVersionPatch));
    } else if(filter == PluginInfoFilter::ApiVersion) {
        return std::to_string(info->apiVersionMajor).append(".").append(std::to_string(info->apiVersionMinor)).append(".").append(std::to_string(info->apiVersionPatch));
    } else {
        throw std::runtime_error("Unsupported apl::PluginInfoFilter");
    }
}
const char* apl::detail::filterFeatureInfo(const PluginFeatureInfo *info, const PluginFeatureFilter filter)
{
    if(filter == PluginFeatureFilter::FeatureGroup) {
        return info->featureGroup;
    } else if(filter == PluginFeatureFilter::FeatureName) {
        return info->featureName;
    } else if(filter == PluginFeatureFilter::ReturnType) {
        return info->returnType;
    } else if(filter == PluginFeatureFilter::ParameterList) {
        return info->parameterList;
    } else if (filter == PluginFeatureFilter::ParameterTypes) {
        return info->parameterTypes;
    } else if (filter == PluginFeatureFilter::ParameterNames) {
        return info->parameterNames;
    } else {
        throw std::runtime_error("Unsupported apl::PluginFeatureFilter");
    }
}
const char* apl::detail::filterClassInfo(const PluginClassInfo* info, PluginClassFilter filter)
{
    if(filter == PluginClassFilter::InterfaceName) {
        return info->interfaceName;
    } else if(filter == PluginClassFilter::ClassName) {
        return info->className;
    } else {
        throw std::runtime_error("Unimplemented apl::PluginFeatureFilter");
    }
}

std::string apl::detail::getAbsolutePath(const std::string& path)
{
    char buf[PATH_MAX];
    char *res = realpath(path.c_str(), buf);
    if(res)
        return buf;
    return path;
}
