#include "../../../../include/APluginLibrary/apluginapi.h"
#include "../../interfaces.h"

class Implementation : public Interface
{
public:
    int function1(int x1, int x2) override;
    int function2(int x) override;
};

A_PLUGIN_CLASS_REGISTRATION(Interface, Implementation);

int Implementation::function1(int x1, int x2) {
    return x1 * x2;
}
int Implementation::function2(int x) {
    return x * x;
}
