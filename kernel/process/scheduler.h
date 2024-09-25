#pragma once
#include "./process.h"

class Scheduler {
private:
    Process* current_process;
    process_id_t next_id;
    ProcessListItem *first_process;
    void add_new_process(Process* process);
    process_id_t get_next_process_id();

public:
    Scheduler();
    int submit_new_process(const char *file);
    void run_next_process();
    Process* get_running_process();
};
