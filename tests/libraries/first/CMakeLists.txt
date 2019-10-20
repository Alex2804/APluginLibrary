cmake_minimum_required(VERSION 3.1.0)

add_library(first_lib SHARED src/main.cpp)
set_target_properties(first_lib PROPERTIES PREFIX "")
