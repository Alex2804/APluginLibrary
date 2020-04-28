#include "../pluginprivate.h"

void apl::detail::PluginPrivate::reset()
{
    libraryHandle = nullptr;
    pluginInfo = nullptr;
    initPlugin = nullptr;
    finiPlugin = nullptr;
}
