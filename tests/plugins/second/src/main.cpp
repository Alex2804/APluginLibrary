#include "../../../../include/APluginLibrary/pluginapi.h"

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

A_PLUGIN_REGISTER_FEATURE(int, second_group_pow, feature_pow2, int x)
{
    return x * x;
}
A_PLUGIN_REGISTER_FEATURE(int, second_group_pow, feature_pow3, int x)
{
    return x * x * x;
}