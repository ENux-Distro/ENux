#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/mount.h>
#include <limits.h>

#define CHROOT_DIR argv[1]


int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s /path/to/chroot [command]\n", argv[0]);
        return 0;
    }
    if (access(CHROOT_DIR, X_OK) == 0) {
        printf("");
    } else {
        perror("ERROR");
        return 1;
    }

    char target[PATH_MAX];

    snprintf(target, sizeof(target), "%s/proc", argv[1]);
    if (mount("proc", target, "proc", 0, NULL) == -1) {
        perror("mount /proc");
        return EXIT_FAILURE;
    }

    snprintf(target, sizeof(target), "%s/sys", argv[1]);
    if (mount("/sys", target, NULL, MS_BIND, NULL) == -1) {
        perror("mount /sys");
        return EXIT_FAILURE;
    }

    snprintf(target, sizeof(target), "%s/dev", argv[1]);
    if (mount("/dev", target, NULL, MS_BIND, NULL) == -1) {
        perror("mount /dev");
        return EXIT_FAILURE;
    }

    snprintf(target, sizeof(target), "%s/run", argv[1]);
    if (mount("/run", target, NULL, MS_BIND, NULL) == -1) {
        perror("mount /run");
        return EXIT_FAILURE;
    }


    pid_t pid = fork();
    if (pid == 0) {

        if (chdir(argv[1]) == -1) {
            perror("chdir");
            _exit(EXIT_FAILURE);
        }

        if (chroot(argv[1]) == -1) {
            perror("chroot");
            _exit(EXIT_FAILURE);
        }

        if (chdir("/") == -1) {
            perror("chdir");
            _exit(EXIT_FAILURE);
        }

        if (argc == 2) {
            char *bash_argv[] = { "/bin/bash", NULL };

            execv("/bin/bash", bash_argv);

            perror("execv");
            _exit(EXIT_FAILURE);
        }

        execvp(argv[2], &argv[2]);

        perror("execvp");
        _exit(EXIT_FAILURE);

    } else {
        int status;

        if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
    }

}
    snprintf(target, sizeof(target), "%s/run", argv[1]);
    if (umount(target) == -1)
        perror("umount");

    snprintf(target, sizeof(target), "%s/dev", argv[1]);
    if (umount(target) == -1)
        perror("umount");

    snprintf(target, sizeof(target), "%s/sys", argv[1]);
    if (umount(target) == -1)
        perror("umount");

    snprintf(target, sizeof(target), "%s/proc", argv[1]);
    if (umount(target) == -1)
        perror("umount");


}
