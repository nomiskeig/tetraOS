# System  calls


System calls are handled by the handle_syscall function in syscalls.c
In generell, the following convention is used:

Registers a1 to a6 contain the arguments starting with a1.
Register a7 holds the number of the system call.

The system call numbers are the same as the numbers linux uses on x86
Syscalls available with their number are: 

| System call | number | status |
| ----------- | ------ | ------ | 
| Read        | 0 
| Write       | 1 

