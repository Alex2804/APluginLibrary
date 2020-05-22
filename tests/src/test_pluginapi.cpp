#include "gtest/gtest.h"

#include "APluginLibrary/libraryloader.h"

#include "APluginSDK/pluginapi.h"
#include "APluginSDK/private/privateplugininfos.h"

#include "../plugins/interface.h"
#include "../plugins/include.h"

GTEST_TEST(Test_PluginAPI, initAPlugin)
{
    void* handle1 = apl::LibraryLoader::load("plugins/first/first_plugin");
    ASSERT_NE(handle1, nullptr);
    void* handle2 = apl::LibraryLoader::load("plugins/second/second_plugin");
    ASSERT_NE(handle2, nullptr);

    auto getAPluginInfo1 = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle1, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo1, nullptr);
    auto initAPlugin1 = apl::LibraryLoader::getSymbol<void(*)()>(handle1, "APluginSDK_initPlugin");
    ASSERT_NE(initAPlugin1, nullptr);
    auto finiAPlugin1 = apl::LibraryLoader::getSymbol<void(*)()>(handle1, "APluginSDK_finiPlugin");
    ASSERT_NE(finiAPlugin1, nullptr);

    auto getAPluginInfo2 = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle2, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo2, nullptr);
    ASSERT_EQ(apl::LibraryLoader::getSymbol<void(*)()>(handle2, "APluginSDK_initPlugin"), nullptr);
    ASSERT_EQ(apl::LibraryLoader::getSymbol<void(*)()>(handle2, "APluginSDK_finiPlugin"), nullptr);

    const apl::APluginInfo* info1 = getAPluginInfo1();
    ASSERT_NE(info1, nullptr);
    info1->privateInfo->constructPluginInternals(initAPlugin1);

    ASSERT_STREQ(info1->pluginName, "first_plugin");
    ASSERT_EQ(info1->pluginVersionMajor, 9);
    ASSERT_EQ(info1->pluginVersionMinor, 87);
    ASSERT_EQ(info1->pluginVersionPatch, 789);
    ASSERT_EQ(info1->apiVersionMajor, APLUGINSDK_API_VERSION_MAJOR);
    ASSERT_EQ(info1->apiVersionMinor, APLUGINSDK_API_VERSION_MINOR);
    ASSERT_EQ(info1->apiVersionPatch, APLUGINSDK_API_VERSION_PATCH);
    ASSERT_NE(info1->allocateMemory, nullptr);
    ASSERT_NE(info1->freeMemory, nullptr);
    ASSERT_NE(info1->getFeatureCount, nullptr);
    ASSERT_NE(info1->getFeatureInfo, nullptr);
    ASSERT_NE(info1->getFeatureInfos, nullptr);
    ASSERT_NE(info1->getClassCount, nullptr);
    ASSERT_NE(info1->getClassInfo, nullptr);
    ASSERT_NE(info1->getClassInfos, nullptr);

    const apl::APluginInfo* info2 = getAPluginInfo2();
    ASSERT_NE(info2, nullptr);
    info2->privateInfo->constructPluginInternals(nullptr);

    ASSERT_STREQ(info2->pluginName, "second_plugin");
    ASSERT_EQ(info2->pluginVersionMajor, 3);
    ASSERT_EQ(info2->pluginVersionMinor, 5);
    ASSERT_EQ(info2->pluginVersionPatch, 12);
    ASSERT_EQ(info2->apiVersionMajor, APLUGINSDK_API_VERSION_MAJOR);
    ASSERT_EQ(info2->apiVersionMinor, APLUGINSDK_API_VERSION_MINOR);
    ASSERT_EQ(info2->apiVersionPatch, APLUGINSDK_API_VERSION_PATCH);
    ASSERT_NE(info2->allocateMemory, nullptr);
    ASSERT_NE(info2->freeMemory, nullptr);
    ASSERT_NE(info2->getFeatureCount, nullptr);
    ASSERT_NE(info2->getFeatureInfo, nullptr);
    ASSERT_NE(info2->getFeatureInfos, nullptr);
    ASSERT_NE(info2->getClassCount, nullptr);
    ASSERT_NE(info2->getClassInfo, nullptr);
    ASSERT_NE(info2->getClassInfos, nullptr);

    info1->privateInfo->destructPluginInternals(finiAPlugin1);
    apl::LibraryLoader::unload(handle1);
    info2->privateInfo->destructPluginInternals(nullptr);
    apl::LibraryLoader::unload(handle2);
}

