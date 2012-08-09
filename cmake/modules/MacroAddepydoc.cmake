##
## Doxygen macro, allow Doxygen generation from cmake
## do a ""make doc" for the generation


macro(addEpydocGeneration EPYDOC_CONFIG_LOCATION)


        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${EPYDOC_CONFIG_LOCATION} ${CMAKE_CURRENT_BINARY_DIR}/epydoc_config @ONLY)

	add_custom_target(doc
        epydoc --config ${CMAKE_CURRENT_BINARY_DIR}/epydoc_config -v
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generate API documentation with epydoc" VERBATIM
	)


endmacro(addEpydocGeneration EPYDOC_CONFIG_LOCATION)
