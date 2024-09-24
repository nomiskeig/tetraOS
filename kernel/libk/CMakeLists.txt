add_library(libk list.cpp logging.cpp printf.cpp read.cpp kstdio.h list.h)
target_include_directories(libk PUBLIC ../include)