GTEST_TEST(Test_PluginAPI, feature_loading_single)
{
    void* handle = apl::LibraryLoader::load("plugins/first/first_plugin");
    ASSERT_NE(handle, nullptr);
    auto getAPluginInfo = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo, nullptr);
    auto initAPluginInfo = apl::LibraryLoader::getSymbol<void(*)()>(handle, "APluginSDK_initPlugin");
    ASSERT_NE(initAPluginInfo, nullptr);
    auto finiAPluginInfo = apl::LibraryLoader::getSymbol<void(*)()>(handle, "APluginSDK_finiPlugin");
    ASSERT_NE(finiAPluginInfo, nullptr);

    auto pluginInfo = getAPluginInfo();
    ASSERT_NE(pluginInfo, nullptr);
    pluginInfo->privateInfo->constructPluginInternals(initAPluginInfo);

    ASSERT_NE(pluginInfo->getFeatureCount, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfo, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfos, nullptr);

    ASSERT_EQ(pluginInfo->getFeatureCount(), 2);

    const apl::APluginFeatureInfo* info = pluginInfo->getFeatureInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::APluginFeatureInfo* const* infos = pluginInfo->getFeatureInfos();
    ASSERT_NE(infos, nullptr);
    ASSERT_EQ(info, infos[0]);

    ASSERT_STREQ(info->returnType, "int");
    ASSERT_STREQ(info->featureGroup, "first_group1");
    ASSERT_STREQ(info->featureName, "feature1");
    ASSERT_STREQ(info->parameterList, "int x1, int x2");
    ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(7, 3), 21);

    info = pluginInfo->getFeatureInfo(1);
    ASSERT_NE(info, nullptr);
    ASSERT_EQ(info, infos[1]);

    ASSERT_STREQ(info->returnType, "struct APluginLibrary_Test_PointStruct");
    ASSERT_STREQ(info->featureGroup, "first_group1");
    ASSERT_STREQ(info->featureName, "feature2");
    ASSERT_STREQ(info->parameterList, "int y, int x");
    afl::APluginLibrary_Test_PointStruct APluginLibrary_Test_PointStruct = reinterpret_cast<afl::APluginLibrary_Test_PointStruct(*)(int, int)>(info->functionPointer)(7, 3);
    ASSERT_EQ(APluginLibrary_Test_PointStruct.x, 3);
    ASSERT_EQ(APluginLibrary_Test_PointStruct.y, 7);

    pluginInfo->privateInfo->destructPluginInternals(finiAPluginInfo);
    apl::LibraryLoader::unload(handle);
}

GTEST_TEST(Test_PluginAPI, feature_loading_multiple)
{
    void* handle = apl::LibraryLoader::load("plugins/second/second_plugin");
    ASSERT_NE(handle, nullptr);
    auto getAPluginInfo = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo, nullptr);
    auto pluginInfo = getAPluginInfo();
    ASSERT_NE(pluginInfo, nullptr);
    pluginInfo->privateInfo->constructPluginInternals(nullptr);

    ASSERT_NE(pluginInfo->getFeatureCount, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfo, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfos, nullptr);

    ASSERT_EQ(pluginInfo->getFeatureCount(), 6);

    const apl::APluginFeatureInfo* info;
    const apl::APluginFeatureInfo* const* infos = pluginInfo->getFeatureInfos();
    ASSERT_NE(infos, nullptr);

    const char* featureNames[] = {"feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3"};
    int results[] = {12, 6, 27, 3, 49, 343};
    for(int i = 0; i < 6; i++) {
        info = pluginInfo->getFeatureInfo(i);
        ASSERT_NE(info, nullptr);
        ASSERT_EQ(infos[i], info);
        ASSERT_STREQ(info->returnType, "int");
        ASSERT_STREQ(info->featureGroup, i >= 4 ? "second_group_pow" : "second_group_math");
        ASSERT_STREQ(info->featureName, featureNames[i]);
        ASSERT_STREQ(info->parameterList, i >= 4 ? "int x" : "int x1, int x2");
        if(i >= 4)
            ASSERT_EQ(reinterpret_cast<int(*)(int)>(info->functionPointer)(7), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
    }

    pluginInfo->privateInfo->destructPluginInternals(nullptr);
    apl::LibraryLoader::unload(handle);
}



GTEST_TEST(Test_PluginAPI, class_loading_single)
{
    void* handle = apl::LibraryLoader::load("plugins/third/third_plugin");
    ASSERT_NE(handle, nullptr);
    auto getAPluginInfo = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo, nullptr);
    auto pluginInfo = getAPluginInfo();
    ASSERT_NE(pluginInfo, nullptr);
    pluginInfo->privateInfo->constructPluginInternals(nullptr);

    ASSERT_NE(pluginInfo->getClassCount, nullptr);
    ASSERT_NE(pluginInfo->getClassInfo, nullptr);
    ASSERT_NE(pluginInfo->getClassInfos, nullptr);

    ASSERT_EQ(pluginInfo->getClassCount(), 1);

    const apl::APluginClassInfo* info = pluginInfo->getClassInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::APluginClassInfo* const* infos = pluginInfo->getClassInfos();
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

    pluginInfo->privateInfo->destructPluginInternals(nullptr);
    apl::LibraryLoader::unload(handle);
}

