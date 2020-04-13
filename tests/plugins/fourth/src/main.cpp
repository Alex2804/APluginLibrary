#include "APluginSDK/pluginapi.h"
#include "../../interface.h"

class APLUGINSDK_NO_EXPORT Implementation0 : public Interface
{
public:
    int function1(int x1, int x2) override;
    int function2(int x) override;
};

A_PLUGIN_REGISTER_CLASS(Interface, Implementation0);

int Implementation0::function1(int x1, int x2) {
    return x1 * x2;
}
int Implementation0::function2(int x) {
    return x * x;
}


class APLUGINSDK_NO_EXPORT Implementation1 : public Interface
{
public:
    int function1(int x1, int x2) override;
    int function2(int x) override;
};

A_PLUGIN_REGISTER_CLASS(Interface, Implementation1);

int Implementation1::function1(int x1, int x2) {
    return x1 + x2;
}
int Implementation1::function2(int x) {
    return x * x * x;
}


class APLUGINSDK_NO_EXPORT Implementation2 : public Interface
{
public:
    int function1(int x1, int x2) override;
    int function2(int x) override;
};

A_PLUGIN_REGISTER_CLASS(Interface, Implementation2);

int Implementation2::function1(int x1, int x2) {
    return x1 - x2;
}
int Implementation2::function2(int x) {
    return x * x * x * x;
}