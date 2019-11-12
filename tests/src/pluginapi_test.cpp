#include "gtest/gtest.h"

#include "APluginLibrary/libraryloader.h"
#include "APluginSDK/plugininfos.h"

#include "../plugins/interface.h"

GTEST_TEST(PluginAPI_Test, memory_allocate_free)
{
    void* handle = apl::LibraryLoader::load("plugins/first/first_plugin");
    ASSERT_NE(handle, nullptr);

    auto allocateMemory = apl::LibraryLoader::getSymbol<void*(*)(size_t)>(handle, "allocatePluginMemory");
    ASSERT_NE(allocateMemory, nullptr);
    auto freeMemory = apl::LibraryLoader::getSymbol<void(*)(void*)>(handle, "freePluginMemory");
    ASSERT_NE(freeMemory, nullptr);

    void* ptr = allocateMemory(sizeof(apl::PluginClassInfo));
    ASSERT_NE(ptr, nullptr);
    auto classPtr = static_cast<apl::PluginClassInfo*>(ptr);

    classPtr->className = "TestClassName";
    classPtr->interfaceName = "TestInterfaceName";
    classPtr->createInstance = nullptr;
    classPtr->deleteInstance = nullptr;

    ASSERT_STREQ(classPtr->className, "TestClassName");
    ASSERT_STREQ(classPtr->interfaceName, "TestInterfaceName");
    ASSERT_EQ(classPtr->createInstance, nullptr);
    ASSERT_EQ(classPtr->deleteInstance, nullptr);

    freeMemory(classPtr);

    apl::LibraryLoader::unload(handle);
}

GTEST_TEST(PluginAPI_Test, feature_loading_single)
{
    void* handle = apl::LibraryLoader::load("plugins/first/first_plugin");
    ASSERT_NE(handle, nullptr);

    auto getFeatureCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginFeatureCount");
    ASSERT_NE(getFeatureCount, nullptr);
    auto getFeatureInfo = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*(*)(size_t)>(handle, "getPluginFeatureInfo");
    ASSERT_NE(getFeatureInfo, nullptr);
    auto getFeatureInfos = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*const*(*)()>(handle, "getPluginFeatureInfos");
    ASSERT_NE(getFeatureInfos, nullptr);

    ASSERT_EQ(getFeatureCount(), 1);

    const apl::PluginFeatureInfo* info = getFeatureInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::PluginFeatureInfo* const* infos = getFeatureInfos();
    ASSERT_NE(infos, nullptr);
    ASSERT_EQ(info, infos[0]);

    ASSERT_STREQ(info->returnType, "int");
    ASSERT_STREQ(info->featureGroup, "first_group1");
    ASSERT_STREQ(info->featureName, "feature1");
    ASSERT_STREQ(info->parameterList, "int x1, int x2");
    ASSERT_STREQ(info->parameterTypes, "int, int");
    ASSERT_STREQ(info->parameterNames, "x1, x2");
    ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(7, 3), 21);

    apl::LibraryLoader::unload(handle);
}

