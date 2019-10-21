#ifndef APLUGINLIBRARY_TESTS_INTERFACES_H
#define APLUGINLIBRARY_TESTS_INTERFACES_H

class Interface
{
public:
    Interface() = default;
    virtual ~Interface() = default;

    virtual int function1(int x1, int x2) = 0;
    virtual int function2(int x) = 0;
};

class OtherInterface
{
public:
    OtherInterface() = default;
    virtual ~OtherInterface() = default;

    virtual int otherFunction1(int x1, int x2, int x3) = 0;
    virtual const char* otherFunction2() = 0;
};

#endif //APLUGINLIBRARY_TESTS_INTERFACES_H
