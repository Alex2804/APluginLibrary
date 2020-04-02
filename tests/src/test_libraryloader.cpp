#include "gtest/gtest.h"

#include <string>

#include "APluginLibrary/libraryloader.h"

typedef double(*addFunc)(double,double);
typedef double(*subFunc)(double,double);
typedef char*(*setToCharFunc)(char*,char,int);
typedef void*(*allocateFunc)(size_t);
typedef void(*releaseFunc)(void*);

GTEST_TEST(Test_LibraryLoader, load_getSymbol_unload_single)
{
	void* handle = apl::LibraryLoader::load("libraries/first/first_lib");
	ASSERT_NE(handle, nullptr);

	addFunc add = reinterpret_cast<addFunc>(apl::LibraryLoader::getSymbol(handle, "add"));
	subFunc sub = reinterpret_cast<subFunc>(apl::LibraryLoader::getSymbol(handle, "sub"));
	auto setToChar = apl::LibraryLoader::getSymbol<setToCharFunc>(handle, "setToChar");
	addFunc addImg = reinterpret_cast<addFunc>(apl::LibraryLoader::getSymbol(handle, "addImg"));
	subFunc subImg = reinterpret_cast<subFunc>(apl::LibraryLoader::getSymbol(handle, "subImg"));
	auto setToCharImg = apl::LibraryLoader::getSymbol<setToCharFunc>(handle, "setToCharImg");

	ASSERT_NE(add, nullptr);
	ASSERT_NE(sub, nullptr);
	ASSERT_NE(setToChar, nullptr);
	ASSERT_EQ(addImg, nullptr);
	ASSERT_EQ(subImg, nullptr);
	ASSERT_EQ(setToCharImg, nullptr);

	ASSERT_EQ(add(12.5, 15), 27.5);
	ASSERT_EQ(sub(15, 12.5), 2.5);
	char* c = static_cast<char*>(std::malloc(5 * sizeof(char)));
	memcpy(c, std::string("abcx").c_str(), 5 * sizeof(char));
	ASSERT_EQ(std::string(setToChar(c, '6', 3)), std::string("666x"));
	std::free(c);

	ASSERT_TRUE(apl::LibraryLoader::unload(handle));
}

GTEST_TEST(Test_LibraryLoader, load_getSymbol_unload_mutiple)
{
    void* imaginary1 = apl::LibraryLoader::load("libraries/invalid/imaginary1/imaginary1_lib");
    void* first = apl::LibraryLoader::load("libraries/first/first_lib");
    void* imaginary2 = apl::LibraryLoader::load("libraries/invalid/imaginary2/imaginary2_lib");
    void* second = apl::LibraryLoader::load("libraries/second/second_lib");

    ASSERT_EQ(imaginary1, nullptr);
    ASSERT_NE(first, nullptr);
    ASSERT_EQ(imaginary2, nullptr);
    ASSERT_NE(second, nullptr);

    addFunc add = reinterpret_cast<addFunc>(apl::LibraryLoader::getSymbol(first, "add"));
    subFunc sub = reinterpret_cast<subFunc>(apl::LibraryLoader::getSymbol(first, "sub"));
    auto setToChar = apl::LibraryLoader::getSymbol<setToCharFunc>(first, "setToChar");
    addFunc addImg = reinterpret_cast<addFunc>(apl::LibraryLoader::getSymbol(first, "addImg"));
    subFunc subImg = reinterpret_cast<subFunc>(apl::LibraryLoader::getSymbol(first, "subImg"));
    auto setToCharImg = apl::LibraryLoader::getSymbol<setToCharFunc>(first, "setToCharImg");

    ASSERT_NE(add, nullptr);
    ASSERT_NE(sub, nullptr);
    ASSERT_NE(setToChar, nullptr);
    ASSERT_EQ(addImg, nullptr);
    ASSERT_EQ(subImg, nullptr);
    ASSERT_EQ(setToCharImg, nullptr);

    ASSERT_EQ(add(12.5, 15), 27.5);
    ASSERT_EQ(sub(15, 12.5), 2.5);
    char* c = static_cast<char*>(std::malloc(5 * sizeof(char)));
    memcpy(c, std::string("abcx").c_str(), 5 * sizeof(char));
    ASSERT_EQ(std::string(setToChar(c, '6', 3)), std::string("666x"));
    std::free(c);

    allocateFunc allocate = reinterpret_cast<allocateFunc>(apl::LibraryLoader::getSymbol(second, "allocate"));
    auto release = apl::LibraryLoader::getSymbol<releaseFunc>(second, "release");
    allocateFunc allocateImg = reinterpret_cast<allocateFunc>(apl::LibraryLoader::getSymbol(second, "allocateImg"));
    auto releaseImg = apl::LibraryLoader::getSymbol<releaseFunc>(second, "releaseImg");

    ASSERT_NE(allocate, nullptr);
    ASSERT_NE(release, nullptr);
    ASSERT_EQ(allocateImg, nullptr);
    ASSERT_EQ(releaseImg, nullptr);

    void* ptr = allocate(sizeof(char));
    ASSERT_NE(ptr, nullptr);
    release(ptr);

    void* img1 = apl::LibraryLoader::getSymbol(imaginary1, "add");
    void* img2 = apl::LibraryLoader::getSymbol(imaginary1, "sub");
    void* img3 = apl::LibraryLoader::getSymbol(imaginary2, "allocate");
    void* img4 = apl::LibraryLoader::getSymbol(imaginary2, "release");

    ASSERT_EQ(img1, nullptr);
    ASSERT_EQ(img2, nullptr);
    ASSERT_EQ(img3, nullptr);
    ASSERT_EQ(img4, nullptr);

    ASSERT_TRUE(apl::LibraryLoader::unload(imaginary1));
    ASSERT_TRUE(apl::LibraryLoader::unload(first));
    ASSERT_TRUE(apl::LibraryLoader::unload(imaginary2));
    ASSERT_TRUE(apl::LibraryLoader::unload(second));
}

GTEST_TEST(Test_LibraryLoader, error_handling)
{
    apl::LibraryLoader::clearError();
    ASSERT_EQ(apl::LibraryLoader::getError(), nullptr);
    apl::LibraryLoader::load("libraries/invalid/imaginary1/imaginary1_lib");
    ASSERT_NE(apl::LibraryLoader::getError(), nullptr);
    apl::LibraryLoader::clearError();
    ASSERT_EQ(apl::LibraryLoader::getError(), nullptr);
    void* first = apl::LibraryLoader::load("libraries/first/first_lib");
    ASSERT_EQ(apl::LibraryLoader::getError(), nullptr);

    apl::LibraryLoader::getSymbol<setToCharFunc>(first, "setToChar");
    ASSERT_EQ(apl::LibraryLoader::getError(), nullptr);
    apl::LibraryLoader::getSymbol<setToCharFunc>(first, "setToCharImg");
    ASSERT_NE(apl::LibraryLoader::getError(), nullptr);
    apl::LibraryLoader::clearError();
    ASSERT_EQ(apl::LibraryLoader::getError(), nullptr);

}
