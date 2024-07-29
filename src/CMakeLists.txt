set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
file(GLOB src "*.cpp")

add_library(gfal-py3 MODULE ${src})
target_link_libraries(gfal-py3
    ${Boost_LIBRARIES}
    ${GLIB2_LIBRARIES}
    ${GTHREAD_LIBRARIES}
    ${GFAL2_LIBRARIES}
)
target_include_directories(gfal-py3 PRIVATE
    ${Python_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${GLIB2_INCLUDE_DIRS}
    ${GFAL2_INCLUDE_DIRS}
)
set_target_properties(gfal-py3
    PROPERTIES
        CLEAN_DIRECT_OUTPUT 1
        PREFIX ""
        OUTPUT_NAME gfal2
        LIBRARY_OUTPUT_DIRECTORY python3
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set_property(TARGET gfal-py3 APPEND PROPERTY LINK_FLAGS "-undefined dynamic_lookup")
endif()

install(TARGETS gfal-py3
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${Python_SITEARCH})
