# create library from source files
ADD_LIBRARY(dcmpstat dcmpstat dviface dvpsab dvpsabl dvpsal dvpsall dvpscf dvpscu dvpscul dvpsda dvpsdal dvpsfs dvpsga dvpsgal dvpsgl dvpsgll dvpsgr dvpsgrl dvpshlp dvpsib dvpsibl dvpsmsg dvpsov dvpsovl dvpspl dvpspl2 dvpspll dvpspr dvpsprt dvpsri dvpsril dvpsrs dvpsrsl dvpssp dvpsspl dvpssv dvpssvl dvpstat dvpstx dvpstxl dvpsvl dvpsvll dvpsvw dvpsvwl dvsighdl)

TARGET_LINK_LIBRARIES(dcmpstat ofstd oflog dcmdata dcmimgle dcmimage dcmnet dcmdsig dcmtls dcmsr dcmqrdb)

# declare installation files
INSTALL(TARGETS dcmpstat
        COMPONENT lib
        RUNTIME DESTINATION ${DCMTK_INSTALL_BINDIR}
        LIBRARY DESTINATION ${DCMTK_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${DCMTK_INSTALL_LIBDIR})
