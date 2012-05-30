
message("python path : ${PYTHON_INCLUDE_PATH}")

INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" ${PYTHON_INCLUDE_PATH} 
						${GLIB2_PKG_INCLUDE_DIRS} ${GTHREAD2_PKG_INCLUDE_DIRS} ${GFAL2_PKG_INCLUDE_DIRS})

FILE(GLOB src_python "*.cpp")

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)


ADD_LIBRARY(gfal-py SHARED ${src_python} )
TARGET_LINK_LIBRARIES(gfal-py ${Boost_LIBRARIES} ${GFAL2_PKG_LIBRARIES}
 ${GLIB2_PKG_LIBRARIES} ${GTHREAD2_PKG_LIBRARIES} ${PYTHON_LIBRARIES})


set_target_properties(gfal-py	 	PROPERTIES
								CLEAN_DIRECT_OUTPUT 1
								PREFIX ""
								OUTPUT_NAME ${OUTPUT_NAME_MAIN}
)


install(TARGETS gfal-py 
	    RUNTIME		DESTINATION ${CMAKE_INSTALL_PREFIX}
	    LIBRARY		DESTINATION ${PYTHON_SITE_PACKAGES} )