GTEST_TEST(Test_PluginAPI, class_loading_multiple)
{
    void* handle = apl::LibraryLoader::load("plugins/fourth/fourth_plugin");
    ASSERT_NE(handle, nullptr);
    auto getAPluginInfo = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo, nullptr);
    auto pluginInfo = getAPluginInfo();
    ASSERT_NE(pluginInfo, nullptr);
    pluginInfo->privateInfo->constructPluginInternals(nullptr);

    ASSERT_NE(pluginInfo->getClassCount, nullptr);
    ASSERT_NE(pluginInfo->getClassInfo, nullptr);
    ASSERT_NE(pluginInfo->getClassInfos, nullptr);

    ASSERT_EQ(pluginInfo->getClassCount(), 3);

    const apl::APluginClassInfo* info;
    const apl::APluginClassInfo* const* infos = pluginInfo->getClassInfos();
    ASSERT_NE(infos, nullptr);

    Interface* interface;
    int result1[] = {35, 12, -2};
    int result2[] = {25, 125, 625};
    for(size_t i = 0; i < 3; i++) {
        info = pluginInfo->getClassInfo(i);
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

    pluginInfo->privateInfo->destructPluginInternals(nullptr);
    apl::LibraryLoader::unload(handle);
}



GTEST_TEST(Test_PluginAPI, feature_and_class_loading_single)
{
    void* handle = apl::LibraryLoader::load("plugins/fifth/fifth_plugin");
    ASSERT_NE(handle, nullptr);
    auto getAPluginInfo = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo, nullptr);
    auto pluginInfo = getAPluginInfo();
    ASSERT_NE(pluginInfo, nullptr);
    pluginInfo->privateInfo->constructPluginInternals(nullptr);

    ASSERT_NE(pluginInfo->getFeatureCount, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfo, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfos, nullptr);

    ASSERT_NE(pluginInfo->getClassCount, nullptr);
    ASSERT_NE(pluginInfo->getClassInfo, nullptr);
    ASSERT_NE(pluginInfo->getClassInfos, nullptr);

    ASSERT_EQ(pluginInfo->getFeatureCount(), 1);
    ASSERT_EQ(pluginInfo->getClassCount(), 1);

    // feature check
    const apl::APluginFeatureInfo* featureInfo = pluginInfo->getFeatureInfo(0);
    ASSERT_NE(featureInfo, nullptr);
    const apl::APluginFeatureInfo* const* featureInfos = pluginInfo->getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);
    ASSERT_EQ(featureInfo, featureInfos[0]);

    ASSERT_STREQ(featureInfo->returnType, "int");
    ASSERT_STREQ(featureInfo->featureGroup, "fifth_group1");
    ASSERT_STREQ(featureInfo->featureName, "feature1");
    ASSERT_STREQ(featureInfo->parameterList, "");
    ASSERT_EQ(reinterpret_cast<int(*)()>(featureInfo->functionPointer)(), 6);

    // class check
    const apl::APluginClassInfo* classInfo = pluginInfo->getClassInfo(0);
    ASSERT_NE(classInfo, nullptr);
    const apl::APluginClassInfo* const* classInfos = pluginInfo->getClassInfos();
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

    pluginInfo->privateInfo->destructPluginInternals(nullptr);
    apl::LibraryLoader::unload(handle);
}

