#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int fd = open(argv[1], O_RDONLY),
        fileSize;

    // Gets size of file
    fileSize = lseek(fd, 0, SEEK_END);

    // Goes back to the beginning of the file
    lseek(fd, 0, SEEK_SET);
    char content[fileSize + 1];
    read(fd, content, fileSize + 1);
    write(1, content, fileSize);
    close(fd);
    return 0;
}
