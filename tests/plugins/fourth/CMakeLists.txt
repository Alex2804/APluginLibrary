cmake_minimum_required(VERSION 3.1.0)

add_library(fourth_plugin SHARED src/main.cpp)
target_include_directories(fourth_plugin PRIVATE ../../../SDK)
set_target_properties(fourth_plugin PROPERTIES PREFIX "")