#include "syscalls/write.c"
#include <stdlib.h>
int printf(const char *fmt, ...) {
    // TODO: support printing different formats and parameters
  size_t size = 0;

  while (fmt[size]) {
    size++;
  };
  char *buffer = (char *)malloc((sizeof(char) + 1) * size);
  size_t cur = 0;
  while (fmt[cur]) {
    buffer[cur] = fmt[cur];
    cur++;
  }
  write(2, buffer, size);
  return 0;
}
