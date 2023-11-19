#include "klib.h"
#include "uart.h"
#include   <stdarg.h>

int kprintf(const char *fmt,...) {
    va_list args;
    va_start(args, fmt);
    int count = 0;
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'i':
                case 'd':
                    {
                        int x = va_arg(args, int);
                        // TODO: this buffer should probably have dynamic size
                        // TODO: this maybe fucks up the stack i dont fing know
                        char buf[10];
                        int current = 0;
                        // use fucking math to print this because we need each number of the number 
                        // this must be changed if the base is different
                        int rest = x;
                        while (rest > 9) {
                            buf[current] = ((rest % 10) + 48);
                            rest = rest / 10;
                            current += 1;
                        }
                        buf[current] = rest + 48;
                        // print the buffer backwards
                        for (int i = current; i >= 0; i--) {
                            kputc(buf[i]);
                        }
                        break;
                    }
                case 's':
                    kputc(va_arg(args, char));
                    break;
                case 'x':
                    kputc(va_arg(args, int));
                    break;
                case 'c':
                    kputc(va_arg(args, char));
                    break;
                default:
                    kputc(*fmt);
                    break;
            }
        } else {
            kputc(*fmt);
        }
        fmt++;
        count++;
    }
    va_end(args);
    return count;

}


int kputc(char c) {
    write_char(c);
    return 1;

}
