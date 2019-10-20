#include "gtest/gtest.h"

#include <string>

#include "APluginLibrary/alibraryloader.h"

typedef double(*addFunc)(double,double);
typedef double(*subFunc)(double,double);
typedef char*(*setToCharFunc)(char*,char,int);
typedef void*(*allocateFunc)(size_t);
typedef void(*releaseFunc)(void*);

GTEST_TEST(ALibraryLoader_Test, load_getSymbol_unload_single)
{
	void* handle = apl::ALibraryLoader::load("libraries/first/first_lib");
	ASSERT_NE(handle, nullptr);

	addFunc add = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(handle, "add"));
	subFunc sub = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(handle, "sub"));
	auto setToChar = apl::ALibraryLoader::getSymbol<setToCharFunc>(handle, "setToChar");
	addFunc addImg = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(handle, "addImg"));
	subFunc subImg = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(handle, "subImg"));
	auto setToCharImg = apl::ALibraryLoader::getSymbol<setToCharFunc>(handle, "setToCharImg");

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

	ASSERT_TRUE(apl::ALibraryLoader::unload(handle));
}

GTEST_TEST(ALibraryLoader_Test, load_getSymbol_unload_mutiple)
{
    void* imaginary1 = apl::ALibraryLoader::load("libraries/invalid/imaginary1/imaginary1_lib");
    void* first = apl::ALibraryLoader::load("libraries/first/first_lib");
    void* imaginary2 = apl::ALibraryLoader::load("libraries/invalid/imaginary2/imaginary2_lib");
    void* second = apl::ALibraryLoader::load("libraries/second/second_lib");

    ASSERT_EQ(imaginary1, nullptr);
    ASSERT_NE(first, nullptr);
    ASSERT_EQ(imaginary2, nullptr);
    ASSERT_NE(second, nullptr);

    addFunc add = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(first, "add"));
    subFunc sub = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(first, "sub"));
    auto setToChar = apl::ALibraryLoader::getSymbol<setToCharFunc>(first, "setToChar");
    addFunc addImg = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(first, "addImg"));
    subFunc subImg = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(first, "subImg"));
    auto setToCharImg = apl::ALibraryLoader::getSymbol<setToCharFunc>(first, "setToCharImg");

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

    allocateFunc allocate = reinterpret_cast<allocateFunc>(apl::ALibraryLoader::getSymbol(second, "allocate"));
    auto release = apl::ALibraryLoader::getSymbol<releaseFunc>(second, "release");
    allocateFunc allocateImg = reinterpret_cast<allocateFunc>(apl::ALibraryLoader::getSymbol(second, "allocateImg"));
    auto releaseImg = apl::ALibraryLoader::getSymbol<releaseFunc>(second, "releaseImg");

    ASSERT_NE(allocate, nullptr);
    ASSERT_NE(release, nullptr);
    ASSERT_EQ(allocateImg, nullptr);
    ASSERT_EQ(releaseImg, nullptr);

    void* ptr = allocate(sizeof(char));
    ASSERT_NE(ptr, nullptr);
    release(ptr);

    void* img1 = apl::ALibraryLoader::getSymbol(imaginary1, "add");
    void* img2 = apl::ALibraryLoader::getSymbol(imaginary1, "sub");
    void* img3 = apl::ALibraryLoader::getSymbol(imaginary2, "allocate");
    void* img4 = apl::ALibraryLoader::getSymbol(imaginary2, "release");

    ASSERT_EQ(img1, nullptr);
    ASSERT_EQ(img2, nullptr);
    ASSERT_EQ(img3, nullptr);
    ASSERT_EQ(img4, nullptr);

    ASSERT_TRUE(apl::ALibraryLoader::unload(imaginary1));
    ASSERT_TRUE(apl::ALibraryLoader::unload(first));
    ASSERT_TRUE(apl::ALibraryLoader::unload(imaginary2));
    ASSERT_TRUE(apl::ALibraryLoader::unload(second));
}