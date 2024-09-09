
#include <kernel/drivers/ext2.h>
#include <kernel/memory.h>
#include <kernel/process/elf.h>
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
    Elf64_Phdr *program_headers =
        (Elf64_Phdr *)(contents + program_headers_offset);
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
                     (PhysicalFrame *)physical_frame);
        }
        memcpy((void *)(virtual_address_start_address + (vaddr % PAGE_SIZE)),
               (void *)(contents + program_headers[i].p_offset),
               program_headers[i].p_memsz);
    }

    

    return 0;
}
