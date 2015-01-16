#
# This module detects if SRM-IFCE is installed and determines where the
# include files and libraries are.
#
# This code sets the following variables:
# 
# GFAL2_LIBRARIES   = full path to the SRM-IFCE libraries
# GFAL2_INCLUDE_DIR = include dir to be used when using the SRM-IFCE library
# GFAL2_FOUND       = set to true if SRM-IFCE was found successfully
#
# GFAL2_LOCATION
#   setting this enables search for SRM-IFCE libraries / headers in this location

# ----------------------------------------------------- 
# Try with pkgconfig first
# -----------------------------------------------------

pkg_check_modules(GFAL2_PKG gfal2>=2.8.1)
pkg_check_modules(GFAL2_TRANSFER_PKG gfal_transfer>=2.8.1)

if (GFAL2_PKG_FOUND)
    set (GFAL2_INCLUDE_DIR "${GFAL2_PKG_INCLUDE_DIRS}" "${GFAL2_TRANSFER_PKG_INCLUDE_DIRS}")
    set (GFAL2_LIBRARIES "${GFAL2_PKG_LIBRARIES}" "${GFAL2_TRANSFER_PKG_LIBRARIES}")
    set (GFAL2_CFLAGS "${GFAL2_PKG_CFLAGS} ${GFAL2_TRANSFER_PKG_FLAGS}")
else ()
    # GFAL2 Libraries
    find_library(GFAL2_MAIN_LIBRARY
        NAMES libgfal2.so
        HINTS ${GFAL2_LOCATION}/lib ${GFAL2_LOCATION}/lib64 ${GFAL2_LOCATION}/lib32 ${STAGE_DIR}/lib ${STAGE_DIR}/lib64
        DOC "The main GFAL2 library"
    )
    
    find_library(GFAL2_TRANSFER_LIBRARY
        NAMES libgfal_transfer.so
        HINTS ${GFAL2_LOCATION}/lib ${GFAL2_LOCATION}/lib64 ${GFAL2_LOCATION}/lib32 ${STAGE_DIR}/lib ${STAGE_DIR}/lib64
        DOC "The transfer GFAL2 library"
    )
    
    set(GFAL2_LIBRARIES ${GFAL2_MAIN_LIBRARY} ${GFAL2_TRANSFER_LIBRARY})
    
    # GFAL2 Include Directories
    find_path(GFAL2_INCLUDE_DIR 
        NAMES gfal_api.h
        HINTS ${GFAL2_LOCATION} ${GFAL2_LOCATION}/include ${GFAL2_LOCATION}/include/* ${STAGE_DIR}/include ${STAGE_DIR}/include
        DOC "GFAL2 include directory"
    )
    if(GFAL2_INCLUDE_DIR)
        message(STATUS "GFAL2 includes found in ${GFAL2_INCLUDE_DIR}")
    endif()
    
    set (GFAL2_CFLAGS "")
endif()

# -----------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set GFAL2_FOUND to TRUE if 
# all listed variables are TRUE
# -----------------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GFAL2 DEFAULT_MSG GFAL2_LIBRARIES GFAL2_INCLUDE_DIR)
mark_as_advanced(GFAL2_LIBRARIES GFAL2_INCLUDE_DIR GFAL2_CFLAGS)
