#include "APluginSDK/pluginapi.h"

A_PLUGIN_REGISTER_FEATURE(int, second_group_pow, feature_pow2, int x)
{
    return x * x;
}
A_PLUGIN_REGISTER_FEATURE(int, second_group_pow, feature_pow3, int x)
{
    return x * x * x;
}

A_PLUGIN_SET_VERSION(3, 5, 12)
