# declare directories containing used libraries
LINK_DIRECTORIES(${dcmimgle_BINARY_DIR} ${ofstd_BINARY_DIR} ${oflog_BINARY_DIR} ${dcmdata_BINARY_DIR} ${ZLIB_LIBDIR} ${LIBICONV_LIBDIR})

# declare executables
FOREACH(PROGRAM dcmdspfn dcod2lum dconvlum)
  ADD_EXECUTABLE(${PROGRAM} ${PROGRAM})
ENDFOREACH(PROGRAM)

# declare installation files
INSTALL(TARGETS dcmdspfn dcod2lum dconvlum DESTINATION ${DCMTK_INSTALL_BINDIR} COMPONENT bin)

# make sure executables are linked to the corresponding libraries
FOREACH(PROGRAM dcmdspfn dcod2lum dconvlum)
  TARGET_LINK_LIBRARIES(${PROGRAM} dcmimgle dcmdata oflog ofstd ${ZLIB_LIBS} ${LIBICONV_LIBS} ${WIN32_STD_LIBRARIES} ${THREAD_LIBS})
ENDFOREACH(PROGRAM)
