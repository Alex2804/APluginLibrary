#include "gtest/gtest.h"

#include <string>
#include <algorithm>

#include "APluginLibrary/pluginmanager.h"
#include "../../src/private/pluginmanagerprivate.h"
#include "APluginSDK/pluginapi.h"

#include "../plugins/interface.h"
#include "../plugins/otherinterface.h"
#include "../plugins/include.h"

extern const char* integratedPluginInitStatusString;

GTEST_TEST(Test_PluginManager, load_unload_single_extern)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string path = "plugins/first/first_plugin";

    // test with specific unloading
    ASSERT_NE(manager.load(path), nullptr);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 1);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);

    apl::Plugin* plugin = manager.getLoadedPlugins().front();
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), path);

    manager.unload(plugin);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);

    // test with unloadAll
    ASSERT_NE(manager.load(path), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);

    plugin = manager.getLoadedPlugins().front();
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), path);

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);
}

GTEST_TEST(Test_PluginManager, load_unload_single_integrated)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string path;

    // test with specific unloading
    ASSERT_NE(manager.load(path), nullptr);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 1);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);

    apl::Plugin* plugin = manager.getLoadedPlugins().front();
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), path);

    manager.unload(plugin);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);

    // test with unloadAll
    ASSERT_NE(manager.load(path), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);

    plugin = manager.getLoadedPlugins().front();
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), path);

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);

    integratedPluginInitStatusString = "";
}

GTEST_TEST(Test_PluginManager, load_unload_multiple)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/third/third_plugin"};

    // test with specific unloading
    for(size_t i = 0; i < 3; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);

    // test existing but invalid shared lib
    ASSERT_TRUE(manager.load("libraries/first/first_lib") == nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    std::vector<apl::Plugin*> plugins = manager.getLoadedPlugins();
    ASSERT_EQ(plugins.size(), 3);
    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_NE(plugins.at(i), nullptr);
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_EQ(manager.getLoadedPluginCount(), 3 - i);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3 - i);
        manager.unload(plugins.at(i));
        ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 2 - i);
        ASSERT_EQ(manager.getLoadedPluginCount(), 2 - i);
        ASSERT_EQ(manager.getLoadedPlugins().size(), 2 - i);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);

    // test not existing shared lib
    ASSERT_TRUE(manager.load("libraries/imaginary/imaginary_lib") == nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);

    // test with unloadAll
    for(size_t i = 0; i < 3; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);

    plugins = manager.getLoadedPlugins();
    ASSERT_EQ(plugins.size(), 3);
    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_NE(plugins.at(i), nullptr);
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    manager.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, loadDirectory_getPluginByPath)
{
    apl::PluginManager manager = apl::PluginManager();

    // test with specific unloading
    ASSERT_EQ(manager.loadDirectory("plugins", false), std::vector<apl::Plugin*>());
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    auto loadedVector = manager.loadDirectory("plugins/first", false);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 1);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);
    ASSERT_EQ(loadedVector, std::vector<apl::Plugin*>({apl::detail::PluginManagerPrivate::allPlugins.begin()->second.second}));

    apl::Plugin* plugin = manager.getLoadedPlugins().front();
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), "plugins/first/first_plugin");

    loadedVector = manager.loadDirectory("plugins", true);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 7);
    ASSERT_EQ(manager.getLoadedPluginCount(), 7);
    auto plugins = manager.getLoadedPlugins();
    std::sort(loadedVector.begin(), loadedVector.end());
    std::sort(plugins.begin(), plugins.end());
    ASSERT_EQ(loadedVector, plugins);

    // test same for const manager
    const apl::PluginManager constManager = manager;
    auto constLoadedVector = constManager.getLoadedPlugins();
    ASSERT_EQ(constManager.getLoadedPluginCount(), 7);
    auto constPlugins = constManager.getLoadedPlugins();
    std::sort(constLoadedVector.begin(), constLoadedVector.end());
    std::sort(constPlugins.begin(), constPlugins.end());
    ASSERT_EQ(constLoadedVector, constPlugins);

    ASSERT_EQ(manager.getLoadedPlugin("plugins/first/first_plugin"), plugin);
    ASSERT_EQ(constManager.getLoadedPlugin("plugins/first/first_plugin"), plugin);

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
}

