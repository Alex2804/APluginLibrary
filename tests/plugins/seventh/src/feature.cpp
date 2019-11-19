#include "APluginSDK/pluginapi.h"

A_PLUGIN_REGISTER_FEATURE(char, seventh_group1, convert_to_char, int x)
{
    return static_cast<char>(x);
}
