#include "APluginSDK/pluginapi.h"

A_PLUGIN_REGISTER_FEATURE(int, first_group1, feature1, int x1, int x2)
{
    return x1 * x2;
}

A_PLUGIN_SET_NAME(first_plugin)
A_PLUGIN_SET_VERSION(9, 87, 789)
