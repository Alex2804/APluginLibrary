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
	ASSERT_TRUE(handle);

	addFunc add = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(handle, "add"));
	subFunc sub = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(handle, "sub"));
	auto setToChar = apl::ALibraryLoader::getSymbol<setToCharFunc>(handle, "setToChar");
	addFunc addImg = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(handle, "addImg"));
	subFunc subImg = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(handle, "subImg"));
	auto setToCharImg = apl::ALibraryLoader::getSymbol<setToCharFunc>(handle, "setToCharImg");

	ASSERT_TRUE(add);
	ASSERT_TRUE(sub);
	ASSERT_TRUE(setToChar);
	ASSERT_FALSE(addImg);
	ASSERT_FALSE(subImg);
	ASSERT_FALSE(setToCharImg);

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

    ASSERT_FALSE(imaginary1);
    ASSERT_TRUE(first);
    ASSERT_FALSE(imaginary2);
    ASSERT_TRUE(second);

    addFunc add = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(first, "add"));
    subFunc sub = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(first, "sub"));
    auto setToChar = apl::ALibraryLoader::getSymbol<setToCharFunc>(first, "setToChar");
    addFunc addImg = reinterpret_cast<addFunc>(apl::ALibraryLoader::getSymbol(first, "addImg"));
    subFunc subImg = reinterpret_cast<subFunc>(apl::ALibraryLoader::getSymbol(first, "subImg"));
    auto setToCharImg = apl::ALibraryLoader::getSymbol<setToCharFunc>(first, "setToCharImg");

    ASSERT_TRUE(add);
    ASSERT_TRUE(sub);
    ASSERT_TRUE(setToChar);
    ASSERT_FALSE(addImg);
    ASSERT_FALSE(subImg);
    ASSERT_FALSE(setToCharImg);

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

    ASSERT_TRUE(allocate);
    ASSERT_TRUE(release);
    ASSERT_FALSE(allocateImg);
    ASSERT_FALSE(releaseImg);

    void* ptr = allocate(sizeof(char));
    ASSERT_TRUE(ptr);
    release(ptr);

    void* img1 = apl::ALibraryLoader::getSymbol(imaginary1, "add");
    void* img2 = apl::ALibraryLoader::getSymbol(imaginary1, "sub");
    void* img3 = apl::ALibraryLoader::getSymbol(imaginary2, "allocate");
    void* img4 = apl::ALibraryLoader::getSymbol(imaginary2, "release");

    ASSERT_FALSE(img1);
    ASSERT_FALSE(img2);
    ASSERT_FALSE(img3);
    ASSERT_FALSE(img4);

    ASSERT_TRUE(apl::ALibraryLoader::unload(imaginary1));
    ASSERT_TRUE(apl::ALibraryLoader::unload(first));
    ASSERT_TRUE(apl::ALibraryLoader::unload(imaginary2));
    ASSERT_TRUE(apl::ALibraryLoader::unload(second));
}