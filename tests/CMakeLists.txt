cmake_minimum_required(VERSION 2.8.12)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(libs/googletest
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(SOURCES
        src/test_libraryloader.cpp
        src/test_pluginapi.cpp
        src/test_plugin.cpp
        src/test_pluginmanager.cpp
        src/test_pluginmanagerobserver.cpp)

add_executable(APluginLibraryTest ${SOURCES})
target_link_libraries(APluginLibraryTest APluginLibrary gtest_main)
add_test(NAME APluginLibraryTest COMMAND APluginLibraryTest)

add_subdirectory(libraries/first)
add_subdirectory(libraries/second)
add_dependencies(APluginLibraryTest first_lib)
add_dependencies(APluginLibraryTest second_lib)

add_subdirectory(plugins/first)
add_subdirectory(plugins/second)
add_subdirectory(plugins/third)
add_subdirectory(plugins/fourth)
add_subdirectory(plugins/fifth)
add_subdirectory(plugins/sixth)
add_subdirectory(plugins/seventh)
add_dependencies(APluginLibraryTest first_plugin)
add_dependencies(APluginLibraryTest second_plugin)
add_dependencies(APluginLibraryTest third_plugin)
add_dependencies(APluginLibraryTest fourth_plugin)
add_dependencies(APluginLibraryTest fifth_plugin)
add_dependencies(APluginLibraryTest sixth_plugin)
add_dependencies(APluginLibraryTest seventh_plugin)
