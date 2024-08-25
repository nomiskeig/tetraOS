add_library(drivers virtio/virtio.cpp ext2.cpp virtio/blk.cpp virtio/blk.h)
target_include_directories(drivers PUBLIC ../include)
target_link_libraries(drivers PRIVATE libk)
