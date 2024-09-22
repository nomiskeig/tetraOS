
#include <kernel/drivers/ext2.h>
#include <kernel/memory.h>
#include <kernel/process/elf.h>
extern "C" void switch_to_process(long, long, long);
extern "C" void set_sum_bit();
extern "C" void set_spp_zero();
int create_and_run_new_process(const char *file) {
    log(LogLevel::PROCESS, "Creating process %s", file);

    void *physical_frame = kalloc_frame();
    // VirtualPage* virtual_page = 0x0;
    // map_page(virtual_page, (PhysicalFrame*)physical_frame);
    size_t file_size = get_file_size(file);
    printf("file size: %x\n", file_size);
    char *contents = (char *)kalloc(file_size);
    read_file(file, file_size, contents);
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)contents;
    // TODO: verify ELF magic number
    printf("size: %i\n", elf_header->e_ehsize);
    printf("entry: 0x%x\n", elf_header->e_entry);
    printf("format: %i\n", elf_header->e_ident[4]);
    uint64_t program_headers_offset = elf_header->e_phoff;
    uint32_t program_headers_amount = elf_header->e_phnum;
    uint64_t bytes_for_program_headers =
        program_headers_amount * elf_header->e_phentsize;
    printf("program header size: %i\n", elf_header->e_phentsize);
    set_sum_bit(); // allows accessing user pages from supervisor mode
    
    printf("test");
    Elf64_Phdr *program_headers =
        (Elf64_Phdr *)(contents + program_headers_offset);
    uint64_t data_end = 0x0;
    for (size_t i = 0; i < program_headers_amount; i++) {
        // check if we have to load the segment
        if (program_headers[i].p_type != 0x1) {
            continue;
        }
        // found a loadable segment, so let's load it
        printf("virtual address of segment: 0x%x\n",
               program_headers[i].p_vaddr);
        size_t amount_pages_to_map =
            (program_headers[i].p_memsz / PAGE_SIZE) + 1;
        printf("amount pages to map: %i\n", amount_pages_to_map);
        printf("virtual start: 0x%x\n", program_headers[i].p_vaddr);
        uint64_t vaddr = program_headers[i].p_vaddr;
        uint64_t virtual_address_start_address = vaddr - (vaddr % PAGE_SIZE);
        printf("virtual address start address: 0x%x",
               virtual_address_start_address);
        for (int i = 0; i < amount_pages_to_map; i++) {
            void *physical_frame = kalloc_frame();
            map_page((VirtualPage *)virtual_address_start_address,
                     (PhysicalFrame *)physical_frame, 0x1F);
        }
        printf("mapped\n");
        memcpy((void *)(virtual_address_start_address + (vaddr % PAGE_SIZE)),
               (void *)((uint64_t)contents + program_headers[i].p_offset),
               program_headers[i].p_filesz);
        if (data_end < (program_headers[i].p_vaddr  + program_headers[i].p_memsz)) {
            data_end = program_headers[i].p_vaddr  + program_headers[i].p_memsz;
        }
        printf("copied\n");
    }
    printf("is here\n");
    printf("data end: 0x%x\n", data_end);
    log(LogLevel::PROCESS, "Jumping to process at address 0x%x", elf_header->e_entry);
    void * stack_frame = kalloc_frame();
    uint64_t stack_page = 0x100000;
    map_page((VirtualPage*)stack_page, (PhysicalFrame*)stack_frame, 0x1F);
    uint64_t stack_pointer = stack_page + 4000;
    printf("stack pointer: 0x%x", stack_pointer);
    set_spp_zero();
    switch_to_process((uint64_t)elf_header->e_entry, stack_pointer, data_end + 20);
    printf("after switch process\n");
    while(true) {
       // printf("looping at end, this should not happen as we should be in the process");
    }

    

}
