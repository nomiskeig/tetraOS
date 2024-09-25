
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void pwd() {
    char* buffer = malloc(40);
    getcwd(buffer, 40);
    printf("%s", buffer);
}
