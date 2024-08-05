.section .rodata
.global TEXT_START
TEXT_START: .dword _text_start

.global TEXT_END
TEXT_END: .dword _text_end

.global RODATA_START
RODATA_START: .dword _rodata_start

.global RODATA_END
RODATA_END: .dword _rodata_end

.global DATA_START
DATA_START: .dword _data_start
.global DATA_END
DATA_END: .dword _data_end

.global BSS_START
BSS_START: .dword _bss_start


.global BSS_END
BSS_END: .dword _bss_end

.global PHYSICAL_MEMORY_START
PHYSICAL_MEMORY_START: .dword _physical_memory_start

.global PHYSICAL_MEMORY_END
PHYSICAL_MEMORY_END: .dword _physical_memory_end

.global PHYSICAL_HEAP_START
PHYSICAL_HEAP_START: .dword _physical_heap_start

.global PHYSICAL_HEAP_SIZE
PHYSICAL_HEAP_SIZE: .dword _physical_heap_size
.global KERNEL_START
KERNEL_START: .dword _kernel_start

.global KERNEL_END 
KERNEL_END: .dword _kernel_end
