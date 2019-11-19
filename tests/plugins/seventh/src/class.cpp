#include "APluginSDK/pluginapi.h"
#include "../../otherinterface.h"

class Implementation1 : public OtherInterface
{
public:
    int otherFunction1(int x1, int x2, int x3) override;
    const char* otherFunction2() override;
    int otherFunction3(double x) override;
};

A_PLUGIN_REGISTER_CLASS(OtherInterface, Implementation1);

int Implementation1::otherFunction1(int x1, int x2, int x3)
{
    return x1 + x2 - x3;
}
const char *Implementation1::otherFunction2()
{
    return "This is for testing!";
}

int Implementation1::otherFunction3(double x) {
    return static_cast<int>(x);
}
