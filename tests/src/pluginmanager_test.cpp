#include "gtest/gtest.h"

#include <string>

#include "APluginLibrary/plugininfos.h"
#include "APluginLibrary/pluginmanager.h"

#include "../../src/private/pluginmanagerprivate.h"

#include "../plugins/interfaces.h"

GTEST_TEST(PluginManager_Test, load_unload_single)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string path = "plugins/first/first_plugin";

    // test with specific unloading
    manager.load(path);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 1);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);

    apl::Plugin* plugin = manager.getLoadedPlugins().front();
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), path);

    manager.unload(plugin);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);

    // test with unloadAll
    manager.load(path);
    ASSERT_EQ(manager.getLoadedPluginCount(), 1);

    plugin = manager.getLoadedPlugins().front();
    ASSERT_NE(plugin, nullptr);
    ASSERT_TRUE(plugin->isLoaded());
    ASSERT_EQ(plugin->getPath(), path);

    manager.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 0);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
}

GTEST_TEST(PluginManager_Test, load_unload_multiple)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/third/third_plugin"};

    // test with specific unloading
    for(size_t i = 0; i < 3; i++) {
        manager.load(paths[i]);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);

    // test existing but invalid shared lib
    manager.load("libraries/first/first_lib");
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
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3 - i);
        manager.unload(plugins.at(i));
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 2 - i);
        ASSERT_EQ(manager.getLoadedPluginCount(), 2 - i);
        ASSERT_EQ(manager.getLoadedPlugins().size(), 2 - i);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);

    // test not existing shared lib
    manager.load("libraries/imaginary/imaginary_lib");
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);

    // test with unloadAll
    for(size_t i = 0; i < 3; i++) {
        manager.load(paths[i]);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);

    plugins = manager.getLoadedPlugins();
    ASSERT_EQ(plugins.size(), 3);
    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_NE(plugins.at(i), nullptr);
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    manager.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 0);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);

    // test with variadic template unloading
    for(size_t i = 0; i < 3; i++) {
        manager.load(paths[i]);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);

    plugins = manager.getLoadedPlugins();
    ASSERT_EQ(plugins.size(), 3);
    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_NE(plugins.at(i), nullptr);
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    manager.unload(plugins.at(0), plugins.at(1), plugins.at(2));
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 0);
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(PluginManager_Test, copy_assign_construct)
{
    apl::PluginManager manager1 = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/third/third_plugin"};

    // test with specific unloading
    for(size_t i = 0; i < 3; i++) {
        manager1.load(paths[i]);
        ASSERT_EQ(manager1.getLoadedPluginCount(), i + 1);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), i + 1);
    }
    ASSERT_EQ(manager1.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);

    std::vector<apl::Plugin*> plugins = manager1.getLoadedPlugins();
    ASSERT_EQ(plugins.size(), 3);
    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_NE(plugins.at(i), nullptr);
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    apl::PluginManager manager2 = manager1;
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);
    ASSERT_EQ(manager2.getLoadedPluginCount(), manager1.getLoadedPluginCount());
    ASSERT_EQ(manager2.getLoadedPlugins(), manager1.getLoadedPlugins());

    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_EQ(manager1.getLoadedPluginCount(), 3 - i);
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);
        manager1.unload(plugins.at(i));
        ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);
        ASSERT_EQ(manager1.getLoadedPluginCount(), 2 - i);
        ASSERT_EQ(manager1.getLoadedPlugins().size(), 2 - i);
        ASSERT_EQ(manager2.getLoadedPluginCount(), 3);
    }
    ASSERT_EQ(manager1.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 0);
    ASSERT_EQ(manager2.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager2.getLoadedPlugins().size(), 3);
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 3);

    for(size_t i = 0; i < plugins.size(); i++) {
        ASSERT_TRUE(plugins.at(i)->isLoaded());
        ASSERT_EQ(plugins.at(i)->getPath(), paths[i]);
    }

    manager2.unloadAll();
    ASSERT_EQ(apl::detail::PluginManagerPrivate::loadedPlugins.size(), 0);
    ASSERT_EQ(manager1.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager1.getLoadedPlugins().size(), 0);
    ASSERT_EQ(manager2.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager2.getLoadedPlugins().size(), 0);
}

