add_executable(kernel boot.s  os.cpp UART.cpp UART.h mem.s)
set_target_properties(kernel PROPERTIES RUNTIME_OUTPUT_DIRECTORY /home/simon/dev/tetraOS/build/)
target_link_libraries(kernel PRIVATE libk)
target_link_libraries(kernel PRIVATE memory)
target_link_libraries(kernel PRIVATE drivers)
target_link_libraries(kernel PRIVATE process)

add_subdirectory(libk)
add_subdirectory(memory)
add_subdirectory(drivers)
add_subdirectory(process)


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


