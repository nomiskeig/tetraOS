#ifndef STDLIB_H
#define STDLIB_H
#include "string.h"
void abort(void);
void free(void *pointer);
void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
int atoi(const char*nptr);
int abs(int j);
char*  getenv(const char*name);
void exit(int status);

#endif
