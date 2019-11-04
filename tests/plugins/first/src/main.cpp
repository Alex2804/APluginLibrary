#include "APluginSDK/pluginapi.h"

A_PLUGIN_REGISTER_FEATURE(int, first_group1, feature1, int x1, int x2)
{
    return x1 * x2;
}