GTEST_TEST(PluginManager_Test, getFeatures_unfiltered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/fifth/fifth_plugin"};

    for(size_t i = 0; i < 3; i++) {
        manager.load(paths[i]);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    std::vector<const apl::PluginFeatureInfo*> features = manager.getFeatures();
    ASSERT_EQ(features.size(), 8);
    const char* featureGroups[] = {"first_group1",
                                   "second_group_math", "second_group_math", "second_group_math", "second_group_math",
                                   "second_group_pow", "second_group_pow",
                                   "fifth_group1"};
    const char* featureNames[] = {"feature1",
                                  "feature_add", "feature_sub", "feature_mul", "feature_div",
                                  "feature_pow2", "feature_pow3",
                                  "feature1"};
    const char* returnTypes[] = {"int", "int", "int", "int", "int", "int", "int", "int"};
    const char* argumentLists[] = {"int x1, int x2",
                                   "int x1, int x2", "int x1, int x2", "int x1, int x2", "int x1, int x2",
                                   "int x", "int x",
                                   "int x1, int x2"};
    int results[] = {27, 12, 6, 27, 3, 49, 343, 27};
    const apl::PluginFeatureInfo* info;
    for(size_t i = 0; i < features.size(); i++) {
        info = features.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->featureGroup, featureGroups[i]);
        ASSERT_STREQ(info->featureName, featureNames[i]);
        ASSERT_STREQ(info->returnType, returnTypes[i]);
        ASSERT_STREQ(info->argumentList, argumentLists[i]);
        if(std::string(info->argumentList) == "int x")
            ASSERT_EQ(reinterpret_cast<int(*)(int)>(info->functionPointer)(7), results[i]);
        else
            ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
    }

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(PluginManager_Test, getFeatures_filtered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/first/first_plugin", "plugins/second/second_plugin", "plugins/fifth/fifth_plugin",
                           "plugins/seventh/seventh_plugin"};

    for(size_t i = 0; i < 4; i++) {
        manager.load(paths[i]);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 4);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 4);

    // filter feature groups
    std::vector<const apl::PluginFeatureInfo*> features = manager.getFeatures("second_group_math", apl::PluginFeatureFilter::FeatureGroup);
    ASSERT_EQ(features.size(), 4);
    std::vector<const char*> featureGroups;
    std::vector<const char*> featureNames = {"feature_add", "feature_sub", "feature_mul", "feature_div"};
    std::vector<const char*> returnTypes = {"int", "int", "int", "int"};
    std::vector<const char*> argumentLists = {"int x1, int x2", "int x1, int x2", "int x1, int x2", "int x1, int x2"};
    std::vector<int> results = {12, 6, 27, 3};
    const apl::PluginFeatureInfo* info;
    for(size_t i = 0; i < features.size(); i++) {
        info = features.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->featureGroup, "second_group_math");
        ASSERT_STREQ(info->featureName, featureNames[i]);
        ASSERT_STREQ(info->returnType, returnTypes[i]);
        ASSERT_STREQ(info->argumentList, argumentLists[i]);
        ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
    }

    // filter feature names
    features = manager.getFeatures("feature1", apl::PluginFeatureFilter::FeatureName);
    ASSERT_EQ(features.size(), 2);
    featureGroups = {"first_group1", "fifth_group1"};
    returnTypes = {"int", "int"};
    argumentLists = {"int x1, int x2", "int x1, int x2"};
    results = {27, 27};
    for(size_t i = 0; i < features.size(); i++) {
        info = features.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->featureGroup, featureGroups[i]);
        ASSERT_STREQ(info->featureName, "feature1");
        ASSERT_STREQ(info->returnType, returnTypes[i]);
        ASSERT_STREQ(info->argumentList, argumentLists[i]);
        ASSERT_EQ(reinterpret_cast<int(*)(int, int)>(info->functionPointer)(9, 3), results[i]);
    }

    // filter return types
    features = manager.getFeatures("char", apl::PluginFeatureFilter::ReturnType);
    ASSERT_EQ(features.size(), 1);
    info = features.at(0);
    ASSERT_NE(info, nullptr);
    ASSERT_STREQ(info->featureGroup, "seventh_group1");
    ASSERT_STREQ(info->featureName, "convert_to_char");
    ASSERT_STREQ(info->returnType, "char");
    ASSERT_STREQ(info->argumentList, "int x");
    ASSERT_EQ(reinterpret_cast<char(*)(int)>(info->functionPointer)(static_cast<int>('a')), 'a');

    // filter argument lists
    features = manager.getFeatures("int x", apl::PluginFeatureFilter::ArgumentList);
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
        ASSERT_STREQ(info->argumentList, "int x");
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

