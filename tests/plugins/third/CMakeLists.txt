cmake_minimum_required(VERSION 3.1.0)

add_library(third_plugin SHARED src/main.cpp)
set_target_properties(third_plugin PROPERTIES PREFIX "")