
#include "./process.h"
#include <kernel/drivers/ext2.h>
#include <kernel/memory.h>
#include <kernel/process/elf.h>

Path::Path(const char *path) { 
    size_t size = 0;
    while (path[size]) {
        size++;
    }
    char* path_buffer = (char*)kalloc(size);
    memcpy(path_buffer, path, size);
    this->path_string = path_buffer;
}

void Path::copy_into(char *buffer) {
    printf("copying\n");
    size_t size = 0;
    while (this->path_string[size]) {
        size++;
    }
    printf("size: %i\n", size);
    memcpy(buffer, this->path_string, size);
}

Process::Process(process_id_t id, address_t entry, address_t stack,
                 address_t heap_start) {
    this->process_id = id;
    this->heap_start = heap_start;
    this->stack_pointer = stack;
    this->entry = entry;
    this->state = ProcessState::READY;
    Path *path = new Path("/home/user/");
    this->current_working_directory = path;
}

bool Process::is_ready() { return this->state == ProcessState::READY; }

address_t Process::get_heap_start_address() { return this->heap_start; }
address_t Process::get_stack_pointer() { return this->stack_pointer; }

address_t Process::get_entry_address() { return this->entry; }

process_id_t Process::get_id() { return this->process_id; }

Path* Process::get_cwd() {
    return this->current_working_directory;
}
