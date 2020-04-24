#include "APluginSDK/pluginapi.h"
#include "../../include.h"

A_PLUGIN_REGISTER_FEATURE(int, first_group1, feature1, int x1, int x2)
{
    return x1 * x2;
}

A_PLUGIN_REGISTER_FEATURE(afl::TestPointStruct, first_group1, feature2, int y, int x)
{
    return afl::TestPointStruct{x, y};
}

A_PLUGIN_SET_NAME(first_plugin)
A_PLUGIN_SET_VERSION(9, 87, 789)
