#include "APluginSDK/pluginapi.h"
#include "../../include.h"

APLUGINSDK_EXPORT const char* firstPluginInitStatusString = "";

A_PLUGIN_REGISTER_FEATURE(int, first_group1, feature1, int x1, int x2)
{
    return x1 * x2;
}

A_PLUGIN_REGISTER_FEATURE(struct APluginLibrary_Test_PointStruct, first_group1, feature2, int y, int x)
{
    struct APluginLibrary_Test_PointStruct tmp = {.x = x, .y = y};
    return tmp;
}

A_PLUGIN_INIT
{
    A_PLUGIN_SET_NAME("first_plugin");
    A_PLUGIN_SET_VERSION(9, 87, 789);
    A_PLUGIN_RECORD_FEATURE(first_group1, feature1);
    A_PLUGIN_RECORD_FEATURE(first_group1, feature2);
    firstPluginInitStatusString = "first plugin -> initialized";
}

A_PLUGIN_FINI
{
    firstPluginInitStatusString = "first plugin -> finalized";
}
