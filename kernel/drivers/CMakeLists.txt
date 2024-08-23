set(HEADER_LIST "${TetraOS_SOURCE_DIR}/include/drivers/virtio/virtio.h" "${TetraOS_SOURCE_DIR}/include/drivers/virtio/blk.h")
add_library(drivers virtio/virtio.cpp  virtio/blk.cpp ${HEADER_LIST})
target_include_directories(drivers PUBLIC ../include)
target_link_libraries(drivers PRIVATE libk)
