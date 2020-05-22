#include "gtest/gtest.h"

#include "APluginLibrary/plugin.h"

#include "APluginSDK/pluginapi.h"

#include "../plugins/interface.h"
#include "../plugins/include.h"

extern const char* integratedPluginInitStatusString;

GTEST_TEST(Test_Plugin, load_unload_extern)
{
    apl::library_handle handle = apl::LibraryLoader::load("plugins/first/first_plugin"); // load second ref to dll to hold it in memory while testing fini function
    apl::Plugin* plugin = apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    const char** initStatus = static_cast<const char**>(apl::LibraryLoader::getSymbol(const_cast<apl::library_handle>(plugin->getHandle()), "firstPluginInitStatusString"));
    ASSERT_NE(initStatus, nullptr);
    ASSERT_STREQ(*initStatus, "first plugin -> initialized");
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    ASSERT_STREQ(*initStatus, "first plugin -> finalized");
    delete plugin;
    apl::LibraryLoader::unload(handle);

    plugin = apl::Plugin::load("plugins/imaginary/imaginary_plugin");
    ASSERT_EQ(plugin, nullptr);

    plugin = apl::Plugin::load("libraries/first/first_lib");
    ASSERT_EQ(plugin, nullptr);
}
GTEST_TEST(Test_Plugin, load_unload_integrated)
{
    ASSERT_STREQ(integratedPluginInitStatusString, "");
    apl::Plugin* plugin = apl::Plugin::load("");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_STREQ(integratedPluginInitStatusString, "integrated plugin -> initialized");
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    ASSERT_STREQ(integratedPluginInitStatusString, "integrated plugin -> finalized");
    integratedPluginInitStatusString = "";
    delete plugin;
    integratedPluginInitStatusString = "";
}

GTEST_TEST(Test_Plugin, getPath_extern)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/first/first_plugin");
    delete apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), "plugins/first/first_plugin");
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), "plugins/first/first_plugin");
    delete plugin;
}
GTEST_TEST(Test_Plugin, getPath_integrated)
{
    apl::Plugin* plugin = apl::Plugin::load("");
    delete apl::Plugin::load("");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), "");
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), "");
    delete plugin;
    integratedPluginInitStatusString = "";
}

GTEST_TEST(Test_Plugin, getPluginInfo_extern)
{
    apl::Plugin* plugin1 = apl::Plugin::load("plugins/first/first_plugin");
    delete apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin1, nullptr);
    apl::Plugin* plugin2 = apl::Plugin::load("plugins/second/second_plugin");
    delete apl::Plugin::load("plugins/second/second_plugin");
    ASSERT_NE(plugin2, nullptr);

    const apl::APluginInfo* info1 = plugin1->getPluginInfo();
    ASSERT_NE(info1, nullptr);

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

    const apl::APluginInfo* info2 = plugin2->getPluginInfo();
    ASSERT_NE(info2, nullptr);

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

    delete plugin1;
    delete plugin2;
}
GTEST_TEST(Test_Plugin, getPluginInfo_integrated)
{
    apl::Plugin* plugin = apl::Plugin::load("");
    delete apl::Plugin::load("");
    ASSERT_NE(plugin, nullptr);

    const apl::APluginInfo* info = plugin->getPluginInfo();
    ASSERT_NE(info, nullptr);

    ASSERT_STREQ(info->pluginName, "integrated_plugin_name");
    ASSERT_EQ(info->pluginVersionMajor, 28);
    ASSERT_EQ(info->pluginVersionMinor, 4);
    ASSERT_EQ(info->pluginVersionPatch, 2000);
    ASSERT_EQ(info->apiVersionMajor, APLUGINSDK_API_VERSION_MAJOR);
    ASSERT_EQ(info->apiVersionMinor, APLUGINSDK_API_VERSION_MINOR);
    ASSERT_EQ(info->apiVersionPatch, APLUGINSDK_API_VERSION_PATCH);
    ASSERT_NE(info->allocateMemory, nullptr);
    ASSERT_NE(info->freeMemory, nullptr);
    ASSERT_NE(info->getFeatureCount, nullptr);
    ASSERT_NE(info->getFeatureInfo, nullptr);
    ASSERT_NE(info->getFeatureInfos, nullptr);
    ASSERT_NE(info->getClassCount, nullptr);
    ASSERT_NE(info->getClassInfo, nullptr);
    ASSERT_NE(info->getClassInfos, nullptr);

    ASSERT_EQ(info->getFeatureCount(), 2);
    ASSERT_EQ(info->getClassCount(), 1);

    delete plugin;
    integratedPluginInitStatusString = "";
}

GTEST_TEST(Test_Plugin, memory_allocate_free)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/first/first_plugin");
    delete apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    void* ptr = plugin->allocateMemory(sizeof(apl::APluginClassInfo));
    ASSERT_NE(ptr, nullptr);
    auto classPtr = static_cast<apl::APluginClassInfo*>(ptr);

    classPtr->className = "TestClassName";
    classPtr->interfaceName = "TestInterfaceName";
    classPtr->createInstance = nullptr;
    classPtr->deleteInstance = nullptr;

    ASSERT_STREQ(classPtr->className, "TestClassName");
    ASSERT_STREQ(classPtr->interfaceName, "TestInterfaceName");
    ASSERT_EQ(classPtr->createInstance, nullptr);
    ASSERT_EQ(classPtr->deleteInstance, nullptr);

    ASSERT_TRUE(plugin->freeMemory(ptr));

    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    ASSERT_FALSE(plugin->freeMemory(ptr));
    delete plugin;
}

