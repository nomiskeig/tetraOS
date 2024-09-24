#include <kernel/UART.h>
#include <kernel/libk/kstdio.h>

//
size_t read(void *buffer, size_t size) {
    size_t counter = 0;
    bool cont = true;
    while (counter < size && cont) {
        char c = get_char();
        if (c == '\r') {
            ((char *)buffer)[counter] = '\0';
            put_char('\n');
            break;
        }
        put_char(c);
        ((char *)buffer)[counter] = c;
        counter++;
    }
    return counter;
}
