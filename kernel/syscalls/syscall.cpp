#include "kernel/memory.h"
#include <kernel/exception.h>
#include <kernel/libk/kstdio.h>
#include <kernel/process/scheduler.h>
#include <kernel/processor.h>
#include <kernel/vfs.h>

void handle_syscall(SyscallParameters syscall_params) {
    log(LogLevel::SYSCALL, "Handling system call with parameter %i",
        syscall_params.a7);
    switch (syscall_params.a7) {
    case 0: {
        read((void *)syscall_params.a1, syscall_params.a2);
        break;
    }
    case 1: {
        void *buffer = kalloc(syscall_params.a2 + 1);
        memcpy(buffer, (void *)syscall_params.a1, syscall_params.a2);
        *(((char *)buffer) + syscall_params.a2) = '\0';
        printf("%s", buffer);
        // TODO: free the buffer
        break;
    }
    case 2: {
        printf("syscall open\n");
            VFS *vfs = VFS::get_instance();
            vfs->open_file((char*)syscall_params.a0);
        break;
    }
    case 79: {
        printf("got getcwd syscall\n");

        Scheduler *scheduler = Scheduler::get_instance();
        printf("a\n");
        Process *current_process = scheduler->get_running_process();
        printf("b\n");
        Path *path = current_process->get_cwd();
        printf("c\n");
        path->copy_into((char *)syscall_params.a0);
        printf("d\n");
        break;
    }
    }
}