GTEST_TEST(PluginAPI_Test, feature_loading_multiple)
{
    void* handle = apl::LibraryLoader::load("plugins/second/second_plugin");
    ASSERT_NE(handle, nullptr);

    auto getFeatureCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginFeatureCount");
    ASSERT_NE(getFeatureCount, nullptr);
    auto getFeatureInfo = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*(*)(size_t)>(handle, "getPluginFeatureInfo");
    ASSERT_NE(getFeatureInfo, nullptr);
    auto getFeatureInfos = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*const*(*)()>(handle, "getPluginFeatureInfos");
    ASSERT_NE(getFeatureInfos, nullptr);

    ASSERT_EQ(getFeatureCount(), 6);

    const apl::PluginFeatureInfo* info;
    const apl::PluginFeatureInfo* const* infos = getFeatureInfos();
    ASSERT_NE(infos, nullptr);

    const char* featureNames[] = {"feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3"};
    int results[] = {12, 6, 27, 3, 49, 343};
    for(int i = 0; i < 6; i++) {
        info = getFeatureInfo(i);
        ASSERT_NE(info, nullptr);
        ASSERT_EQ(infos[i], info);
        ASSERT_STREQ(info->returnType, "int");
        ASSERT_STREQ(info->featureGroup, i >= 4 ? "second_group_pow" : "second_group_math");
        ASSERT_STREQ(info->featureName, featureNames[i]);
        ASSERT_STREQ(info->parameterList, i >= 4 ? "int x" : "int x1, int x2");
        ASSERT_STREQ(info->parameterTypes, i >= 4 ? "int" : "int, int");
        ASSERT_STREQ(info->parameterNames, i >= 4 ? "x" : "x1, x2");
        if(i >= 4)
            ASSERT_EQ(reinterpret_cast<int(*)(int)>(info->functionPointer)(7), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
    }

    apl::LibraryLoader::unload(handle);
}



GTEST_TEST(PluginAPI_Test, class_loading_single)
{
    void* handle = apl::LibraryLoader::load("plugins/third/third_plugin");
    ASSERT_NE(handle, nullptr);

    auto getClassCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginClassCount");
    ASSERT_NE(getClassCount, nullptr);
    auto getClassInfo = apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*(*)(size_t)>(handle, "getPluginClassInfo");
    ASSERT_NE(getClassInfo, nullptr);
    auto getClassInfos= apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*const*(*)()>(handle, "getPluginClassInfos");
    ASSERT_NE(getClassInfos, nullptr);

    ASSERT_EQ(getClassCount(), 1);

    const apl::PluginClassInfo* info = getClassInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::PluginClassInfo* const* infos = getClassInfos();
    ASSERT_NE(infos, nullptr);
    ASSERT_EQ(info, infos[0]);

    ASSERT_STREQ(info->interfaceName, "Interface");
    ASSERT_STREQ(info->className, "Implementation");
    ASSERT_NE(info->createInstance, nullptr);
    ASSERT_NE(info->deleteInstance, nullptr);

    auto createInstance = reinterpret_cast<Interface*(*)()>(info->createInstance);
    auto deleteInstance = reinterpret_cast<void(*)(Interface*)>(info->deleteInstance);
    ASSERT_NE(createInstance, nullptr);
    ASSERT_NE(deleteInstance, nullptr);

    Interface* interface = createInstance();
    ASSERT_NE(interface, nullptr);

    ASSERT_EQ(interface->function1(5, 7), 35);
    ASSERT_EQ(interface->function2(5), 25);

    deleteInstance(interface);

    apl::LibraryLoader::unload(handle);
}

GTEST_TEST(PluginAPI_Test, class_loading_multiple)
{
    void* handle = apl::LibraryLoader::load("plugins/fourth/fourth_plugin");
    ASSERT_NE(handle, nullptr);

    auto getClassCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginClassCount");
    ASSERT_NE(getClassCount, nullptr);
    auto getClassInfo = apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*(*)(size_t)>(handle, "getPluginClassInfo");
    ASSERT_NE(getClassInfo, nullptr);
    auto getClassInfos= apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*const*(*)()>(handle, "getPluginClassInfos");
    ASSERT_NE(getClassInfos, nullptr);

    ASSERT_EQ(getClassCount(), 3);

    const apl::PluginClassInfo* info;
    const apl::PluginClassInfo* const* infos = getClassInfos();
    ASSERT_NE(infos, nullptr);

    Interface* interface;
    int result1[] = {35, 12, -2};
    int result2[] = {25, 125, 625};
    for(size_t i = 0; i < 3; i++) {
        info = getClassInfo(i);
        ASSERT_NE(info, nullptr);
        ASSERT_EQ(infos[i], info);
        ASSERT_STREQ(info->interfaceName, "Interface");
        ASSERT_STREQ(info->className, std::string("Implementation").append(std::to_string(i)).c_str());
        ASSERT_NE(info->createInstance, nullptr);
        ASSERT_NE(info->deleteInstance, nullptr);

        auto createInstance = reinterpret_cast<Interface*(*)()>(info->createInstance);
        auto deleteInstance = reinterpret_cast<void(*)(Interface*)>(info->deleteInstance);
        ASSERT_NE(createInstance, nullptr);
        ASSERT_NE(deleteInstance, nullptr);

        interface = createInstance();
        ASSERT_NE(interface, nullptr);

        ASSERT_EQ(interface->function1(5, 7), result1[i]);
        ASSERT_EQ(interface->function2(5), result2[i]);

        deleteInstance(interface);
    }

    apl::LibraryLoader::unload(handle);
}



GTEST_TEST(PluginAPI_Test, feature_and_class_loading_single)
{
    void* handle = apl::LibraryLoader::load("plugins/fifth/fifth_plugin");
    ASSERT_NE(handle, nullptr);

    // load feature methods
    auto getFeatureCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginFeatureCount");
    ASSERT_NE(getFeatureCount, nullptr);
    auto getFeatureInfo = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*(*)(size_t)>(handle, "getPluginFeatureInfo");
    ASSERT_NE(getFeatureInfo, nullptr);
    auto getFeatureInfos = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*const*(*)()>(handle, "getPluginFeatureInfos");
    ASSERT_NE(getFeatureInfos, nullptr);

    // load class methods
    auto getClassCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginClassCount");
    ASSERT_NE(getClassCount, nullptr);
    auto getClassInfo = apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*(*)(size_t)>(handle, "getPluginClassInfo");
    ASSERT_NE(getClassInfo, nullptr);
    auto getClassInfos= apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*const*(*)()>(handle, "getPluginClassInfos");
    ASSERT_NE(getClassInfos, nullptr);

    ASSERT_EQ(getFeatureCount(), 1);
    ASSERT_EQ(getClassCount(), 1);

    // feature check
    const apl::PluginFeatureInfo* featureInfo = getFeatureInfo(0);
    ASSERT_NE(featureInfo, nullptr);
    const apl::PluginFeatureInfo* const* featureInfos = getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);
    ASSERT_EQ(featureInfo, featureInfos[0]);

    ASSERT_STREQ(featureInfo->returnType, "int");
    ASSERT_STREQ(featureInfo->featureGroup, "fifth_group1");
    ASSERT_STREQ(featureInfo->featureName, "feature1");
    ASSERT_STREQ(featureInfo->parameterList, "int x1, int x2");
    ASSERT_STREQ(featureInfo->parameterTypes, "int, int");
    ASSERT_STREQ(featureInfo->parameterNames, "x1, x2");
    ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(featureInfo->functionPointer)(7, 3), 21);

    // class check
    const apl::PluginClassInfo* classInfo = getClassInfo(0);
    ASSERT_NE(classInfo, nullptr);
    const apl::PluginClassInfo* const* classInfos = getClassInfos();
    ASSERT_NE(classInfos, nullptr);
    ASSERT_EQ(classInfo, classInfos[0]);

    ASSERT_STREQ(classInfo->interfaceName, "Interface");
    ASSERT_STREQ(classInfo->className, "Implementation");
    ASSERT_NE(classInfo->createInstance, nullptr);
    ASSERT_NE(classInfo->deleteInstance, nullptr);

    auto createInstance = reinterpret_cast<Interface*(*)()>(classInfo->createInstance);
    auto deleteInstance = reinterpret_cast<void(*)(Interface*)>(classInfo->deleteInstance);
    ASSERT_NE(createInstance, nullptr);
    ASSERT_NE(deleteInstance, nullptr);

    Interface* interface = createInstance();
    ASSERT_NE(interface, nullptr);

    ASSERT_EQ(interface->function1(5, 7), 35);
    ASSERT_EQ(interface->function2(5), 25);

    deleteInstance(interface);

    apl::LibraryLoader::unload(handle);
}

