cmake_minimum_required(VERSION 3.1.0)

add_library(sixth_plugin SHARED src/main.cpp)
set_target_properties(sixth_plugin PROPERTIES PREFIX "")