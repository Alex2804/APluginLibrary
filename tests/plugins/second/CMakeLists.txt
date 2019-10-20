cmake_minimum_required(VERSION 3.1.0)

add_library(second_plugin SHARED src/main.cpp)
set_target_properties(second_plugin PROPERTIES PREFIX "")