GTEST_TEST(Test_PluginManager, test_no_double_loading)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/third/third_plugin"};

    for(const auto& path : paths) {
        ASSERT_NE(manager.load(path), nullptr);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);

    manager.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, test_shared_plugin_instances)
{
    apl::PluginManager manager1 = apl::PluginManager();
    apl::PluginManager manager2 = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/third/third_plugin"};

    for(const auto& path : paths) {
        ASSERT_NE(manager1.load(path), nullptr);
        ASSERT_NE(manager2.load(path), nullptr);
    }
    ASSERT_EQ(manager1.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 3);
    ASSERT_EQ(manager2.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager2.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);

    manager1.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);
    ASSERT_EQ(manager1.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 0);
    ASSERT_EQ(manager2.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager2.getLoadedPlugins().size(), 3);

    manager2.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);
    ASSERT_EQ(manager2.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager2.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, copy_assign_construct)
{
    apl::PluginManager manager1 = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/third/third_plugin"};

    // test with specific unloading
    for(size_t i = 0; i < 3; i++) {
        ASSERT_NE(manager1.load(paths[i]), nullptr);
        ASSERT_EQ(manager1.getLoadedPluginCount(), i + 1);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), i + 1);
    }
    ASSERT_EQ(manager1.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);

    std::vector<apl::Plugin*> plugins = manager1.getLoadedPlugins();
    ASSERT_EQ(plugins.size(), 3);
    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_NE(plugins.at(i), nullptr);
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    apl::PluginManager manager2 = manager1;
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);
    ASSERT_EQ(manager2.getLoadedPluginCount(), manager1.getLoadedPluginCount());
    ASSERT_EQ(manager2.getLoadedPlugins(), manager1.getLoadedPlugins());

    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_EQ(manager1.getLoadedPluginCount(), 3 - i);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);
        manager1.unload(plugins.at(i));
        ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);
        ASSERT_EQ(manager1.getLoadedPluginCount(), 2 - i);
        ASSERT_EQ(manager1.getLoadedPlugins().size(), 2 - i);
        ASSERT_EQ(manager2.getLoadedPluginCount(), 3);
    }
    ASSERT_EQ(manager1.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 0);
    ASSERT_EQ(manager2.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager2.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 3);

    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    manager2.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::allPlugins.size(), 0);
    ASSERT_EQ(manager1.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 0);
    ASSERT_EQ(manager2.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager2.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, getPluginInfo_unfiltered)
{
    apl::PluginManager manager = apl::PluginManager();
    ASSERT_NE(manager.load("plugins/first/first_plugin"), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 1);
    ASSERT_NE(manager.load("plugins/second/second_plugin"), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 2);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 2);

    std::vector<const apl::APluginInfo*> infos = manager.getPluginInfos();
    ASSERT_EQ(infos.size(), 2);
    const apl::APluginInfo* info1 = infos.front();
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

    const apl::APluginInfo* info2 = infos.back();
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
}

