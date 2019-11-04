# APluginLibrary
Simple C++ Plugin Library.

All of the public API is in the **apl** namespace.

### <a name="Plugin">Plugin</a>
Plugins are provided as shared libraries and can be loaded into the program at runtime.

For more information about plugins and how to write them, please check out
**[APluginSDK](https://github.com/Alex2804/APluginSDK)**.

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
