cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER /bin/riscv64-unknown-elf-g++)
add_compile_options(-g -nostdlib -nostdinc -fno-builtin -ffreestanding -fno-rtti -fno-exceptions -mcmodel=medany -march=rv64imad_zicsr -mabi=lp64d -fno-threadsafe-statics  -I/home/simon/dev/tetraOS/)
add_link_options(-nostdlib -fno-builtin -mcmodel=medany -march=rv64imad_zicsr -mabi=lp64d -T linker.ld )
project(TetraOS VERSION 1.0 LANGUAGES CXX ASM)

add_subdirectory(kernel)
