# setup → modules → targets (debug flags, sanitizers) → install → export → package config → tests
# 1️⃣ Project Setup
cmake_minimum_required(VERSION 3.31)
project(collection VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 2️⃣ Define targets
add_library(collection STATIC)
target_sources(collection
        PRIVATE
            src/array.c
            src/dictionary.c
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS include/collection
            FILES
                include/collection/array.h
                include/collection/dictionary.h)
add_library(collection::collection ALIAS collection)

#target_include_directories(collection
#        PUBLIC
#            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> # generated headers
#            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> # public headers
#            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # installed headers
#        PRIVATE
#            ${CMAKE_CURRENT_SOURCE_DIR}/src) # private headers


# Use CI flags
# cmake -B build \
#  -DCMAKE_C_FLAGS="-Wall -Werror -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer" \
#  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined" \
#  -DCMAKE_BUILD_TYPE=Debug

#option(STRICT_GLOBAL "Apply strict compile options globally" OFF) # -DSTRICT_GLOBAL=ON
#if (STRICT_GLOBAL AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang") AND NOT WIN32)
#    message(STATUS "[Debug Flags] Debug build with supported compiler — enabling -Wall -Werror")
#    add_compile_options(-Wall -Werror)
#endif ()
#
#option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF) # -DENABLE_SANITIZERS=ON
#if (ENABLE_SANITIZERS AND NOT WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
#    message(STATUS "[Sanitizers] ENABLE_SANITIZERS is ON — adding AddressSanitizer and UndefinedBehaviorSanitizer flags")
#    add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
#    add_link_options(-fsanitize=address -fsanitize=undefined)
#endif ()

# 3️⃣ Install rules
include(GNUInstallDirs)

install(TARGETS collection
        EXPORT collection-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT collection_RunTime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT collection_RunTime NAMELINK_COMPONENT collection_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT collection_Development
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 4️⃣ Export targets
install(EXPORT collection-targets
        FILE collection-targets.cmake
        NAMESPACE collection::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 5️⃣ Package config
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT)

configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/collection-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/collection-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 6️⃣ Tests
option(BUILD_TESTS "Build Tests" OFF) # -DBUILD_TESTS=ON
if (BUILD_TESTS)
    message(STATUS "[Tests] BUILD_TESTS is ON — configuring test targets")
    enable_testing()
    add_subdirectory(test)
endif ()
