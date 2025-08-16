# 1️⃣ Project Setup
cmake_minimum_required(VERSION 3.23)
project(collection VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 2️⃣ Define targets
add_library(collection STATIC
        src/array.c
        src/dictionary.c)
add_library(collection::collection ALIAS collection)

target_include_directories(collection
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

# 3️⃣ Install rules
include(GNUInstallDirs)

install(TARGETS collection
        EXPORT collection-config
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT collection_RunTime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT collection_RunTime NAMELINK_COMPONENT collection_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT collection_Development)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 4️⃣ Export targets
install(EXPORT collection-config
        FILE collection-config.cmake
        NAMESPACE collection::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 6️⃣ Tests
option(BUILD_TESTS "Build Tests" OFF)
if (BUILD_TESTS)
    message(STATUS "[Tests] BUILD_TESTS is ON — configuring test targets")
    enable_testing()
    add_subdirectory(test)
endif ()
