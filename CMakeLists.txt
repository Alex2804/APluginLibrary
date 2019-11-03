cmake_minimum_required(VERSION 3.1.0)
project(APluginLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
        include/APluginLibrary/libraryloader.h
        include/APluginLibrary/plugin.h src/private/pluginprivate.h
        include/APluginLibrary/pluginmanager.h src/private/pluginmanagerprivate.h)
set(SOURCES
        src/libraryloader.cpp include/APluginLibrary/implementation/libraryloader.tpp
        src/plugin.cpp
        src/pluginmanager.cpp src/private/pluginmanagerprivate.cpp)

set(SDK_HEADERS
        SDK/APluginLibrary/plugininfos.h)
set(SDK_SOURCES
        )

if(WIN32)
    add_subdirectory(libs/dlfcn)
    list(APPEND CMAKE_DL_LIBS dlfcn-win32)
endif()

add_library(APluginLibrary STATIC ${HEADERS} ${SOURCES} ${SDK_HEADERS} ${SDK_SOURCES})
include(GenerateExportHeader)
generate_export_header(APluginLibrary
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/exports/APluginLibrary/apluginlibrary_export.h)
target_include_directories(APluginLibrary PUBLIC include SDK "${CMAKE_CURRENT_BINARY_DIR}/exports")
target_link_libraries(APluginLibrary "${CMAKE_DL_LIBS}")

if(${APluginLibraryTest})
    add_subdirectory(tests)
    target_compile_definitions(APluginLibrary PUBLIC APLUGINLIBRARY_TEST)
endif()