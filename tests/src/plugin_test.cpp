#include "gtest/gtest.h"

#include "APluginLibrary/plugin.h"
#include "APluginLibrary/plugininfos.h"

#include "../plugins/interface.h"

GTEST_TEST(Plugin_Test, load_unload)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    delete plugin;

    plugin = apl::Plugin::load("plugins/imaginary/imaginary_plugin");
    ASSERT_EQ(plugin, nullptr);

    plugin = apl::Plugin::load("libraries/first/first_lib");
    ASSERT_EQ(plugin, nullptr);
}

GTEST_TEST(Plugin_Test, getPath)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), "plugins/first/first_plugin");
    plugin->unload();
    ASSERT_FALSE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), "plugins/first/first_plugin");
    delete plugin;
}

GTEST_TEST(Plugin_Test, feature_loading_single)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/first/first_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getFeatureCount(), 1);

    const apl::PluginFeatureInfo* info = plugin->getFeatureInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::PluginFeatureInfo* const* infos = plugin->getFeatureInfos();
    ASSERT_NE(infos, nullptr);
    ASSERT_EQ(info, infos[0]);

    ASSERT_STREQ(info->returnType, "int");
    ASSERT_STREQ(info->featureGroup, "first_group1");
    ASSERT_STREQ(info->featureName, "feature1");
    ASSERT_STREQ(info->argumentList, "int x1, int x2");
    ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(7, 3), 21);

    delete plugin;
}

GTEST_TEST(Plugin_Test, feature_loading_multiple)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/second/second_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getFeatureCount(), 6);

    const apl::PluginFeatureInfo* info;
    const apl::PluginFeatureInfo* const* infos = plugin->getFeatureInfos();
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
        ASSERT_STREQ(info->argumentList, i >= 4 ? "int x" : "int x1, int x2");
        if(i >= 4)
            ASSERT_EQ(reinterpret_cast<int(*)(int)>(info->functionPointer)(7), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
    }

    delete plugin;
}



GTEST_TEST(Plugin_Test, class_loading_single)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/third/third_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getClassCount(), 1);

    const apl::PluginClassInfo* info = plugin->getClassInfo(0);
    ASSERT_NE(info, nullptr);
    const apl::PluginClassInfo* const* infos = plugin->getClassInfos();
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

GTEST_TEST(Plugin_Test, class_loading_multiple)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/fourth/fourth_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getClassCount(), 3);

    const apl::PluginClassInfo* info;
    const apl::PluginClassInfo* const* infos = plugin->getClassInfos();
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



GTEST_TEST(Plugin_Test, feature_and_class_loading_single)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/fifth/fifth_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());

    ASSERT_EQ(plugin->getFeatureCount(), 1);
    ASSERT_EQ(plugin->getClassCount(), 1);

    // feature check
    const apl::PluginFeatureInfo* featureInfo = plugin->getFeatureInfo(0);
    ASSERT_NE(featureInfo, nullptr);
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
    ASSERT_NE(featureInfos, nullptr);
    ASSERT_EQ(featureInfo, featureInfos[0]);

    ASSERT_STREQ(featureInfo->returnType, "int");
    ASSERT_STREQ(featureInfo->featureGroup, "fifth_group1");
    ASSERT_STREQ(featureInfo->featureName, "feature1");
    ASSERT_STREQ(featureInfo->argumentList, "int x1, int x2");
    ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(featureInfo->functionPointer)(7, 3), 21);

    // class check
    const apl::PluginClassInfo* classInfo = plugin->getClassInfo(0);
    ASSERT_NE(classInfo, nullptr);
    const apl::PluginClassInfo* const* classInfos = plugin->getClassInfos();
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

GTEST_TEST(Plugin_Test, feature_and_class_loading_multiple)
{
    apl::Plugin* plugin = apl::Plugin::load("plugins/sixth/sixth_plugin");
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin);

    ASSERT_EQ(plugin->getFeatureCount(), 6);
    ASSERT_EQ(plugin->getClassCount(), 3);

    // feature check
    const apl::PluginFeatureInfo* featureInfo;
    const apl::PluginFeatureInfo* const* featureInfos = plugin->getFeatureInfos();
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
        ASSERT_STREQ(featureInfo->argumentList, i >= 4 ? "int x" : "int x1, int x2");
        if(i >= 4)
            ASSERT_EQ(reinterpret_cast<int(*)(int)>(featureInfo->functionPointer)(7), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(featureInfo->functionPointer)(9, 3), results[i]);
    }

    // class check
    const apl::PluginClassInfo* classInfo;
    const apl::PluginClassInfo* const* classInfos = plugin->getClassInfos();
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