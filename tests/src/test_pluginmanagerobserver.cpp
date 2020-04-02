#include "gtest/gtest.h"

#include <unordered_map>
#include <algorithm>

#include "APluginLibrary/pluginmanagerobserver.h"

namespace
{
    class Observer : public apl::PluginManagerObserver
    {
    public:
        void pluginLoaded(apl::PluginManager* pluginManager, apl::Plugin* plugin) override
        {
            auto iterator = loaded.find(pluginManager);
            if(iterator != loaded.end())
                loaded[pluginManager].push_back(plugin);
            else
                loaded.insert({pluginManager, {plugin}});
            ++loadCounter;
            std::sort(loaded[pluginManager].begin(), loaded[pluginManager].end());
        }

        void pluginUnloaded(apl::PluginManager* pluginManager, apl::Plugin* plugin) override
        {
            auto iterator = unloaded.find(pluginManager);
            if(iterator != unloaded.end())
                unloaded[pluginManager].push_back(plugin);
            else
                unloaded.insert({pluginManager, {plugin}});
            ++unloadCounter;
            std::sort(unloaded[pluginManager].begin(), unloaded[pluginManager].end());
        }

        int loadCounter = 0, unloadCounter = 0;
        std::unordered_map<apl::PluginManager*, std::vector<apl::Plugin*>> loaded, unloaded;
    };
}


GTEST_TEST(Test_PluginManagerObserver, load_notification)
{
    auto manager1 = new apl::PluginManager(), manager2 = new apl::PluginManager();
    auto observer = new Observer();
    std::unordered_map<apl::PluginManager*, std::vector<apl::Plugin*>> expectedLoaded;
    manager1->addObserver(observer);
    manager2->addObserver(observer);
    apl::Plugin* firstPlugin = manager1->load("plugins/first/first_plugin");
    ASSERT_EQ(observer->loadCounter, 1);
    expectedLoaded.insert({manager1, {firstPlugin}});
    ASSERT_EQ(observer->loaded, expectedLoaded);
    apl::Plugin* secondPlugin = manager1->load("plugins/second/second_plugin");
    ASSERT_EQ(observer->loadCounter, 2);
    expectedLoaded[manager1].push_back(secondPlugin);
    std::sort(expectedLoaded[manager1].begin(), expectedLoaded[manager1].end());
    ASSERT_EQ(observer->loaded, expectedLoaded);
    ASSERT_EQ(manager2->load("plugins/second/second_plugin"), secondPlugin);
    ASSERT_EQ(observer->loadCounter, 3);
    expectedLoaded.insert({manager2, {secondPlugin}});
    std::sort(expectedLoaded[manager2].begin(), expectedLoaded[manager2].end());
    ASSERT_EQ(observer->loaded, expectedLoaded);
    ASSERT_EQ(manager1->load("plugins/first/first_plugin"), firstPlugin);
    ASSERT_EQ(observer->loadCounter, 3);
    manager1->loadDirectory("plugins", true);
    ASSERT_EQ(observer->loadCounter, 8);
    expectedLoaded[manager1] = manager1->getLoadedPlugins();
    std::sort(expectedLoaded[manager1].begin(), expectedLoaded[manager1].end());
    ASSERT_EQ(observer->loaded, expectedLoaded);
    manager1->loadDirectory("plugins", true);
    ASSERT_EQ(observer->loadCounter, 8);
    ASSERT_EQ(observer->loaded, expectedLoaded);
    delete manager1;
    delete manager2;
    delete observer;
}

GTEST_TEST(Test_PluginManagerObserver, unload_notification)
{
    auto manager1 = new apl::PluginManager(), manager2 = new apl::PluginManager();
    manager1->loadDirectory("plugins", true);
    manager2->loadDirectory("plugins", true);
    auto observer = new Observer();
    std::unordered_map<apl::PluginManager*, std::vector<apl::Plugin*>> expectedUnloaded;
    manager1->addObserver(observer);
    manager2->addObserver(observer);
    apl::Plugin* firstPlugin = manager1->getLoadedPlugin("plugins/first/first_plugin");
    manager1->unload(firstPlugin);
    ASSERT_EQ(observer->unloadCounter, 1);
    expectedUnloaded.insert({manager1, {firstPlugin}});
    ASSERT_EQ(observer->unloaded, expectedUnloaded);
    apl::Plugin* secondPlugin = manager2->getLoadedPlugin("plugins/second/second_plugin");
    manager2->unload(secondPlugin);
    ASSERT_EQ(observer->unloadCounter, 2);
    expectedUnloaded.insert({manager2, {secondPlugin}});
    ASSERT_EQ(observer->unloaded, expectedUnloaded);
    manager1->unload(secondPlugin);
    ASSERT_EQ(observer->unloadCounter, 3);
    expectedUnloaded[manager1].push_back(secondPlugin);
    std::sort(expectedUnloaded[manager1].begin(), expectedUnloaded[manager1].end());
    ASSERT_EQ(observer->unloaded, expectedUnloaded);
    manager2->unload(secondPlugin);
    ASSERT_EQ(observer->unloadCounter, 3);
    manager1->unloadAll();
    ASSERT_EQ(observer->unloadCounter, 8);
    delete manager1;
    ASSERT_EQ(observer->unloadCounter, 8);
    delete manager2;
    ASSERT_EQ(observer->unloadCounter, 14);
    delete observer;
}