GTEST_TEST(Test_Plugin, feature_loading_single)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/first/first_plugin");
    delete apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getFeatureCount(), 2);

    const apl::APluginFeatureInfo* info = plugin->getFeatureInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::APluginFeatureInfo* const* infos = plugin->getFeatureInfos();
    ASSERT_NE(infos, nullptr);
    ASSERT_EQ(info, infos[0]);

    ASSERT_STREQ(info->returnType, "int");
    ASSERT_STREQ(info->featureGroup, "first_group1");
    ASSERT_STREQ(info->featureName, "feature1");
    ASSERT_STREQ(info->parameterList, "int x1, int x2");
    ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(7, 3), 21);

    info = plugin->getFeatureInfo(1);
    ASSERT_NE(info, nullptr);
    ASSERT_EQ(info, infos[1]);

    ASSERT_STREQ(info->returnType, "struct APluginLibrary_Test_PointStruct");
    ASSERT_STREQ(info->featureGroup, "first_group1");
    ASSERT_STREQ(info->featureName, "feature2");
    ASSERT_STREQ(info->parameterList, "int y, int x");
    afl::APluginLibrary_Test_PointStruct APluginLibrary_Test_PointStruct = reinterpret_cast<afl::APluginLibrary_Test_PointStruct(*)(int, int)>(info->functionPointer)(7, 3);
    ASSERT_EQ(APluginLibrary_Test_PointStruct.x, 3);
    ASSERT_EQ(APluginLibrary_Test_PointStruct.y, 7);

    delete plugin;
}

GTEST_TEST(Test_Plugin, feature_loading_multiple)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/second/second_plugin");
    delete apl::Plugin::load("plugins/second/second_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getFeatureCount(), 6);

    const apl::APluginFeatureInfo* info;
    const apl::APluginFeatureInfo* const* infos = plugin->getFeatureInfos();
    ASSERT_NE(infos, nullptr);

    const char* featureNames[] = {"feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3"};
    int results[] = {12, 6, 27, 3, 49, 343};
    for(size_t i = 0; i < 6; i++) {
        info = plugin->getFeatureInfo(i);
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

    delete plugin;
}



GTEST_TEST(Test_Plugin, class_loading_single)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/third/third_plugin");
    delete apl::Plugin::load("plugins/third/third_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getClassCount(), 1);

    const apl::APluginClassInfo* info = plugin->getClassInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::APluginClassInfo* const* infos = plugin->getClassInfos();
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

    delete plugin;
}

GTEST_TEST(Test_Plugin, class_loading_multiple)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/fourth/fourth_plugin");
    delete apl::Plugin::load("plugins/fourth/fourth_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getClassCount(), 3);

    const apl::APluginClassInfo* info;
    const apl::APluginClassInfo* const* infos = plugin->getClassInfos();
    ASSERT_NE(infos, nullptr);

    Interface* interface;
    int result1[] = {35, 12, -2};
    int result2[] = {25, 125, 625};
    for(size_t i = 0; i < 3; i++) {
        info = plugin->getClassInfo(i);
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

    delete plugin;
}



GTEST_TEST(Test_Plugin, feature_and_class_loading_single)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/fifth/fifth_plugin");
    delete apl::Plugin::load("plugins/fifth/fifth_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getFeatureCount(), 1);
    ASSERT_EQ(plugin->getClassCount(), 1);

    // feature check
    const apl::APluginFeatureInfo* featureInfo = plugin->getFeatureInfo(0);
    ASSERT_NE(featureInfo, nullptr);
    const apl::APluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);
    ASSERT_EQ(featureInfo, featureInfos[0]);

    ASSERT_STREQ(featureInfo->returnType, "int");
    ASSERT_STREQ(featureInfo->featureGroup, "fifth_group1");
    ASSERT_STREQ(featureInfo->featureName, "feature1");
    ASSERT_STREQ(featureInfo->parameterList, "");
    ASSERT_EQ(reinterpret_cast<int(*)()>(featureInfo->functionPointer)(), 6);

    // class check
    const apl::APluginClassInfo* classInfo = plugin->getClassInfo(0);
    ASSERT_NE(classInfo, nullptr);
    const apl::APluginClassInfo* const* classInfos = plugin->getClassInfos();
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

    delete plugin;
}

GTEST_TEST(Test_Plugin, feature_and_class_loading_multiple)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/sixth/sixth_plugin");
    delete apl::Plugin::load("plugins/sixth/sixth_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin);

    ASSERT_EQ(plugin->getFeatureCount(), 6);
    ASSERT_EQ(plugin->getClassCount(), 3);

    // feature check
    const apl::APluginFeatureInfo* featureInfo;
    const apl::APluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);

    const char* featureNames[] = {"feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3"};
    int results[] = {12, 6, 27, 3, 49, 343};
    for(int i = 0; i < 6; i++) {
        featureInfo = plugin->getFeatureInfo(i);
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
    const apl::APluginClassInfo* const* classInfos = plugin->getClassInfos();
    ASSERT_NE(classInfos, nullptr);

    Interface* interface;
    int result1[] = {35, 12, -2};
    int result2[] = {25, 125, 625};
    for(size_t i = 0; i < 3; i++) {
        classInfo = plugin->getClassInfo(i);
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

    delete plugin;
}
