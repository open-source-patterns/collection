# Project Setup
cmake_minimum_required(VERSION 3.23)
project(collection VERSION 1.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define library target
add_library(collection STATIC
        src/array.c
        src/dictionary.c)

if(WIN32)
    target_sources(collection PRIVATE src/platform/win/mutex.c)
else()
    target_sources(collection PRIVATE src/platform/posix/mutex.c)
endif()

add_library(collection::collection ALIAS collection)

# Target include paths
target_include_directories(collection
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Compiler warnings (cross-platform, strict)
target_compile_options(collection
        PRIVATE
            $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Werror -fsanitize=address,undefined,leak -fno-omit-frame-pointer>
            $<$<C_COMPILER_ID:MSVC>:/W4 /WX>)

# Install rules
include(GNUInstallDirs)

install(TARGETS collection
        EXPORT collection-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT CollectionDevelopment)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets for find_package()
install(EXPORT collection-targets
        FILE collection-targets.cmake
        NAMESPACE collection::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# Package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/collection-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# Package version file
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# Tests
option(COLLECTION_BUILD_TESTS "Build Collection Tests" ${PROJECT_IS_TOP_LEVEL})
if (COLLECTION_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

# Examples
option(COLLECTION_BUILD_EXAMPLES "Build Collection Examples" OFF)
if (COLLECTION_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
