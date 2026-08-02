#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define PMM "/bedrock/bin/pmm"

int main(int argc, char **argv) {


    if (access(PMM, X_OK) == 0) {
        printf("");
    } else {
        printf("ERROR: Bedrock Linux isn't hijacked. You must have Bedrock Linux hijacked on your system to use this tool\n");
        return 1;
    }

    if (argc < 2) {
            if (access(PMM, X_OK) == 0) {
                printf("ENux Package Manager\n");
                printf("Use the --about flag for more information\n");
                return 1;
            }
    }

    if (strcmp(argv[1], "--about") == 0) {

        if (access(PMM, X_OK) == 0) {
            printf("\n");
            printf("ENux Package Manager\n");
            printf("Simple commands:\n");
            printf("install - Installs your desired package\n");
            printf("remove - Removes your desired package\n");
            printf("update - Updates the system\n");
            printf("upgrade - Upgrades the system\n");
            printf("\n");
            if (geteuid() == 0) {
                printf("And the power is unlocked now\n"); /* Thank you cajald for this idea */
            }
            return 0;
        }
    }

    char *newargv[argc + 1];

    newargv[0] = PMM;

    for (int i = 1; i < argc; i++)
        newargv[i] = argv[i];

    newargv[argc] = NULL;

    if (geteuid() != 0) {
        fprintf(stderr,
            "ERROR: You need to run this program with root privileges\n");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        execv(PMM, newargv);

        perror("execv");
        _exit(EXIT_FAILURE);
    }

    /* Parent */

    int status;

    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return EXIT_FAILURE;
}
