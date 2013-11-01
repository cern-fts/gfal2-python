## cmake build script for gfal2 bindings
project (gfal2-bindings)
cmake_minimum_required (VERSION 2.6)
message("cmake source dir : ${CMAKE_SOURCE_DIR}")
# build type
set(CMAKE_BUILD_TYPE "RelWithDebInfo" 
        CACHE STRING "build type")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")
# global options

# load module
include(DefineInstallationPaths REQUIRED)
include(MacroCopyFile REQUIRED)
include(ReleaseDebugAutoFlags REQUIRED)
include(CMakeGeneratePkgConfig REQUIRED)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS python)

find_package(PythonEasy REQUIRED)

#enable testing
enable_testing()

#define PROJECT vars
set(PROJECT_NAME_MAIN "gfal2-python")
set(OUTPUT_NAME_MAIN "gfal2")


# EPYDOC definition for EPEL5 support
if (NOT SKIP_DOC)
    set(EPYDOC_MODULE_PATH "${CMAKE_BINARY_DIR}/src/gfal2.so")
    set(EPYDOC_MODULE_URL "https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2-python")
    set(EPYDOC_MODULE_NAME "gfal2")

    include(MacroAddepydoc REQUIRED)
endif (NOT SKIP_DOC)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 3)
set(VERSION_PATCH 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})



# libs checks
pkg_check_modules(GLIB2_PKG REQUIRED glib-2.0)
pkg_check_modules(GTHREAD2_PKG REQUIRED gthread-2.0)

pkg_check_modules(GFAL2_PKG REQUIRED gfal2)
pkg_check_modules(GFAL2_TRANSFER_PKG REQUIRED gfal_transfer)



#install doc license
install(FILES "LICENSE" "RELEASE-NOTES" "README"
                        DESTINATION ${DOC_INSTALL_DIR})

add_definitions( -D_FILE_OFFSET_BITS=64 )

add_subdirectory (src)
add_subdirectory (test)

if (NOT SKIP_DOC)
	add_subdirectory(doc)
endif (NOT SKIP_DOC)

