# 1️⃣ Project Setup
cmake_minimum_required(VERSION 3.23)
project(collection VERSION 1.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 2️⃣ Define library target
add_library(collection STATIC
        src/Array.c
        src/Dictionary.c)
add_library(collection::collection ALIAS collection)

# 3️⃣ Target include paths
target_include_directories(collection
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

# 4️⃣ Install rules
include(GNUInstallDirs)

install(TARGETS collection
        EXPORT collectionTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT CollectionDevelopment)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 5️⃣ Export targets for find_package()
install(EXPORT collectionTargets
        FILE collectionTargets.cmake
        NAMESPACE collection::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 6️⃣ Package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/collectionConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/collectionConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 7️⃣ Package version file
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/collectionConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/collectionConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/collectionConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/collection)

# 8️⃣ Tests
option(BUILD_TESTS "Build Tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

# 9️⃣ Examples
option(BUILD_EXAMPLES "Build Examples" OFF)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
