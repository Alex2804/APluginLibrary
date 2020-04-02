#ifndef APLUGINLIBRARY_PLUGINMANAGEROBSERVER_H
#define APLUGINLIBRARY_PLUGINMANAGEROBSERVER_H

#include "APluginLibrary/apluginlibrary_export.h"

#include "APluginLibrary/pluginmanager.h"

namespace apl
{
    class APLUGINLIBRARY_EXPORT PluginManagerObserver
    {
    public:
        virtual ~PluginManagerObserver();

        /**
         * This function gets invoked if a plugin is loaded from a PluginManager where this observer is registered.
         * @param pluginManager The PluginManager where the plugin is loaded.
         * @param plugin  The Plugin which was loaded.
         */
        virtual void pluginLoaded(PluginManager* pluginManager, Plugin* plugin) = 0;
        /**
         * This function gets invoked if a plugin is loaded from a PluginManager where this observer is registered.
         * @param pluginManager The PluginManager where the plugin is loaded.
         * @param plugin  The Plugin which was loaded.
         */
        virtual void pluginUnloaded(PluginManager* pluginManager, Plugin* plugin) = 0;
    };
}

#endif //APLUGINLIBRARY_PLUGINMANAGEROBSERVER_H
