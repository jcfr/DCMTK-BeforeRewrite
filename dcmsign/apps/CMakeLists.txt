# declare directories containing used libraries
LINK_DIRECTORIES(${dcmsign_BINARY_DIR} ${dcmdata_BINARY_DIR} ${oflog_BINARY_DIR} ${ofstd_BINARY_DIR} ${ZLIB_LIBDIR} ${LIBICONV_LIBDIR} ${OPENSSL_LIBDIR})

# declare executables
ADD_EXECUTABLE(dcmsign dcmsign)

# declare installation files
INSTALL(TARGETS dcmsign DESTINATION ${DCMTK_INSTALL_BINDIR} COMPONENT bin)

# make sure executables are linked to the corresponding libraries
TARGET_LINK_LIBRARIES(dcmsign dcmdsig dcmdata oflog ofstd oflog ${ZLIB_LIBS} ${LIBICONV_LIBS} ${OPENSSL_LIBS} ${WIN32_STD_LIBRARIES} ${THREAD_LIBS})
