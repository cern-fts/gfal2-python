include_directories(${Boost_INCLUDE_DIRS}
    ${GLIB2_INCLUDE_DIRS} ${GTHREAD2_INCLUDE_DIRS}
    ${GFAL2_INCLUDE_DIRS}
)

file(GLOB src_python "*.cpp")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

link_directories (
    ${GLIB2_LIBRARY_DIRS}
    ${GFAL2_LIBRARY_DIRS}
)

# Python 2
if (Boost_LIBRARIES)
     
    if(RHEL STREQUAL "7")
      set(PYTHON2_VER "")
    else()
      set(PYTHON2_VER "_${PYTHON_CURRENT_VERSION}")
    endif()
    
    add_library(gfal-py MODULE ${src_python})
    target_link_libraries(gfal-py
        ${Boost_LIBRARIES} ${GLIB2_LIBRARIES}  ${GTHREAD2_LIBRARIES}
        ${GFAL2_LIBRARIES}
    )
    set_target_properties(gfal-py
        PROPERTIES
            CLEAN_DIRECT_OUTPUT 1
            PREFIX ""
            OUTPUT_NAME gfal2
            LIBRARY_OUTPUT_DIRECTORY python2
    )
    set_property(TARGET gfal-py
        APPEND PROPERTY INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_PATH${PYTHON2_VER}}
    )
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set_property(TARGET gfal-py APPEND PROPERTY LINK_FLAGS "-undefined dynamic_lookup")
    endif()

    install(TARGETS gfal-py 
	    RUNTIME		DESTINATION ${CMAKE_INSTALL_PREFIX}
	    LIBRARY		DESTINATION ${PYTHON_SITE_PACKAGES${PYTHON2_VER}}
    )
endif ()

# Python3
if (Boost_PYTHON_3_LIBRARY)

    set(PYTHON3_VER "_${PYTHON3_CURRENT_VERSION}")
    message(STATUS "Python${PYTHON3_CURRENT_VERSION} include: ${PYTHON_INCLUDE_PATH${PYTHON3_VER}}")
    message(STATUS "Python${PYTHON3_CURRENT_VERSION} libraries: ${PYTHON_LIBRARIES${PYTHON3_VER}}")
    message(STATUS "Python${PYTHON3_CURRENT_VERSION} site-packages: ${PYTHON_SITE_PACKAGES${PYTHON3_VER}}")

    add_library(gfal-py3 MODULE ${src_python})
    target_link_libraries(gfal-py3
        ${Boost_PYTHON_3_LIBRARY} ${GLIB2_LIBRARIES} ${GTHREAD_LIBRARIES}
        ${GFAL2_LIBRARIES}
    )
    set_target_properties(gfal-py3
        PROPERTIES
            CLEAN_DIRECT_OUTPUT 1
            PREFIX ""
            OUTPUT_NAME gfal2
            LIBRARY_OUTPUT_DIRECTORY python3
    )
    set_property(TARGET gfal-py3
        APPEND PROPERTY INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_PATH${PYTHON3_VER}}
    )
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set_property(TARGET gfal-py3 APPEND PROPERTY LINK_FLAGS "-undefined dynamic_lookup")
    endif()

    install(TARGETS gfal-py3
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES${PYTHON3_VER}}
    )
endif ()

