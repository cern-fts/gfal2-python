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
if (PYTHON2)
   
    if(RHEL STREQUAL "6")
      set(PYTHON2_VER "")
     elseif(RHEL STREQUAL "7")
      set(PYTHON2_VER "")
     else()
      set(PYTHON2_VER "_${Python2_VERSION_MAJOR}.${Python2_VERSION_MINOR}")
  
    set(PYTHON2_LIBRARIES "PYTHON_LIBRARIES${PYTHON2_VER}"
    set(PYTHON2_INCLUDEPATH "PYTHON_INCLUDE_PATH${PYTHON2_VER}"
    set(PYTHON2_SITE_PACKAGES "PYTHON_SITE_PACKAGES${PYTHON2_VER}"

    add_library(gfal-py MODULE ${src_python})
    target_link_libraries(gfal-py
        ${Boost_LIBRARIES} ${GLIB2_LIBRARIES}  ${GTHREAD2_LIBRARIES}
        ${GFAL2_LIBRARIES} ${PYTHON2_LIBRARIES}
    )
    set_target_properties(gfal-py
        PROPERTIES
            CLEAN_DIRECT_OUTPUT 1
            PREFIX ""
            OUTPUT_NAME gfal2
            LIBRARY_OUTPUT_DIRECTORY python2
    )
    set_property(TARGET gfal-py
        APPEND PROPERTY INCLUDE_DIRECTORIES ${PYTHON2_INCLUDE_PATH}
    ) 

    install(TARGETS gfal-py 
	    RUNTIME		DESTINATION ${CMAKE_INSTALL_PREFIX}
	    LIBRARY		DESTINATION ${PYTHON2_SITE_PACKAGES}
    )
endif ()

# If available, Python3
if (Boost_PYTHON_3_LIBRARY)
    add_library(gfal-py3 MODULE ${src_python})
    target_link_libraries(gfal-py3
        ${Boost_PYTHON_3_LIBRARY} ${GLIB2_LIBRARIES} ${GTHREAD_LIBRARIES}
        ${GFAL2_LIBRARIES} ${PYTHON_LIBRARIES_3.6}
    )
    set_target_properties(gfal-py3
        PROPERTIES
            CLEAN_DIRECT_OUTPUT 1
            PREFIX ""
            OUTPUT_NAME gfal2
            LIBRARY_OUTPUT_DIRECTORY python3
    )
    set_property(TARGET gfal-py3
        APPEND PROPERTY INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_PATH_3.6}
    )

    install(TARGETS gfal-py3
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES_3.6}
    )
endif ()

