cmake_minimum_required(VERSION 3.1.0)

add_library(first_plugin SHARED src/main.cpp)
target_include_directories(first_plugin PRIVATE ../../../SDK)
set_target_properties(first_plugin PROPERTIES PREFIX "")
