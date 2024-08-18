set(HEADER_LIST "${TetraOS_SOURCE_DIR}/include/libk/kstdio.h" "${TetraOS_SOURCE_DIR}/include/libk/list.h")
add_library(libk list.cpp logging.cpp printf.cpp  ${HEADER_LIST})
target_include_directories(libk PUBLIC ../include)
