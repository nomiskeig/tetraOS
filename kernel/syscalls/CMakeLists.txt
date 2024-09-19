add_library(syscalls syscall.cpp syscall.h) 
target_link_libraries(syscalls PRIVATE libk)