GTEST_TEST(PluginAPI_Test, feature_and_class_loading_multiple)
{
    void* handle = apl::LibraryLoader::load("plugins/sixth/sixth_plugin");
    ASSERT_NE(handle, nullptr);

    // load feature methods
    auto getFeatureCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginFeatureCount");
    ASSERT_NE(getFeatureCount, nullptr);
    auto getFeatureInfo = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*(*)(size_t)>(handle, "getPluginFeatureInfo");
    ASSERT_NE(getFeatureInfo, nullptr);
    auto getFeatureInfos = apl::LibraryLoader::getSymbol<const apl::PluginFeatureInfo*const*(*)()>(handle, "getPluginFeatureInfos");
    ASSERT_NE(getFeatureInfos, nullptr);

    // load class methods
    auto getClassCount = apl::LibraryLoader::getSymbol<size_t(*)()>(handle, "getPluginClassCount");
    ASSERT_NE(getClassCount, nullptr);
    auto getClassInfo = apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*(*)(size_t)>(handle, "getPluginClassInfo");
    ASSERT_NE(getClassInfo, nullptr);
    auto getClassInfos= apl::LibraryLoader::getSymbol<const apl::PluginClassInfo*const*(*)()>(handle, "getPluginClassInfos");
    ASSERT_NE(getClassInfos, nullptr);

    ASSERT_EQ(getFeatureCount(), 6);
    ASSERT_EQ(getClassCount(), 3);

    // feature check
    const apl::PluginFeatureInfo* featureInfo;
    const apl::PluginFeatureInfo* const* featureInfos = getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);

    const char* featureNames[] = {"feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3"};
    int results[] = {12, 6, 27, 3, 49, 343};
    for(int i = 0; i < 6; i++) {
        featureInfo = getFeatureInfo(i);
        ASSERT_EQ(featureInfos[i], featureInfo);
        ASSERT_TRUE(featureInfo);
        ASSERT_STREQ(featureInfo->returnType, "int");
        ASSERT_STREQ(featureInfo->featureGroup, i >= 4 ? "sixth_group_pow" : "sixth_group_math");
        ASSERT_STREQ(featureInfo->featureName, featureNames[i]);
        ASSERT_STREQ(featureInfo->parameterList, i >= 4 ? "int x" : "int x1, int x2");
        ASSERT_STREQ(featureInfo->parameterTypes, i >= 4 ? "int" : "int, int");
        ASSERT_STREQ(featureInfo->parameterNames, i >= 4 ? "x" : "x1, x2");
        if(i >= 4)
            ASSERT_EQ(reinterpret_cast<int(*)(int)>(featureInfo->functionPointer)(7), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(featureInfo->functionPointer)(9, 3), results[i]);
    }

    // class check
    const apl::PluginClassInfo* classInfo;
    const apl::PluginClassInfo* const* classInfos = getClassInfos();
    ASSERT_NE(classInfos, nullptr);

    Interface* interface;
    int result1[] = {35, 12, -2};
    int result2[] = {25, 125, 625};
    for(size_t i = 0; i < 3; i++) {
        classInfo = getClassInfo(i);
        ASSERT_NE(classInfo, nullptr);
        ASSERT_EQ(classInfos[i], classInfo);
        ASSERT_STREQ(classInfo->interfaceName, "Interface");
        ASSERT_STREQ(classInfo->className, std::string("Implementation").append(std::to_string(i)).c_str());
        ASSERT_NE(classInfo->createInstance, nullptr);
        ASSERT_NE(classInfo->deleteInstance, nullptr);

        auto createInstance = reinterpret_cast<Interface*(*)()>(classInfo->createInstance);
        auto deleteInstance = reinterpret_cast<void(*)(Interface*)>(classInfo->deleteInstance);
        ASSERT_NE(createInstance, nullptr);
        ASSERT_NE(deleteInstance, nullptr);

        interface = createInstance();
        ASSERT_NE(interface, nullptr);

        ASSERT_EQ(interface->function1(5, 7), result1[i]);
        ASSERT_EQ(interface->function2(5), result2[i]);

        deleteInstance(interface);
    }

    apl::LibraryLoader::unload(handle);
}
