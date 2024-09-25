#include "scheduler.h"
#include <kernel/drivers/ext2.h>
#include <kernel/libk/kstdio.h>
#include <kernel/memory.h>
#include <kernel/process/elf.h>
extern "C" void switch_to_process(long, long, long);
extern "C" void set_sum_bit();
extern "C" void set_spp_zero();
static bool created = false;
static Scheduler* instance = 0x0;

int Scheduler::submit_new_process(const char *file) {
    log(LogLevel::PROCESS, "Creating process %s", file);

    void *physical_frame = kalloc_frame();
    // VirtualPage* virtual_page = 0x0;
    // map_page(virtual_page, (PhysicalFrame*)physical_frame);
    size_t file_size = get_file_size(file);
    char *contents = (char *)kalloc(file_size);
    read_file(file, file_size, contents);
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)contents;
    // TODO: verify ELF magic number
    uint64_t program_headers_offset = elf_header->e_phoff;
    uint32_t program_headers_amount = elf_header->e_phnum;
    uint64_t bytes_for_program_headers =
        program_headers_amount * elf_header->e_phentsize;
    set_sum_bit(); // allows accessing user pages from supervisor mode
    Elf64_Phdr *program_headers =
        (Elf64_Phdr *)(contents + program_headers_offset);
    uint64_t data_end = 0x0;
    for (size_t i = 0; i < program_headers_amount; i++) {
        // check if we have to load the segment
        if (program_headers[i].p_type != 0x1) {
            continue;
        }
        // found a loadable segment, so let's load it
        size_t amount_pages_to_map =
            (program_headers[i].p_memsz / PAGE_SIZE) + 1;
        uint64_t vaddr = program_headers[i].p_vaddr;
        uint64_t virtual_address_start_address = vaddr - (vaddr % PAGE_SIZE);
        for (int i = 0; i < amount_pages_to_map; i++) {
            void *physical_frame = kalloc_frame();
            map_page((VirtualPage *)virtual_address_start_address,
                     (PhysicalFrame *)physical_frame, 0x1F);
        }
        memcpy((void *)(virtual_address_start_address + (vaddr % PAGE_SIZE)),
               (void *)((uint64_t)contents + program_headers[i].p_offset),
               program_headers[i].p_filesz);
        if (data_end <
            (program_headers[i].p_vaddr + program_headers[i].p_memsz)) {
            data_end = program_headers[i].p_vaddr + program_headers[i].p_memsz;
        }
    }
    // find next 4k boundary after data_end to put heap on
    data_end = data_end + PAGE_SIZE - (data_end % PAGE_SIZE);
    // map the first heap page
    void *heap_frame = kalloc_frame();
    map_page((VirtualPage *)data_end, (PhysicalFrame *)heap_frame, 0x1F);

    void *stack_frame = kalloc_frame();
    uint64_t stack_page = 0x100000;
    map_page((VirtualPage *)stack_page, (PhysicalFrame *)stack_frame, 0x1F);
    uint64_t stack_pointer = stack_page + 4000;
    set_spp_zero();

    Process *process =
        new Process(this->get_next_process_id(), elf_header->e_entry,
                    stack_pointer, data_end);
    this->add_new_process(process);
    return 0;
}

void Scheduler::add_new_process(Process *process) {
    ProcessListItem *newItem = new ProcessListItem();
    newItem->next = 0x0;
    newItem->process = process;
    ProcessListItem *item;
    if (this->first_process) {
        item = this->first_process;
        while (item->next) {
            item = item->next;
        }
        item->next = newItem;
        newItem->prev = item;
    } else {

        this->first_process = newItem;
    }
};

Scheduler::Scheduler() {
    printf("is in constructor");
    this->next_id = 0;
    this->first_process = 0x0;
};

process_id_t Scheduler::get_next_process_id() { return this->next_id++; }

void Scheduler::run_next_process() {
    ProcessListItem *processItem = this->first_process;
    

    while (!processItem->process->is_ready()) {
        printf("iterating");
        processItem = processItem->next;
    }

    Process *process = processItem->process;
    log(LogLevel::PROCESS, "Running next process with id %i",
        process->get_id());
    switch_to_process(process->get_entry_address(),
                      process->get_stack_pointer(),
                      process->get_heap_start_address());
    while (true) {
        log(LogLevel::ERROR, "This is after switching to userspace, something "
                             "went terribly wrong");
    }
}

Scheduler *Scheduler::get_instance() {
    printf("getting instacne\n");
    printf("instance: 0x%x\n", instance);
    if (!created) {
        printf("allocating instance");
        instance = new Scheduler();
        created = true;
    }
    printf("instance: 0x%x\n", instance);
    return instance;
}

