
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
void pwd() {
    char* buffer = malloc(40);
    getcwd(buffer, 40);
    printf("%s", buffer);
}

void ls() {
    int fd = open("/home/user/", 0, 0);
    printf("got file descriptor: %i\n", fd);


}
