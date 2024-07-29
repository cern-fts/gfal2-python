
SET(MAIN_DOC TRUE CACHE STRING "enable installation of the main library documentation")

# Execute "make doc" for doc generation
if (MAIN_DOC AND BUILDDOCS)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/")
    addEpydocGeneration("epydoc_config.in")
    # Install compiled examples
    SET(l_doc "${CMAKE_CURRENT_BINARY_DIR}/html/")
    install(DIRECTORY ${l_doc}
              DESTINATION ${DOC_INSTALL_DIR}/html/)
endif()
