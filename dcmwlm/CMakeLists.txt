# declare project
PROJECT(dcmwlm)

# declare include directories which hold for all subdirectories
INCLUDE_DIRECTORIES(${dcmwlm_SOURCE_DIR}/include ${ofstd_SOURCE_DIR}/include ${oflog_SOURCE_DIR}/include ${oflog_SOURCE_DIR}/include ${dcmdata_SOURCE_DIR}/include ${dcmnet_SOURCE_DIR}/include ${ZLIB_INCDIR})

# recurse into subdirectories
SUBDIRS(libsrc apps wwwapps include docs data)
