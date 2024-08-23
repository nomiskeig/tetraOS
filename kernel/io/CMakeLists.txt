set(HEADER_LIST "${TetraOS_SOURCE_DIR}/include/drivers/virtio/virtio.h")
add_library(io virtio.cpp  ${HEADER_LIST})
target_include_directories(io PUBLIC ../include)
target_link_libraries(io PRIVATE libk)
