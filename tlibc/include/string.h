#ifndef STRING_H
#define STRING_H

typedef unsigned long int size_t;
typedef long int ssize_t;
#define NULL 0x0
void *memcpy(const void* dest, const void* src, size_t n);
void *memset(void *s, int c, size_t n);
size_t strlen(const char *s);
char *strchr(const char*s, int c);
char* strcat(char *s1, const char* s2);

int strcmp(const char* s1, const char*s2);
int strncmp(const char* s1,const char* s2, size_t n);


#endif
