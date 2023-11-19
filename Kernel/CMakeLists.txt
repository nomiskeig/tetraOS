add_executable(Kernel "")


target_sources(Kernel PRIVATE
    asm/boot.S
    asm/mem.S
    main.cpp
    kprintf.cpp


    PUBLIC
    uart.h
    klib.h
    )

target_compile_options(Kernel PUBLIC -ffreestanding -nostdlib -O0 -Wall -Wextra -fno-exceptions -fno-rtti)
target_link_options(Kernel PUBLIC -T /home/simon/dev/tetraOS/Kernel/linker.ld -ffreestanding -O0 -nostdlib -lgcc -fno-builtin) 

