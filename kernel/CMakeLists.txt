cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER /bin/riscv64-unknown-elf-g++)
add_compile_options(-g -nostdlib -fno-builtin -mcmodel=medany -march=rv64imad_zicsr -mabi=lp64d -fno-threadsafe-statics)
add_link_options(-nostdlib -fno-builtin -mcmodel=medany -march=rv64imad_zicsr -mabi=lp64d -T linker.ld)
project(TetraOS VERSION 1.0 LANGUAGES CXX ASM)
add_executable(os boot.s  os.cpp UART.cpp UART.h mem.s)
target_link_libraries(os PRIVATE libk)
target_link_libraries(os PRIVATE memory)

add_subdirectory(libk)
add_subdirectory(memory)

add_custom_target(qemu)
add_custom_command(
    TARGET os
    POST_BUILD
    COMMAND qemu-system-riscv64 -machine virt -nographic -kernel os -bios none
)

add_custom_target(debug)
add_custom_command(TARGET debug
    COMMAND qemu-system-riscv64 -machine virt -nographic -kernel os -bios none -S -s
)

add_custom_target(gdb)
add_custom_command(TARGET gdb
    COMMAND gdb
)

add_custom_target(dis)
add_custom_command(TARGET dis
    COMMAND /bin/riscv64-unknown-elf-objdump os -d
)


