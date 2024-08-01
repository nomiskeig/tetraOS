
set(HEADER_LIST "${LearnOS_SOURCE_DIR}/include/libk/printf.h")
add_library(libk printf.cpp ${HEADER_LIST})
target_include_directories(libk PUBLIC ../include)