GTEST_TEST(Test_PluginManager, getPluginInfo_filtered)
{
    apl::PluginManager manager = apl::PluginManager();
    ASSERT_NE(manager.load("plugins/first/first_plugin"), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 1);
    ASSERT_NE(manager.load("plugins/second/second_plugin"), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 2);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 2);

    std::vector<const apl::APluginInfo*> infos = manager.getPluginInfos("first_plugin", apl::PluginInfoFilter::PluginName);
    ASSERT_EQ(infos.size(), 1);
    const apl::APluginInfo* info1 = infos.front();
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

    infos = manager.getPluginInfos("3.5.12", apl::PluginInfoFilter::PluginVersion);
    ASSERT_EQ(infos.size(), 1);
    const apl::APluginInfo* info2 = infos.front();
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

    // test name filter
    infos = manager.getPluginInfos("first_plugin", apl::PluginInfoFilter::PluginName);
    ASSERT_EQ(infos.size(), 1);
    infos = manager.getPluginInfos("second_plugin", apl::PluginInfoFilter::PluginName);
    ASSERT_EQ(infos.size(), 1);
    infos = manager.getPluginInfos("imaginary_plugin_name", apl::PluginInfoFilter::PluginName);
    ASSERT_EQ(infos.size(), 0);

    // test plugin version filter
    infos = manager.getPluginInfos("9.87.789", apl::PluginInfoFilter::PluginVersion);
    ASSERT_EQ(infos.size(), 1);
    infos = manager.getPluginInfos("3.5.12", apl::PluginInfoFilter::PluginVersion);
    ASSERT_EQ(infos.size(), 1);
    infos = manager.getPluginInfos("0.0.0", apl::PluginInfoFilter::PluginVersion);
    ASSERT_EQ(infos.size(), 0);

    // test api version filter
    infos = manager.getPluginInfos(std::to_string(APLUGINSDK_API_VERSION_MAJOR).append(".").append(std::to_string(APLUGINSDK_API_VERSION_MINOR)).append(".").append(std::to_string(APLUGINSDK_API_VERSION_PATCH)), apl::PluginInfoFilter::ApiVersion);
    ASSERT_EQ(infos.size(), 2);
    infos = manager.getPluginInfos(std::to_string(APLUGINSDK_API_VERSION_MAJOR).append(".").append(std::to_string(APLUGINSDK_API_VERSION_MINOR)).append(".").append(std::to_string(APLUGINSDK_API_VERSION_PATCH + 1)), apl::PluginInfoFilter::ApiVersion);
    ASSERT_EQ(infos.size(), 0);
}

GTEST_TEST(Test_PluginManager, getPluginProperties)
{
    apl::PluginManager manager = apl::PluginManager();
    ASSERT_NE(manager.load("plugins/first/first_plugin"), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 1);
    ASSERT_NE(manager.load("plugins/second/second_plugin"), nullptr);
    ASSERT_EQ(manager.getLoadedPluginCount(), 2);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 2);

    std::vector<std::string> properties = manager.getPluginProperties(apl::PluginInfoFilter::PluginName);
    std::vector<std::string> expectedProperties = {"first_plugin", "second_plugin"};
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    std::sort(expectedProperties.begin(), expectedProperties.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), expectedProperties.size());
    ASSERT_EQ(properties, expectedProperties);

    properties = manager.getPluginProperties(apl::PluginInfoFilter::PluginVersion);
    expectedProperties = {"9.87.789", "3.5.12"};
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    std::sort(expectedProperties.begin(), expectedProperties.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), expectedProperties.size());
    ASSERT_EQ(properties, expectedProperties);

    properties = manager.getPluginProperties(apl::PluginInfoFilter::ApiVersion);
    expectedProperties = {std::to_string(APLUGINSDK_API_VERSION_MAJOR).append(".").append(std::to_string(APLUGINSDK_API_VERSION_MINOR)).append(".").append(std::to_string(APLUGINSDK_API_VERSION_PATCH))};
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    std::sort(expectedProperties.begin(), expectedProperties.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), expectedProperties.size());
    ASSERT_EQ(properties, expectedProperties);
}

