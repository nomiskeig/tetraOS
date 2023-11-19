#include <stdio.h>

// TODO: this is like absolutely fucked but works for now, i guess
int printf(const char *format) {
    int i = 0;
    while (format[i] != '\0') {
        putchar(format[i]);
        i++;
    }
    return i;
}
