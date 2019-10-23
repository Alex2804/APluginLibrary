cmake_minimum_required(VERSION 3.1.0)

add_library(second_plugin SHARED src/main.cpp)
target_include_directories(second_plugin PRIVATE ../../../SDK)
set_target_properties(second_plugin PROPERTIES PREFIX "")