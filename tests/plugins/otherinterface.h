#ifndef APLUGINLIBRARY_TESTS_OTHERINTERFACE_H
#define APLUGINLIBRARY_TESTS_OTHERINTERFACE_H

class OtherInterface
{
public:
    OtherInterface() = default;
    virtual ~OtherInterface() = default;

    virtual int otherFunction1(int x1, int x2, int x3) = 0;
    virtual const char* otherFunction2() = 0;
    virtual int otherFunction3(double x) = 0;
};

#endif //APLUGINLIBRARY_TESTS_OTHERINTERFACE_H
