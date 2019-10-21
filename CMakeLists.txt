cmake_minimum_required(VERSION 3.1.0)
project(APluginLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
        include/APluginLibrary/alibraryloader.h include/APluginLibrary/alibraryloader.tpp
        include/APluginLibrary/apluginapi.h
        include/APluginLibrary/aplugininfos.h
    )
set(SOURCES
        src/alibraryloader.cpp
    )

if(WIN32)
    add_subdirectory(libs/dlfcn)
    list(APPEND CMAKE_DL_LIBS dlfcn-win32)
endif()

add_library(APluginLibrary STATIC "${SOURCES}" "${HEADERS}")
target_include_directories(APluginLibrary PRIVATE include/APluginLibrary)
target_include_directories(APluginLibrary PUBLIC include)
target_link_libraries(APluginLibrary "${CMAKE_DL_LIBS}")

if(${APluginLibraryTest})
    add_subdirectory(tests)
endif()