GTEST_TEST(Test_PluginAPI, feature_and_class_loading_multiple)
{
    void* handle = apl::LibraryLoader::load("plugins/sixth/sixth_plugin");
    ASSERT_NE(handle, nullptr);
    auto getAPluginInfo = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo, nullptr);
    auto pluginInfo = getAPluginInfo();
    ASSERT_NE(pluginInfo, nullptr);
    pluginInfo->privateInfo->constructPluginInternals(nullptr);

    ASSERT_NE(pluginInfo->getFeatureCount, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfo, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfos, nullptr);

    ASSERT_NE(pluginInfo->getClassCount, nullptr);
    ASSERT_NE(pluginInfo->getClassInfo, nullptr);
    ASSERT_NE(pluginInfo->getClassInfos, nullptr);

    ASSERT_EQ(pluginInfo->getFeatureCount(), 6);
    ASSERT_EQ(pluginInfo->getClassCount(), 3);

    // feature check
    const apl::APluginFeatureInfo* featureInfo;
    const apl::APluginFeatureInfo* const* featureInfos = pluginInfo->getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);

    const char* featureNames[] = {"feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3"};
    int results[] = {12, 6, 27, 3, 49, 343};
    for(int i = 0; i < 6; i++) {
        featureInfo = pluginInfo->getFeatureInfo(i);
        ASSERT_EQ(featureInfos[i], featureInfo);
        ASSERT_TRUE(featureInfo);
        ASSERT_STREQ(featureInfo->returnType, "int");
        ASSERT_STREQ(featureInfo->featureGroup, i >= 4 ? "sixth_group_pow" : "sixth_group_math");
        ASSERT_STREQ(featureInfo->featureName, featureNames[i]);
        ASSERT_STREQ(featureInfo->parameterList, i >= 4 ? "int x" : "int x1, int x2");
        if(i >= 4)
            ASSERT_EQ(reinterpret_cast<int(*)(int)>(featureInfo->functionPointer)(7), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(featureInfo->functionPointer)(9, 3), results[i]);
    }

    // class check
    const apl::APluginClassInfo* classInfo;
    const apl::APluginClassInfo* const* classInfos = pluginInfo->getClassInfos();
    ASSERT_NE(classInfos, nullptr);

    Interface* interface;
    int result1[] = {35, 12, -2};
    int result2[] = {25, 125, 625};
    for(size_t i = 0; i < 3; i++) {
        classInfo = pluginInfo->getClassInfo(i);
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

    pluginInfo->privateInfo->destructPluginInternals(nullptr);
    apl::LibraryLoader::unload(handle);
}


GTEST_TEST(Test_PluginAPI, feature_and_class_plugin_infos)
{
    void* handle = apl::LibraryLoader::load("plugins/sixth/sixth_plugin");
    ASSERT_NE(handle, nullptr);
    auto getAPluginInfo = apl::LibraryLoader::getSymbol<const apl::APluginInfo*(*)()>(handle, "APluginSDK_getPluginInfo");
    ASSERT_NE(getAPluginInfo, nullptr);
    auto pluginInfo = getAPluginInfo();
    ASSERT_NE(pluginInfo, nullptr);
    auto initAPluginInfo = apl::LibraryLoader::getSymbol<void(*)()>(handle, "APluginSDK_initPlugin");
    ASSERT_NE(initAPluginInfo, nullptr);
    ASSERT_EQ(apl::LibraryLoader::getSymbol<void(*)()>(handle, "APluginSDK_finiPlugin"), nullptr);

    pluginInfo->privateInfo->constructPluginInternals(initAPluginInfo);

    ASSERT_NE(pluginInfo->getFeatureCount, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfo, nullptr);
    ASSERT_NE(pluginInfo->getFeatureInfos, nullptr);

    ASSERT_NE(pluginInfo->getClassCount, nullptr);
    ASSERT_NE(pluginInfo->getClassInfo, nullptr);
    ASSERT_NE(pluginInfo->getClassInfos, nullptr);

    ASSERT_EQ(pluginInfo->getFeatureCount(), 6);
    ASSERT_EQ(pluginInfo->getClassCount(), 3);

    // feature check
    const apl::APluginFeatureInfo* featureInfo;
    const apl::APluginFeatureInfo* const* featureInfos = pluginInfo->getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);
    for(int i = 0; i < 6; i++) {
        featureInfo = pluginInfo->getFeatureInfo(i);
        ASSERT_EQ(featureInfos[i], featureInfo);
        ASSERT_TRUE(featureInfo);
        ASSERT_STREQ(featureInfo->pluginInfo->pluginName, "sixth_plugin");
        ASSERT_EQ(featureInfo->pluginInfo->pluginVersionMajor, 1);
        ASSERT_EQ(featureInfo->pluginInfo->pluginVersionMinor, 2);
        ASSERT_EQ(featureInfo->pluginInfo->pluginVersionPatch, 3);
    }

    // class check
    const apl::APluginClassInfo* classInfo;
    const apl::APluginClassInfo* const* classInfos = pluginInfo->getClassInfos();
    ASSERT_NE(classInfos, nullptr);
    for(size_t i = 0; i < 3; i++) {
        classInfo = pluginInfo->getClassInfo(i);
        ASSERT_NE(classInfo, nullptr);
        ASSERT_EQ(classInfos[i], classInfo);
        ASSERT_STREQ(classInfo->pluginInfo->pluginName, "sixth_plugin");
        ASSERT_EQ(classInfo->pluginInfo->pluginVersionMajor, 1);
        ASSERT_EQ(classInfo->pluginInfo->pluginVersionMinor, 2);
        ASSERT_EQ(classInfo->pluginInfo->pluginVersionPatch, 3);
    }

    pluginInfo->privateInfo->constructPluginInternals(nullptr);
    apl::LibraryLoader::unload(handle);
}
