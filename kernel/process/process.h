#pragma once 

#include "kernel/processor.h"

typedef uint64_t process_id_t;
class Path {
private: 
    const char* path_string ;
public:
    Path(const char* path);
    void copy_into(char* buffer);

    
};
enum ProcessState {
    READY, BLOCKED
};

class Process {
private:
    process_id_t process_id;
    ProcessState state;
    Path *current_working_directory;
    register64_t stack_pointer;
    address_t heap_start;
    address_t entry;
public:
    Process(process_id_t id, address_t entry, address_t stack_pointer, address_t heap_start);
    bool is_ready();
    address_t get_entry_address();
    address_t get_heap_start_address();
    address_t get_stack_pointer();
    process_id_t get_id();
    Path* get_cwd();



};

struct ProcessListItem {
    ProcessListItem* next;
    ProcessListItem* prev;
    Process* process;

};
