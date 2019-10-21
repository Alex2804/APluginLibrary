cmake_minimum_required(VERSION 3.1.0)

add_library(seventh_plugin SHARED src/main.cpp)
set_target_properties(seventh_plugin PROPERTIES PREFIX "")