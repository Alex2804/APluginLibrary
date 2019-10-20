cmake_minimum_required(VERSION 2.8.0)
project(dlfcn-win32 VERSION 0.1 LANGUAGES C)

add_library(dlfcn-win32 STATIC dlfcn-win32/dlfcn.h dlfcn-win32/dlfcn.c)
target_include_directories(dlfcn-win32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})