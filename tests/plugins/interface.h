#ifndef APLUGINLIBRARY_TESTS_INTERFACE_H
#define APLUGINLIBRARY_TESTS_INTERFACE_H

class Interface
{
public:
    Interface() = default;
    virtual ~Interface() = default;

    virtual int function1(int, int) = 0;
    virtual int function2(int) = 0;
};

#endif //APLUGINLIBRARY_TESTS_INTERFACE_H
