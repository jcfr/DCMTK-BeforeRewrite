# declare project
PROJECT(doxygen)

# doxygen support
IF(DCMTK_WITH_DOXYGEN)

  IF(DOXYGEN_EXECUTABLE)
    ADD_CUSTOM_TARGET(DOXYGEN)
    ADD_CUSTOM_TARGET(html ${DOXYGEN_EXECUTABLE} htmldocs.tmp)
    ADD_DEPENDENCIES(DOXYGEN html)
    IF(NOT WIN32) # For Unix systems, also build manpages
      ADD_CUSTOM_TARGET(man COMMAND ${DOXYGEN_EXECUTABLE} manpages.tmp
                            COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/manpages/man1/*.man.1
                            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/patchman.sh)
      ADD_DEPENDENCIES(DOXYGEN man)
    ENDIF(NOT WIN32)
  ENDIF(DOXYGEN_EXECUTABLE)

  # create configuration files (expand variables)
  SET(DCMTK_VERSION "${DCMTK_PACKAGE_VERSION}${PACKAGE_VERSION_SUFFIX}")
  SET(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}")
  SET(DOXYGEN_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/htmldocs.cfg ${CMAKE_CURRENT_BINARY_DIR}/htmldocs.tmp ESCAPE_QUOTES @ONLY)
  IF(NOT WIN32)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/manpages.cfg ${CMAKE_CURRENT_BINARY_DIR}/manpages.tmp ESCAPE_QUOTES @ONLY)
  ENDIF(NOT WIN32)

  # create new directories
  FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/htmldocs)
  IF(NOT WIN32)
    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manpages)
  ENDIF(NOT WIN32)

  # install html docs and manpages
  INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/htmldocs/ DESTINATION ${DCMTK_INSTALL_HTMDIR} COMPONENT html PATTERN "CVS" EXCLUDE)
  IF(NOT WIN32)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manpages/ DESTINATION ${DCMTK_INSTALL_MANDIR} COMPONENT man PATTERN "CVS" EXCLUDE)
  ENDIF(NOT WIN32)

  # cleanup created files on "make clean" - directories are not removed with CMake 2.4 :-/
  SET(make_clean_files ${CMAKE_CURRENT_BINARY_DIR}/htmldocs ${CMAKE_CURRENT_BINARY_DIR}/manpages)
  SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${make_clean_files}")

  # the created files "htmldocs.tmp" and "manpages.tmp" should be removed with "make distclean",
  # which will (hopefully) be introduced in a future version, because they are only created during
  # the configure/generate process

ELSE(DCMTK_WITH_DOXYGEN)

  # by default, install the pre-defined manpages, i.e. the ones shipped with this package
  IF(NOT WIN32)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/manpages/ DESTINATION ${DCMTK_INSTALL_MANDIR} COMPONENT man PATTERN "CVS" EXCLUDE)
  ENDIF(NOT WIN32)

ENDIF(DCMTK_WITH_DOXYGEN)
