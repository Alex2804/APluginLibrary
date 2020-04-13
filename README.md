# APluginLibrary
Simple C++ Plugin Library.

All of the public API is in the **apl** namespace.

### <a name="Plugin">Plugin</a>
Plugins are provided as shared libraries and can be loaded into the program at runtime. The application itself can
contain a plugin (for e.g. for platforms where dynamic shared library loading is not allowed or not supported) which
can be loaded by passing an empty path to the load function.

For more information about plugins and how to write them, please check out
**[APluginSDK](https://github.com/Alex2804/APluginSDK)**.

---
### <a name="PluginManager">PluginManager</a>
With the PluginManager a plugin and all its features and classes can simply be loaded using the load method and
unloaded using one of the unload methods, or if the PluginManager is deleted.

You can query how many and which plugins are loaded in this PluginManager.
You can also get all loaded features and classes, where you can filter the features (with PluginFeatureFilter)
by feature group, feature name, return type, parameter list, parameter list types or parameter list names
and the classes (with PluginClassFilter) by interface name or class name.

There can be multiple instances of PluginManager with different plugins.

---
### <a name="Known_Problems">Known Problems</a>

- There is no API version check yet.

- Class interfaces only gets compared by names not by types.
