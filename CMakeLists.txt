# CMake build script for Gfal2 Python bindings
project(gfal2-bindings)
cmake_minimum_required(VERSION 3.12)

# Build type
set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "build type")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Load modules and definitions
include(DefineInstallationPaths REQUIRED)
include(MacroCopyFile REQUIRED)
include(ReleaseDebugAutoFlags REQUIRED)
include(CMakeGeneratePkgConfig REQUIRED)
set(Boost_NO_BOOST_CMAKE ON)

# Define version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 13)
set(VERSION_PATCH 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
add_definitions(-DGFAL2_PYTHON_VERSION="${VERSION_STRING}")

# Find build packages
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(Boost COMPONENTS "python3${Python_VERSION_MINOR}")
if(NOT Boost_LIBRARIES)
  # Failed to find a specific Python3x version, fallback to Python3
  find_package(Boost COMPONENTS "python3" REQUIRED)
endif()
find_package(GLIB2 REQUIRED)
find_package(GTHREAD2 REQUIRED)
find_package(GFAL2 REQUIRED)

add_definitions(-D_FILE_OFFSET_BITS=64 )
add_subdirectory(src)

if (NOT SKIP_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

if (BUILDDOCS)
  # Install docs and license
  install(FILES "LICENSE" "RELEASE-NOTES" "README.md" "readme.html"
            DESTINATION ${DOC_INSTALL_DIR})

  # Install examples
  add_subdirectory(doc)
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/example/python/"
            DESTINATION ${DOC_INSTALL_DIR}/examples)
endif()
