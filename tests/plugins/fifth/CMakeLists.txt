cmake_minimum_required(VERSION 3.1.0)

add_library(fifth_plugin SHARED src/main.cpp)
target_include_directories(fifth_plugin PRIVATE ../../../SDK)
set_target_properties(fifth_plugin PROPERTIES PREFIX "")