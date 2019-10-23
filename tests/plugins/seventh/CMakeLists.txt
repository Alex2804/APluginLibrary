cmake_minimum_required(VERSION 3.1.0)

add_library(seventh_plugin SHARED src/main.cpp)
target_include_directories(seventh_plugin PRIVATE ../../../SDK)
set_target_properties(seventh_plugin PROPERTIES PREFIX "")