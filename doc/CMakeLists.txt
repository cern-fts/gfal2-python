
SET(MAIN_DOC TRUE
	CACHE STRING "enable installation of the main library documentation")	


##
## execute make doc for doc generation
#
IF(MAIN_DOC)

        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/")
        addEpydocGeneration("epydoc_config.in")

        SET( l_doc "${CMAKE_CURRENT_BINARY_DIR}/html/")

        install(DIRECTORY ${l_doc}
                        DESTINATION ${DOC_INSTALL_DIR}/html/) # install examples


ENDIF(MAIN_DOC)




