cmake_minimum_required(VERSION 3.1.0)
project(APluginLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
        include/APluginLibrary/libraryloader.h
        include/APluginLibrary/pluginapi.h
        include/APluginLibrary/plugininfos.h
        include/APluginLibrary/plugin.h src/private/pluginprivate.h
        include/APluginLibrary/pluginmanager.h src/private/pluginmanagerprivate.h)
set(SOURCES
        src/libraryloader.cpp include/APluginLibrary/libraryloader.tpp
        src/plugin.cpp
        src/pluginmanager.cpp include/APluginLibrary/pluginmanager.tpp src/private/pluginmanagerprivate.cpp)

if(WIN32)
    add_subdirectory(libs/dlfcn)
    list(APPEND CMAKE_DL_LIBS dlfcn-win32)
endif()

add_library(APluginLibrary STATIC ${HEADERS} ${SOURCES})
target_include_directories(APluginLibrary PUBLIC include)
target_link_libraries(APluginLibrary "${CMAKE_DL_LIBS}")

if(${APluginLibraryTest})
    add_subdirectory(tests)
endif()