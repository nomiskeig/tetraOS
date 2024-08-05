#include "../include/libk/kstdio.h"
#include "../UART.h"
#include <cstdarg>


void printf(char const *fmt, ...) {
    va_list args;
    va_start(args, fmt);
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
                put_char(i + 48);
                index -= 1;
                while (index >= 0) {
                    put_char(chars[index]);
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
                    put_char(chars[index] + offset);
                    index -= 1;
                }
                fmt++;
            }

        } else {
            put_char(*fmt++);
        }
    }
}
