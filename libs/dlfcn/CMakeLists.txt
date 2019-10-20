cmake_minimum_required(VERSION 3.1.0)
project(dlfcn-win32 LANGUAGES C)

add_library(dlfcn-win32 STATIC dlfcn-win32/dlfcn.h dlfcn-win32/dlfcn.c)
target_include_directories(dlfcn-win32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})