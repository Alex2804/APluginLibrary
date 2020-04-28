cmake_minimum_required(VERSION 3.1.0)

add_library(seventh_plugin SHARED ../../../SDK/APluginSDK/src/cpp/pluginapi.cpp src/feature.cpp src/class.cpp)
target_compile_definitions(seventh_plugin PRIVATE APLUGINSDK_EXCLUDE_IMPLEMENTATION)
target_include_directories(seventh_plugin PRIVATE ../../../SDK)
set_target_properties(seventh_plugin PROPERTIES PREFIX "")