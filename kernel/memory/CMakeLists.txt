set(HEADER_LIST, "${TetraOS_SOURCE_DIR}/include/memory.h")
add_library(memory memory.cpp paging.cpp physical_allocator.cpp virtual_allocator.cpp  ${HEADER_LIST})
target_include_directories(memory PUBLIC ../include)
target_link_libraries(memory PRIVATE libk)

