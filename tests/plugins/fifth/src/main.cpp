#include "APluginSDK/pluginapi.h"
#include "../../interface.h"

A_PLUGIN_REGISTER_FEATURE(int, fifth_group1, feature1)
{
    return 4 + 2;
}

class APLUGINSDK_NO_EXPORT Implementation : public Interface
{
public:
    int function1(int x1, int x2) override;
    int function2(int x) override;
};

A_PLUGIN_REGISTER_CLASS(Interface, Implementation);

int Implementation::function1(int x1, int x2) {
    return x1 * x2;
}
int Implementation::function2(int x) {
    return x * x;
}
