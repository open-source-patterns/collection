# setup → modules → targets (debug flags, sanitizers) → install → export → package config → tests
# 1️⃣ Project Setup
cmake_minimum_required(VERSION 3.31)
project(collection VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 2️⃣ Include modules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 3️⃣ Define targets
add_library(collection STATIC src/array.c src/dictionary.c)
add_library(collection::collection ALIAS collection)

target_include_directories(collection
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> # for generated headers
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> # public headers
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # consumers find installed headers
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src) # private headers for internal use

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang") AND NOT WIN32)
    message(STATUS "[Debug Flags] Debug build with supported compiler — enabling -Wall -Werror") # Set CMake Environment field: MallocNanoZone=0

    add_compile_options(-Wall -Werror)
endif ()

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF) # -DENABLE_SANITIZERS=ON
if (ENABLE_SANITIZERS AND NOT WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "[Sanitizers] ENABLE_SANITIZERS is ON — adding AddressSanitizer and UndefinedBehaviorSanitizer flags")

    add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif ()

# 4️⃣ Install rules
install(TARGETS collection
        EXPORT collection-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT collection_RunTime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT collection_RunTime NAMELINK_COMPONENT collection_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT collection_Development
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 5️⃣ Export targets
install(EXPORT collection-targets
        FILE collection-targets.cmake
        NAMESPACE collection::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 6️⃣ Package config
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake.in"
"@PACKAGE_INIT@\n\n\
include(\"\${CMAKE_CURRENT_LIST_DIR}/collection-targets.cmake\")\n\n\
check_required_components(collection)\n")

configure_package_config_file(
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 7️⃣ Tests (Set CMake Options -DBUILD_TESTS=ON)
option(BUILD_TESTS "Build Tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
