# APluginLibrary
Simple C++ Plugin Library.

All of the public API is in the **apl** namespace.

### <a name="Plugin">Plugin</a>
Plugins are provided as shared libraries and can be loaded into the program at runtime.

A plugin can consist of [features](#Feature) (functions) and [classes](#Class).

Even though C++ types can be passed between plugin and main application, you should limit yourself to C types to avoid
ABI incompatibility.
---
### <a name="Feature">Feature</a>
A feature is a function that has a feature name and belongs to a feature group. A feature also has a return type
and an argument list.  
For each registered feature there is one PluginFeatureInfo object in the plugin.

Features can be registered with the following macro:

    A_PLUGIN_REGISTER_FEATURE(returnType, featureGroup, featureName, [argumentList]...)
    {
        // function body
    }

for example:

    #include "APluginLibrary/pluginapi.h" // this must be included
    
    A_PLUGIN_REGISTER_FEATURE(int, group1, feature1, int x1, double y, const char\* x2)
    {
        std::cout << x2 << std::end;
        return x1 + y;
    }
---
### <a name="Class">Class</a>
A class is a normal C++ class which is derived from an interface and implements its virtual methods.  
For each registered class there is one PluginClassInfo object in the plugin.

There is no guarantee that plugins with different interfaces but the same interface names will be loaded.

Classes can be registered with the following macro:

    A_PLUGIN_REGISTER_CLASS(interfaceName, className);
    
for example:
    
    // interface.h (there should be include guards)
    
    class Interface
    {
    public:
        Interface() = default;
        virtual ~Interface() = default;
    
        virtual int function1(int, int) = 0;
        virtual int function2(int) = 0;
    };
<!-- tsk -->
    // whatever_plugin.cpp
    #include "APluginLibrary/pluginapi.h" // this must be included
    #include "path/to/interface.h"
    
    class Implementation : public Interface
    {
    public:
        int function1(int x1, int x2) override;
        int function2(int x) override;
    };
    
    A_PLUGIN_REGISTER_CLASS(Interface, Implementation);
    
    int Implementation::function1(int x1, int x2) {
        return x1 * x2;
    }
    int Implementation::function2(int x) {
        return x * x;
    }
---

### <a name="PluginManager">PluginManager</a>
With the PluginManager a plugin and all its features and classes can simply be loaded using the load method and
unloaded using one of the unload methods, or if the PluginManager is deleted.

You can query how many and which plugins are loaded in this PluginManager.
You can also get all loaded features and classes, where you can filter the features (with PluginFeatureFilter)
by feature group, feature name, return type or argument list and the classes (with PluginClassFilter)
by interface name or class name.

There can be multiple instances of PluginManager with different plugins.

---
### <a name="Known_Problems">Known Problems</a>

- There is no API version check yet.

- Interfaces only get compared by names not by types.

- If you load 2 or more Plugins the same time, which contain one (or more) class(es) with the same name under macOS, when
  APluginLibrary was compiled with GCC (tested 8.3 and 9.2), every call to "createInstance" and "deleteInstance"
  of this class(es) will call the constructor of the class in the first loaded plugin. This is independent from the
  PluginManager you are using.  
  **This could only be reproduced under macOS with GCC!**
