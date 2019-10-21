#include "../../../../include/APluginLibrary/pluginapi.h"
#include "../../interfaces.h"

A_PLUGIN_FEATURE_REGISTRATION(char, seventh_group1, convert_to_char, int x)
{
    return (char)x;
}


class Implementation1 : public OtherInterface
{
public:
    int otherFunction1(int x1, int x2, int x3) override;
    const char* otherFunction2() override;
};

A_PLUGIN_CLASS_REGISTRATION(OtherInterface, Implementation1);

int Implementation1::otherFunction1(int x1, int x2, int x3)
{
    return x1 + x2 - x3;
}
const char *Implementation1::otherFunction2()
{
    return "This is for testing!";
}