#ifndef APLUGINLIBRARY_PLUGINMANAGER_TPP
#define APLUGINLIBRARY_PLUGINMANAGER_TPP

template<typename... Args>
bool apl::PluginManager::load(std::string path, Args... paths)
{
    return load(std::move(path)) ? load(paths...) : false;
}
template<typename... Args>
void apl::PluginManager::unload(Plugin* plugin, Args... plugins)
{
    unload(plugin);
    unload(plugins...);
}

#endif //APLUGINLIBRARY_PLUGINMANAGER_TPP