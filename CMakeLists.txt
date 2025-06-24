# A: Project Setup
cmake_minimum_required(VERSION 3.31)
project(collection VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# B: Source library
# 1. Library Definition, Target properties
add_library(collection STATIC src/array.c src/dictionary.c)
add_library(collection::collection ALIAS collection)
target_compile_features(collection PRIVATE c_std_11)

# 2. Public Header Paths
target_include_directories(collection PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Include modules for install and config
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# C: Configuration
# 1. Generate Template
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake.in"
"@PACKAGE_INIT@\n\n\
include(\"\${CMAKE_CURRENT_LIST_DIR}/collection-targets.cmake\")\n\n\
check_required_components(collection)\n")

# 2. Configure
configure_package_config_file(
        "${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake.in"
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/collection
        NO_SET_AND_CHECK_MACRO) # share/

# 3. Write version file
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

# D: Installation steps
# 1. Install headers
install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/collection
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 2. Install library and export targets
install(TARGETS collection
        EXPORT collection-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT collection_RunTime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT collection_RunTime
            NAMELINK_COMPONENT collection_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT collection_Development)

# 3. Install the export set
install(EXPORT collection-targets
        FILE collection-targets.cmake
        NAMESPACE collection::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/collection)

# 4. Install config and version files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake # Install Config
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake # Install version
        DESTINATION ${CMAKE_INSTALL_DATADIR}/collection)

# E: Development Build Options (Set CMake Options -DENABLE_SANITIZERS=ON)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if (ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    message(STATUS "[Sanitizers] ENABLE_SANITIZERS is ON — adding AddressSanitizer and UndefinedBehaviorSanitizer flags")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")

    target_compile_options(collection PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all)
    target_link_options(collection PRIVATE -fsanitize=address,undefined)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU" AND NOT WIN32) # Set CMake Environment field: MallocNanoZone=0
    message(STATUS "[Debug Flags] Debug build with supported compiler — enabling -Werror, -Wall, -std=c11")
    target_compile_options(collection PRIVATE -Werror -Wall -std=c11)
endif ()

# F: Tests (Set CMake Options -DBUILD_TESTS=ON)
option(BUILD_TESTS "Build Tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