GTEST_TEST(Test_PluginManager, getFeatures_unfiltered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/fifth/fifth_plugin"};

    for(size_t i = 0; i < 3; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    std::vector<const apl::APluginFeatureInfo*> features = manager.getFeatures();
    ASSERT_EQ(features.size(), 9);
    const char* featureGroups[] = {"first_group1", "first_group1",
                                   "second_group_math", "second_group_math", "second_group_math", "second_group_math",
                                   "second_group_pow", "second_group_pow",
                                   "fifth_group1"};
    const char* featureNames[] = {"feature1", "feature2",
                                  "feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3",
                                  "feature1"};
    const char* returnTypes[] = {"int", "struct APluginLibrary_Test_PointStruct", "int", "int", "int", "int", "int", "int", "int"};
    const char* parameterLists[] = {"int x1, int x2", "int y, int x",
                                   "int x1, int x2", "int x1, int x2", "int x1, int x2", "int x1, int x2",
                                   "int x", "int x",
                                   ""};
    int results[] = {27, -1, 12, 6, 27, 3, 49, 343, 6};
    const apl::APluginFeatureInfo* info;
    for(size_t i = 0; i < features.size(); i++) {
        info = features.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->featureGroup, featureGroups[i]);
        ASSERT_STREQ(info->featureName, featureNames[i]);
        ASSERT_STREQ(info->returnType, returnTypes[i]);
        ASSERT_STREQ(info->parameterList, parameterLists[i]);
        if(std::string(info->parameterList) == "int x") {
            ASSERT_EQ(reinterpret_cast<int (*)(int)>(info->functionPointer)(7), results[i]);
        } else if(std::string(info->parameterList) == "int x1, int x2") {
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
        } else if(std::string(info->parameterList).empty()) {
            ASSERT_EQ(reinterpret_cast<int(*)()>(info->functionPointer)(), results[i]);
        } else {
            afl::APluginLibrary_Test_PointStruct APluginLibrary_Test_PointStruct = reinterpret_cast<afl::APluginLibrary_Test_PointStruct(*)(int, int)>(info->functionPointer)(13, 42);
            ASSERT_EQ(APluginLibrary_Test_PointStruct.x, 42);
            ASSERT_EQ(APluginLibrary_Test_PointStruct.y, 13);
        }
    }

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, getFeatures_filtered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/fifth/fifth_plugin",
                           "plugins/seventh/seventh_plugin"};

    for(size_t i = 0; i < 4; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 4);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 4);

    // filter feature groups
    std::vector<const apl::APluginFeatureInfo*> features = manager.getFeatures("second_group_math", apl::PluginFeatureFilter::FeatureGroup);
    ASSERT_EQ(features.size(), 4);
    std::vector<const char*> featureGroups;
    std::vector<const char*> featureNames = {"feature_add", "feature_sub", "feature_mul", "feature_div"};
    std::vector<const char*> returnTypes = {"int", "int", "int", "int"};
    std::vector<const char*> parameterLists = {"int x1, int x2", "int x1, int x2", "int x1, int x2", "int x1, int x2"};
    std::vector<int> results = {12, 6, 27, 3};
    const apl::APluginFeatureInfo* info;
    for(size_t i = 0; i < features.size(); i++) {
        info = features.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->featureGroup, "second_group_math");
        ASSERT_STREQ(info->featureName, featureNames[i]);
        ASSERT_STREQ(info->returnType, returnTypes[i]);
        ASSERT_STREQ(info->parameterList, parameterLists[i]);
        ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
    }

    // filter feature names
    features = manager.getFeatures("feature1", apl::PluginFeatureFilter::FeatureName);
    ASSERT_EQ(features.size(), 2);
    featureGroups = {"first_group1", "fifth_group1"};
    returnTypes = {"int", "int"};
    parameterLists = {"int x1, int x2", ""};
    results = {27, 6};
    for(size_t i = 0; i < features.size(); i++) {
        info = features.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->featureGroup, featureGroups[i]);
        ASSERT_STREQ(info->featureName, "feature1");
        ASSERT_STREQ(info->returnType, returnTypes[i]);
        ASSERT_STREQ(info->parameterList, parameterLists[i]);
        if(std::string(info->parameterList) == "int x1, int x2")
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)()>(info->functionPointer)(), results[i]);
    }

    // filter return types
    features = manager.getFeatures("char", apl::PluginFeatureFilter::ReturnType);
    ASSERT_EQ(features.size(), 1);
    info = features.at(0);
    ASSERT_NE(info, nullptr);
    ASSERT_STREQ(info->featureGroup, "seventh_group1");
    ASSERT_STREQ(info->featureName, "convert_to_char");
    ASSERT_STREQ(info->returnType, "char");
    ASSERT_STREQ(info->parameterList, "int x");
    ASSERT_EQ(reinterpret_cast<char(*)(int)>(info->functionPointer)(static_cast<int>('a')), 'a');

    // filter parameter lists
    features = manager.getFeatures("int x", apl::PluginFeatureFilter::ParameterList);
    ASSERT_EQ(features.size(), 3);
    featureGroups = {"second_group_pow", "second_group_pow", "seventh_group1"};
    featureNames = {"feature_pow2", "feature_pow3", "convert_to_char"};
    returnTypes = {"int", "int", "char"};
    results = {25, 125, 5};
    for(size_t i = 0; i < features.size(); i++) {
        info = features.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->featureGroup, featureGroups[i]);
        ASSERT_STREQ(info->featureName, featureNames[i]);
        ASSERT_STREQ(info->returnType, returnTypes[i]);
        ASSERT_STREQ(info->parameterList, "int x");
        int x;
        if(std::string(info->returnType) == "int")
            x = reinterpret_cast<int(*)(int)>(info->functionPointer)(5);
        else
            x = static_cast<int>(reinterpret_cast<char(*)(int)>(info->functionPointer)(5));
        ASSERT_EQ(x, results[i]);
    }

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, getFeatureProperties)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/fifth/fifth_plugin",
                           "plugins/seventh/seventh_plugin"};

    for(size_t i = 0; i < 4; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 4);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 4);

    std::vector<std::string> properties = manager.getFeatureProperties(apl::PluginFeatureFilter::FeatureGroup);
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    std::vector<std::string> result = {"first_group1", "second_group_math", "second_group_pow",
                                       "fifth_group1", "seventh_group1"};
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), result.size());
    ASSERT_EQ(properties, result);

    properties = manager.getFeatureProperties(apl::PluginFeatureFilter::FeatureName);
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    result = {"feature1", "feature2", "feature_add", "feature_sub", "feature_mul", "feature_div", "feature_pow2", "feature_pow3",
              "convert_to_char"};
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), result.size());
    ASSERT_EQ(properties, result);

    properties = manager.getFeatureProperties(apl::PluginFeatureFilter::ReturnType);
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    result = {"int", "struct APluginLibrary_Test_PointStruct", "char"};
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), result.size());
    ASSERT_EQ(properties, result);

    properties = manager.getFeatureProperties(apl::PluginFeatureFilter::ParameterList);
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    result = {"int x1, int x2", "int y, int x", "int x", ""};
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), result.size());
    ASSERT_EQ(properties, result);

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, getClasses_unfiltered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/third/third_plugin", "plugins/fourth/fourth_plugin", "plugins/fifth/fifth_plugin"};

    for(size_t i = 0; i < 3; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    std::vector<const apl::APluginClassInfo*> classes = manager.getClasses();
    ASSERT_EQ(classes.size(), 5);
    const char* interfaceClassNames[] = {"Interface", "Interface", "Interface", "Interface", "Interface"};
    const char* classNames[] = {"Implementation",
                                "Implementation0", "Implementation1", "Implementation2",
                                "Implementation"};
    int result1[] = {35, 35, 12, -2, 35};
    int result2[] = {25, 25, 125, 625, 25};
    const apl::APluginClassInfo* info;
    Interface* interface;
    for(size_t i = 0; i < classes.size(); i++) {
        info = classes.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->interfaceName, interfaceClassNames[i]);
        ASSERT_STREQ(info->className, classNames[i]);

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

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, getClasses_filtered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/fourth/fourth_plugin", "plugins/fifth/fifth_plugin", "plugins/seventh/seventh_plugin"};

    for(size_t i = 0; i < 3; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    // filter interface names
    std::vector<const apl::APluginClassInfo*> classes = manager.getClasses("Interface", apl::PluginClassFilter::InterfaceName);
    ASSERT_EQ(classes.size(), 4);
    const char* classNames[] = {"Implementation0", "Implementation1", "Implementation2", "Implementation"};
    int result1[] = {36, 15, -9, 36};
    int result2[] = {9, 27, 81, 9};
    const apl::APluginClassInfo* info;
    Interface* interface;
    for(size_t i = 0; i < classes.size(); i++) {
        info = classes.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->interfaceName, "Interface");
        ASSERT_STREQ(info->className, classNames[i]);

        auto createInstance = reinterpret_cast<Interface*(*)()>(info->createInstance);
        auto deleteInstance = reinterpret_cast<void(*)(Interface*)>(info->deleteInstance);
        ASSERT_NE(createInstance, nullptr);
        ASSERT_NE(deleteInstance, nullptr);

        interface = createInstance();
        ASSERT_NE(interface, nullptr);

        ASSERT_EQ(interface->function1(3, 12), result1[i]);
        ASSERT_EQ(interface->function2(3), result2[i]);

        deleteInstance(interface);
    }

    // filter class names
    classes = manager.getClasses("Implementation1", apl::PluginClassFilter::ClassName);
    ASSERT_EQ(classes.size(), 2);
    const char* interfaceNames[] = {"Interface", "OtherInterface"};
    for(size_t i = 0; i < classes.size(); i++) {
        info = classes.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->interfaceName, interfaceNames[i]);
        ASSERT_STREQ(info->className, "Implementation1");

        if(std::string(info->interfaceName) == "Interface") {
            auto createInstance = reinterpret_cast<Interface *(*)()>(info->createInstance);
            auto deleteInstance = reinterpret_cast<void (*)(Interface *)>(info->deleteInstance);
            ASSERT_NE(createInstance, nullptr);
            ASSERT_NE(deleteInstance, nullptr);

            interface = createInstance();
            ASSERT_NE(interface, nullptr);

            ASSERT_EQ(interface->function1(3, 12), 15);
            ASSERT_EQ(interface->function2(3), 27);

            deleteInstance(interface);
        } else if(std::string(info->interfaceName) == "OtherInterface") {
            auto createInstance = reinterpret_cast<OtherInterface *(*)()>(info->createInstance);
            auto deleteInstance = reinterpret_cast<void (*)(OtherInterface *)>(info->deleteInstance);
            ASSERT_NE(createInstance, nullptr);
            ASSERT_NE(deleteInstance, nullptr);

            OtherInterface* otherInterface = createInstance();
            ASSERT_NE(otherInterface, nullptr);

            ASSERT_EQ(otherInterface->otherFunction1(3, 12, 7), 8);
            ASSERT_STREQ(otherInterface->otherFunction2(), "This is for testing!");
            ASSERT_EQ(otherInterface->otherFunction3(4.2), 4);

            deleteInstance(otherInterface);
        } else {
            ASSERT_TRUE(false) << R"(Interface must be from type "Interface" or "OtherInterface" but has type )" << info->interfaceName;
        }
    }

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(Test_PluginManager, getClassProperties)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/fourth/fourth_plugin", "plugins/fifth/fifth_plugin", "plugins/seventh/seventh_plugin"};

    for(size_t i = 0; i < 3; i++) {
        ASSERT_NE(manager.load(paths[i]), nullptr);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    std::vector<std::string> properties = manager.getClassProperties(apl::PluginClassFilter::InterfaceName);
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    std::vector<std::string> result = {"Interface", "OtherInterface"};
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), result.size());
    ASSERT_EQ(properties, result);

    properties = manager.getClassProperties(apl::PluginClassFilter::ClassName);
    std::sort(properties.begin(), properties.end(), std::greater<std::string>());
    result = {"Implementation0", "Implementation1", "Implementation2", "Implementation"};
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    ASSERT_EQ(properties.size(), result.size());
    ASSERT_EQ(properties, result);


    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}