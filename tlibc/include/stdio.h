
#ifndef STDIO_H
#define STDIO_H
#include "stdarg.h"
#include "string.h"

#define SEEK_SET 0
typedef struct {
  int unused;
} FILE;
#define __UINTPTR_TYPE__ unsigned long int
#define __INTPTR_TYPE__ long int
extern FILE *stdin;
extern FILE *stdout;
extern FILE* stderr;
#define stderr stderr
// TODO: something errors when building gcc with this, but it results in a usable compiler...
int fclose(FILE *stream);
FILE *fopen();
int fflush(FILE *stream);
int fprintf(FILE *stream, const char *format, ...);
long ftell(FILE *stream);
int fseek(FILE *stream, long offset, int whence);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int sprintf(char *str, const char *format, ...);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int feof(FILE *stream);
int vprintf(const char *format, va_list ap);
int vfprintf(FILE *stream, const char *format, va_list ap);
int printf(const char *fmt, ...);
int scanf(const char* format, ...);

#endif
