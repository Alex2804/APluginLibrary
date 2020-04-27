cmake_minimum_required(VERSION 3.1.0)

enable_language(C)

add_library(first_plugin SHARED src/main.c)
target_include_directories(first_plugin PRIVATE ../../../SDK)
set_target_properties(first_plugin PROPERTIES PREFIX "" C_STANDARD 90 C_EXTENSIONS off)
