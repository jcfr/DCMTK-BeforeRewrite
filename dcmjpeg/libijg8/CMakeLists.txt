# create library from source files
ADD_LIBRARY(ijg8 jaricom jcapimin jcapistd jcarith jccoefct jccolor jcdctmgr jcdiffct jchuff jcinit jclhuff jclossls jclossy jcmainct jcmarker jcmaster jcodec jcomapi jcparam jcphuff jcpred jcprepct jcsample jcscale jcshuff jctrans jdapimin jdapistd jdarith jdatadst jdatasrc jdcoefct jdcolor jddctmgr jddiffct jdhuff jdinput jdlhuff jdlossls jdlossy jdmainct jdmarker jdmaster jdmerge jdphuff jdpostct jdpred jdsample jdscale jdshuff jdtrans jerror jfdctflt jfdctfst jfdctint jidctflt jidctfst jidctint jidctred jmemmgr jmemnobs jquant1 jquant2 jutils)

# declare installation files
INSTALL(TARGETS ijg8
        COMPONENT lib
        RUNTIME DESTINATION ${DCMTK_INSTALL_BINDIR}
        LIBRARY DESTINATION ${DCMTK_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${DCMTK_INSTALL_LIBDIR})
