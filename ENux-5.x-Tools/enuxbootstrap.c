#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s /path/to/target\n", argv[0]);
        return 0;
    }

    if (argv[1], X_OK) {
        printf("");
    } else {
        mkdir(argv[1], 0755);
    }
}
