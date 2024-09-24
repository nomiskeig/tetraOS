#include "syscalls/write.c"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
static char *buffer;
void init_printf() {
    // maximum size of output at once we support is 200 characters
    buffer = (char *)malloc(sizeof(char) * 200);
};
int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int res = vprintf(fmt, args);
    va_end(args);
    return res;
}

int vprintf(const char *fmt, va_list args) {

    size_t curr = 0;
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd' || *fmt == 'i') {
                int i = va_arg(args, int);
                char chars[40];
                int index = 0;
                while (i >= 10) {
                    int last = i % 10;
                    chars[index] = last + '0';
                    index += 1;
                    i = i / 10;
                }
                buffer[curr] = (i + 48);
                curr++;
                index -= 1;
                while (index >= 0) {
                    buffer[curr] = chars[index];
                    curr++;
                    index -= 1;
                }
                fmt++;
            } else if (*fmt == 'x') {
                uint64_t i = va_arg(args, uint64_t);
                char chars[200];
                int index = 0;
                while (i >= 16) {
                    int last = i % 16;
                    chars[index] = last;
                    index += 1;
                    i = i / 16;
                }
                chars[index] = i;
                while (index >= 0) {
                    int offset = 48;
                    if (chars[index] > 9) {
                        offset = 65 - 10;
                    }
                    buffer[curr] = chars[index] + offset;
                    curr++;
                    index -= 1;
                }
                fmt++;
            } else if (*fmt == 's') {
                char *chars = va_arg(args, char *);
                int current = 0;
                while (chars[current] != '\0') {
                    buffer[curr] = chars[current];
                    curr++;
                    current++;
                }
                fmt++;

            } else if (*fmt == 'c') {
                char c = va_arg(args, int);
                buffer[curr] = c;
                curr ++;
                fmt++;
            }

        } else {
            buffer[curr] = *fmt++;
            curr++;
        }
    }
    write(1, buffer, curr);
    return 0;


}
