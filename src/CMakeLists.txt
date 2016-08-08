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
add_library(gfal-py MODULE ${src_python})
target_link_libraries(gfal-py
    ${Boost_LIBRARIES} ${GLIB2_LIBRARIES}  ${GTHREAD2_LIBRARIES}
    ${GFAL2_LIBRARIES} ${PYTHON_LIBRARIES}
)
set_target_properties(gfal-py
    PROPERTIES
        CLEAN_DIRECT_OUTPUT 1
        PREFIX ""
        OUTPUT_NAME gfal2
        LIBRARY_OUTPUT_DIRECTORY python2
)
set_property(TARGET gfal-py
    APPEND PROPERTY INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_PATH}
) 

install(TARGETS gfal-py 
	    RUNTIME		DESTINATION ${CMAKE_INSTALL_PREFIX}
	    LIBRARY		DESTINATION ${PYTHON_SITE_PACKAGES}
)

# If available, Python3
if (Boost_PYTHON_3_LIBRARY)
    add_library(gfal-py3 MODULE ${src_python})
    target_link_libraries(gfal-py3
        ${Boost_PYTHON_3_LIBRARY} ${GLIB2_LIBRARIES} ${GTHREAD_LIBRARIES}
        ${GFAL2_LIBRARIES} ${PYTHON_LIBRARIES_3}
    )
    set_target_properties(gfal-py3
        PROPERTIES
            CLEAN_DIRECT_OUTPUT 1
            PREFIX ""
            OUTPUT_NAME gfal2
            LIBRARY_OUTPUT_DIRECTORY python3
    )
    set_property(TARGET gfal-py3
        APPEND PROPERTY INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_PATH_3}
    )

    install(TARGETS gfal-py3
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES_3}
    )
endif ()