GTEST_TEST(PluginManager_Test, getClasses_unfiltered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/third/third_plugin", "plugins/fourth/fourth_plugin", "plugins/fifth/fifth_plugin"};

    for(size_t i = 0; i < 3; i++) {
        manager.load(paths[i]);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    std::vector<const apl::PluginClassInfo*> classes = manager.getClasses();
    ASSERT_EQ(classes.size(), 5);
    const char* interfaceClassNames[] = {"Interface", "Interface", "Interface", "Interface", "Interface"};
    const char* classNames[] = {"Implementation",
                                "Implementation0", "Implementation1", "Implementation2",
                                "Implementation"};
    int result1[] = {35, 35, 12, -2, 35};
    int result2[] = {25, 25, 125, 625, 25};
    const apl::PluginClassInfo* info;
    Interface* interface;
    for(size_t i = 0; i < classes.size(); i++) {
        info = classes.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->interfaceClassName, interfaceClassNames[i]);
        ASSERT_STREQ(info->className, classNames[i]);

        auto createInstance = reinterpret_cast<Interface*(*)()>(info->createInstance);
        auto deleteInstance = reinterpret_cast<void(*)(Interface*)>(info->deleteInstance);
        ASSERT_NE(createInstance, nullptr);
        ASSERT_NE(deleteInstance, nullptr);

        interface = createInstance();
        ASSERT_NE(interface, nullptr);

        ASSERT_EQ(interface->function1(5, 7), result1[i]);
        ASSERT_EQ(interface->function2(5), result2[i]);
    }

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}

GTEST_TEST(PluginManager_Test, getClasses_filtered)
{
    apl::PluginManager manager = apl::PluginManager();
    std::string paths[] = {"plugins/fourth/fourth_plugin", "plugins/fifth/fifth_plugin", "plugins/seventh/seventh_plugin"};

    for(size_t i = 0; i < 3; i++) {
        manager.load(paths[i]);
        ASSERT_EQ(manager.getLoadedPluginCount(), i + 1);
    }
    ASSERT_EQ(manager.getLoadedPluginCount(), 3);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 3);

    // filter interface names
    std::vector<const apl::PluginClassInfo*> classes = manager.getClasses("Interface", apl::PluginClassFilter::InterfaceName);
    ASSERT_EQ(classes.size(), 4);
    const char* classNames[] = {"Implementation0", "Implementation1", "Implementation2", "Implementation"};
    int result1[] = {36, 15, -9, 36};
    int result2[] = {9, 27, 81, 9};
    const apl::PluginClassInfo* info;
    Interface* interface;
    for(size_t i = 0; i < classes.size(); i++) {
        info = classes.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->interfaceClassName, "Interface");
        ASSERT_STREQ(info->className, classNames[i]);

        auto createInstance = reinterpret_cast<Interface*(*)()>(info->createInstance);
        auto deleteInstance = reinterpret_cast<void(*)(Interface*)>(info->deleteInstance);
        ASSERT_NE(createInstance, nullptr);
        ASSERT_NE(deleteInstance, nullptr);

        interface = createInstance();
        ASSERT_NE(interface, nullptr);

        ASSERT_EQ(interface->function1(3, 12), result1[i]);
        ASSERT_EQ(interface->function2(3), result2[i]);
    }

    // filter class names
    classes = manager.getClasses("Implementation1", apl::PluginClassFilter::ClassName);
    ASSERT_EQ(classes.size(), 2);
    const char* interfaceNames[] = {"Interface", "OtherInterface"};
    for(size_t i = 0; i < classes.size(); i++) {
        info = classes.at(i);
        ASSERT_NE(info, nullptr);
        ASSERT_STREQ(info->interfaceClassName, interfaceNames[i]);
        ASSERT_STREQ(info->className, "Implementation1");

        if(std::string(info->interfaceClassName) == "Interface") {
            auto createInstance = reinterpret_cast<Interface *(*)()>(info->createInstance);
            auto deleteInstance = reinterpret_cast<void (*)(Interface *)>(info->deleteInstance);
            ASSERT_NE(createInstance, nullptr);
            ASSERT_NE(deleteInstance, nullptr);

            interface = createInstance();
            ASSERT_NE(interface, nullptr);

            ASSERT_EQ(interface->function1(3, 12), 15);
            ASSERT_EQ(interface->function2(3), 27);
        } else {
            auto createInstance = reinterpret_cast<OtherInterface *(*)()>(info->createInstance);
            auto deleteInstance = reinterpret_cast<void (*)(OtherInterface *)>(info->deleteInstance);
            ASSERT_NE(createInstance, nullptr);
            ASSERT_NE(deleteInstance, nullptr);

            OtherInterface* otherInterface = createInstance();
            ASSERT_NE(otherInterface, nullptr);

            ASSERT_EQ(otherInterface->otherFunction1(3, 12, 7), 8);
            ASSERT_STREQ(otherInterface->otherFunction2(), "This is for testing!");
        }
    }

    manager.unloadAll();
    ASSERT_EQ(manager.getLoadedPluginCount(), 0);
    ASSERT_EQ(manager.getLoadedPlugins().size(), 0);
}