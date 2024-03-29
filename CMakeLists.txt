cmake_minimum_required(VERSION 3.1.0)
project(APluginLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set_property(GLOBAL PROPERTY CXX_EXTENSIONS FALSE)

set(HEADERS
        include/APluginLibrary/libraryloader.h
        include/APluginLibrary/plugin.h src/private/pluginprivate.h
        include/APluginLibrary/pluginmanager.h src/private/pluginmanagerprivate.h
        include/APluginLibrary/pluginmanagerobserver.h)
set(SOURCES
        src/libraryloader.cpp include/APluginLibrary/implementation/libraryloader.tpp
        src/plugin.cpp src/private/src/pluginprivate.cpp
        src/pluginmanager.cpp src/private/src/pluginmanagerprivate.cpp
        src/pluginmanagerobserver.cpp)

set(SDK_HEADERS
        SDK/APluginSDK/pluginapi.h)
set(SDK_SOURCES
        SDK/APluginSDK/src/cpp/pluginapi.cpp)

set(PUBLIC_INCLUDE_DIRECTORIES
        include
        SDK
        ${CMAKE_CURRENT_BINARY_DIR}/exports)
set(PRIVATE_INCLUDE_DIRECTORIES
        libs/tinydir)

if(WIN32)
    add_subdirectory(libs/dlfcn)
    list(APPEND CMAKE_DL_LIBS dlfcn-win32)
endif()

add_library(APluginLibrary STATIC ${HEADERS} ${SOURCES} ${SDK_HEADERS} ${SDK_SOURCES})
include(GenerateExportHeader)
generate_export_header(APluginLibrary
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/exports/APluginLibrary/apluginlibrary_export.h)
target_include_directories(APluginLibrary
        PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES}
        PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
target_compile_definitions(APluginLibrary PUBLIC PRIVATE_APLUGINSDK_INTEGRATED_PLUGIN APLUGINSDK_EXCLUDE_IMPLEMENTATION PRIVATE_APLUGINSDK_DONT_EXPORT_API)
target_link_libraries(APluginLibrary "${CMAKE_DL_LIBS}")

if(${APluginLibraryTest})
    enable_testing()
    add_subdirectory(tests)
    target_compile_definitions(APluginLibrary PUBLIC APLUGINLIBRARY_TEST)
endif()
