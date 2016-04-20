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

set(Boost_NO_BOOST_CMAKE ON)

find_package(Boost COMPONENTS python REQUIRED)
find_package(PythonEasy REQUIRED)

message("Python: ${PYTHON_EXECUTABLE}")

#define PROJECT vars
set(PROJECT_NAME_MAIN "gfal2-python")
set(OUTPUT_NAME_MAIN "gfal2")


# EPYDOC definition for EPEL5 support
if (NOT SKIP_DOC)
    set(EPYDOC_MODULE_PATH "${CMAKE_BINARY_DIR}/src/gfal2.so")
    set(EPYDOC_MODULE_URL  "http://dmc.web.cern.ch/projects/gfal2-python")
    set(EPYDOC_MODULE_NAME "gfal2")

    include(MacroAddepydoc REQUIRED)
endif (NOT SKIP_DOC)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 8)
set(VERSION_PATCH 5)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
add_definitions(-DGFAL2_PYTHON_VERSION="${VERSION_STRING}")

# libs checks
find_package (GLIB2 REQUIRED)
find_package (GTHREAD2 REQUIRED)

# GFAL2
find_package(GFAL2 REQUIRED)

# Install doc license
if (NOT SKIP_DOC)
install(FILES "LICENSE" "RELEASE-NOTES" "README" "readme.html"
                        DESTINATION ${DOC_INSTALL_DIR})
endif (NOT SKIP_DOC)

add_definitions( -D_FILE_OFFSET_BITS=64 )

add_subdirectory (src)

if (NOT SKIP_TESTS)
  enable_testing()
  add_subdirectory (test)
endif (NOT SKIP_TESTS)

if (NOT SKIP_DOC)
  add_subdirectory(doc)
  # Install examples
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/example/python/"
            DESTINATION ${DOC_INSTALL_DIR}/examples)
endif (NOT SKIP_DOC)

