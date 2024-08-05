
set(HEADER_LIST "${LearnOS_SOURCE_DIR}/include/libk/kstdio.h" "${LearnOS_SOURCE_DIR}/include/libk/list.h")
add_library(libk printf.cpp ${HEADER_LIST})
target_include_directories(libk PUBLIC ../include)
