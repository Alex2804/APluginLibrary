#define A_PLUGIN_SDK_EXCLUDE_DEFINITIONS
#include "APluginSDK/pluginapi.h"

A_PLUGIN_REGISTER_FEATURE(int, second_group_math, feature_add, int x1, int x2)
{
    return x1 + x2;
}
A_PLUGIN_REGISTER_FEATURE(int, second_group_math, feature_sub, int x1, int x2)
{
    return x1 - x2;
}
A_PLUGIN_REGISTER_FEATURE(int, second_group_math, feature_mul, int x1, int x2)
{
    return x1 * x2;
}
A_PLUGIN_REGISTER_FEATURE(int, second_group_math, feature_div, int x1, int x2)
{
    return x1 / x2;
}

A_PLUGIN_SET_NAME(second_plugin)
