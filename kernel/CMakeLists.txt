
add_executable(kernel boot.s  os.cpp UART.cpp UART.h mem.s)
target_link_libraries(kernel PRIVATE libk)
target_link_libraries(kernel PRIVATE memory)
target_link_libraries(kernel PRIVATE drivers)

add_subdirectory(libk)
add_subdirectory(memory)
add_subdirectory(drivers)

add_custom_target(qemu)
add_custom_command(
    TARGET kernel 
    POST_BUILD
    COMMAND qemu-system-riscv64 -machine virt -nographic -kernel kernel -bios none -drive if=none,format=raw,file=../data,id=foo -device virtio-blk-device,scsi=off,drive=foo -device virtio-net-device -m 2048M -global virtio-mmio.force-legacy=false
)

add_custom_target(debug)
add_custom_command(TARGET debug
    COMMAND qemu-system-riscv64 -machine virt -nographic -kernel os -bios none -device virtio-net-device -S -s
)

add_custom_target(gdb)
add_custom_command(TARGET gdb
    COMMAND gdb
)

add_custom_target(dis)
add_custom_command(TARGET dis
    COMMAND /bin/riscv64-unknown-elf-objdump os -d
)


