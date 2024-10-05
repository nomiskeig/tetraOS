#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtins.h"

int main() {
    int a = 0;
    a++;
    char* buffer = malloc(20);
    printf("test from userspace\n");
    printf("second test, lets see if this works\n");
    printf("and a third time\n");
    printf("calling ls\n");
    ls();
    printf("after ls\n");
    read(0, buffer, 20);
    printf("got input in userspace: %s\n",buffer);
    if (strcmp(buffer, "pwd") == 0) {
        pwd();
    }
    if (strcmp(buffer, "ls") == 0)  {
        ls();
    }

    while (1) {

    }
    return 0;
}
