cmake_minimum_required(VERSION 3.1.0)

add_library(third_plugin SHARED src/main.cpp)
target_include_directories(third_plugin PRIVATE ../../../SDK)
set_target_properties(third_plugin PROPERTIES PREFIX "")