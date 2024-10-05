cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER /bin/riscv64-unknown-elf-g++)
add_compile_options(-g -nostdlib -nostdinc -fno-builtin -ffreestanding -fno-rtti -fno-exceptions -mcmodel=medany -march=rv64imad_zicsr -mabi=lp64d -fno-threadsafe-statics  -I/home/simon/dev/tetraOS/)
add_link_options(-nostdlib -fno-builtin -mcmodel=medany -march=rv64imad_zicsr -mabi=lp64d -T linker.ld )
project(kernel VERSION 1.0 LANGUAGES CXX ASM)
add_executable(kernel boot.s exception.cpp exception.s os.cpp UART.cpp UART.h vfs.cpp vfs.h mem.s processor.h arch/riscv64/processor.cpp)
set_target_properties(kernel PROPERTIES RUNTIME_OUTPUT_DIRECTORY /home/simon/dev/tetraOS/build/)

target_link_libraries(kernel PRIVATE libk)
target_link_libraries(kernel PRIVATE memory)
target_link_libraries(kernel PRIVATE drivers)
target_link_libraries(kernel PRIVATE process)
target_link_libraries(kernel PRIVATE syscalls)

add_subdirectory(libk)
add_subdirectory(memory)
add_subdirectory(drivers)
add_subdirectory(process)
add_subdirectory(syscalls)




