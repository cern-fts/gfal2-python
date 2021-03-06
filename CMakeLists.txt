## cmake build script for gfal2 bindings
project (gfal2-bindings)
cmake_minimum_required (VERSION 2.6)
message("cmake source dir : ${CMAKE_SOURCE_DIR}")
# build type
set(CMAKE_BUILD_TYPE "RelWithDebInfo" 
        CACHE STRING "build type")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")
# global options

# Find out details if we're on a redhat style platform
execute_process (COMMAND rpm -E "%{?fedora}" OUTPUT_VARIABLE FEDORA OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process (COMMAND rpm -E "%{?rhel}" OUTPUT_VARIABLE RHEL OUTPUT_STRIP_TRAILING_WHITESPACE)


# load module
include(DefineInstallationPaths REQUIRED)
include(MacroCopyFile REQUIRED)
include(ReleaseDebugAutoFlags REQUIRED)
include(CMakeGeneratePkgConfig REQUIRED)

set(Boost_NO_BOOST_CMAKE ON)

find_package(PythonEasy REQUIRED)

message("Python: ${PYTHON_EXECUTABLE}")

if (PYTHON_EXECUTABLE)
    if(RHEL STREQUAL "6")
      find_package(Boost COMPONENTS python REQUIRED)
    elseif(RHEL STREQUAL "7")
      find_package(Boost COMPONENTS python REQUIRED)
    else()
      find_package(Boost COMPONENTS python27 REQUIRED)
    endif()
endif(PYTHON_EXECUTABLE)


if (PYTHON3_CURRENT_VERSION)
    message("Python 3 installed! ${PYTHON3_CURRENT_VERSION}")

    string(REPLACE "." "" Boost_PYTHON_3_SUFFIX "${PYTHON3_CURRENT_VERSION}")

    find_library(Boost_PYTHON_3_LIBRARY
        NAMES boost_python3 boost_python${Boost_PYTHON_3_SUFFIX}  boost_python-py${Boost_PYTHON_3_SUFFIX}
        HINTS
            ${Boost_LIBRARYDIR}
            "/usr/lib"
            "/usr/lib64"
    )
    if (Boost_PYTHON_3_LIBRARY)
        message("Boost Python3 library found! ${Boost_PYTHON_3_LIBRARY}")
    else()
        message("Boost Python3 library not found")
    endif ()
endif ()

#define PROJECT vars
set(PROJECT_NAME_MAIN "gfal2-python")

# EPYDOC definition for EPEL5 support
if (NOT SKIP_DOC)
    set(EPYDOC_MODULE_PATH "${CMAKE_BINARY_DIR}/src/python2/gfal2.so")
    set(EPYDOC_MODULE_URL  "http://dmc.web.cern.ch/projects/gfal2-python")
    set(EPYDOC_MODULE_NAME "gfal2")

    include(MacroAddepydoc REQUIRED)
endif (NOT SKIP_DOC)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 10)
set(VERSION_PATCH 1)
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

