#include "APluginSDK/pluginapi.h"

#include <cmath>

#include "../plugins/interface.h"

A_PLUGIN_SET_NAME(integrated_plugin_name)
A_PLUGIN_SET_VERSION(28, 4, 2000)

A_PLUGIN_REGISTER_FEATURE(double, integrated_feature_group, integrated_feature_1, double x1, double x2)
{
    return std::pow(x1, x2);
}

A_PLUGIN_REGISTER_FEATURE(int, integrated_feature_group, integrated_feature_2)
{
    return 42;
}

class APLUGINSDK_NO_EXPORT Implementation : public Interface
{
    int function1(int, int) override;
    int function2(int) override;
};

int Implementation::function1(int x1, int x2)
{
    return x1 + x2;
}
int Implementation::function2(int x)
{
    return x * 2;
}

A_PLUGIN_REGISTER_CLASS(Interface, Implementation);
