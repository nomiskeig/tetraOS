add_library(process process.h process.s scheduler.h process.cpp scheduler.cpp) 
target_include_directories(process PUBLIC ../include)
target_link_libraries(process PRIVATE libk)
