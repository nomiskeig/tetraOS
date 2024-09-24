#ifndef UNISTD_H
#define UNISTD_H

#include <string.h>
typedef int pid_t;
typedef unsigned long int intptr_t;

pid_t fork(void);
int execv(const char* path, char *const argv[]);
int execvp(const char*file, char*const argv[]);
int execve(const char *filename, char* const argv[], char* const envp[]);
pid_t getpid(void);


int brk(void *addr);
void *sbrk(intptr_t incremement);


ssize_t read(int fildes, void*buf, size_t nybte);
#endif
