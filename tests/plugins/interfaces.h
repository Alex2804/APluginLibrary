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

#endif //APLUGINLIBRARY_TESTS_INTERFACES_H
