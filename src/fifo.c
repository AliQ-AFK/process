#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    if (mkfifo("myfifo1", 0777) == -1) {
        if (errno != EEXIST) {
            printf("Could not create FIFO file\n");
            return 1;
        }
    }

    printf("Opening...\n");
    int fd = open("myfifo1", O_WRONLY);
    if (fd == -1) {
        return 3;
    }

    printf("Opened\n");
    int x = 'Z';
    if (write(fd, &x, sizeof(x)) == -1) {
        close(fd); // Close the file descriptor before returning
        return 2;
    }

    printf("Written\n");
    close(fd); // Close the file descriptor after writing
    return 0;
}
1