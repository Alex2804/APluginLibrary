#include "APluginSDK/pluginapi.h"
#include "../../interface.h"

A_PLUGIN_REGISTER_FEATURE(int, sixth_group_math, feature_add, int x1, int x2)
{
    return x1 + x2;
}
A_PLUGIN_REGISTER_FEATURE(int, sixth_group_math, feature_sub, int x1, int x2)
{
    return x1 - x2;
}
A_PLUGIN_REGISTER_FEATURE(int, sixth_group_math, feature_mul, int x1, int x2)
{
    return x1 * x2;
}
A_PLUGIN_REGISTER_FEATURE(int, sixth_group_math, feature_div, int x1, int x2)
{
    return x1 / x2;
}

A_PLUGIN_REGISTER_FEATURE(int, sixth_group_pow, feature_pow2, int x)
{
    return x * x;
}
A_PLUGIN_REGISTER_FEATURE(int, sixth_group_pow, feature_pow3, int x)
{
    return x * x * x;
}



class Implementation0 : public Interface
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


class Implementation1 : public Interface
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


class Implementation2 : public Interface
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