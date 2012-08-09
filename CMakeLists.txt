## cmake build script for gfal2 bindings
project (gfal2-bindings)
cmake_minimum_required (VERSION 2.6)
message("cmake source dir : ${CMAKE_SOURCE_DIR}")
# build type
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")
# global options

# load module
include(DefineInstallationPaths REQUIRED)
include(MacroCopyFile REQUIRED)
include(ReleaseDebugAutoFlags REQUIRED)
include(CMakeGeneratePkgConfig REQUIRED)
include(MacroAddepydoc REQUIRED)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS python)

find_package(PythonEasy REQUIRED)

#enable testing
enable_testing()

#define PROJECT vars
set(PROJECT_NAME_MAIN "gfal2-python")
set(OUTPUT_NAME_MAIN "gfal2")

set(EPYDOC_MODULE_PATH "${CMAKE_BINARY_DIR}/src/gfal2.so")

set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})



# libs checks
pkg_check_modules(GLIB2_PKG REQUIRED glib-2.0)
pkg_check_modules(GTHREAD2_PKG REQUIRED gthread-2.0)

pkg_check_modules(GFAL2_PKG REQUIRED gfal2)
pkg_check_modules(GFAL2_TRANSFER_PKG REQUIRED gfal_transfer)



#install doc license
#install(FILES "LICENSE" "README" "RELEASE-NOTES" "VERSION" "DESCRIPTION"
#			DESTINATION ${DOC_INSTALL_DIR})

add_subdirectory (src)
add_subdirectory(doc)
add_subdirectory (test)
