# the name of the target operating system
set(CMAKE_SYSTEM_NAME tetraOS)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER riscv64-tetraos-gcc -Wl,-melf_riscv64_tetraos)
set(CMAKE_CXX_COMPILER riscv64-tetraos-g++ -Wl,-melf_riscv64_tetraos)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /home/simon/dev/tetraOS/rootFS)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
