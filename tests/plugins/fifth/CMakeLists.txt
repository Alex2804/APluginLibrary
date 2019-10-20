cmake_minimum_required(VERSION 3.1.0)

add_library(fifth_plugin SHARED src/main.cpp)
set_target_properties(fifth_plugin PROPERTIES PREFIX "")