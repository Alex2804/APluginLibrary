#include "../../../../include/APluginLibrary/apluginapi.h"

A_PLUGIN_FEATURE_REGISTRATION(int, first_group1, feature1, int x1, int x2)
{
    return x1 * x2;
}