#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <limits.h>

int apt_kernel(const char *path) {

    char *args[] = {"apt", "install", "/linux-headers-7.1.0-rc4-enux_amd64.deb", "/linux-image-7.1.0-rc4-enux_amd64.deb", NULL};

    pid_t pid = fork();
    if (pid == 0) {
        chroot(path);
        chdir("/");
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }


}

int rm_kernel(const char *path) {

    char *args[] = {"rm", "-rf", "/linux-headers-7.1.0-rc4-enux_amd64.deb", "/linux-image-7.1.0-rc4-enux_amd64.deb", NULL};

    pid_t pid = fork();
    if (pid == 0) {
        chroot(path);
        chdir("/");
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }

}

int kernel_headers(const char *path) {

    char *args[] = {"wget", "https://github.com/ENux-Distro/ENux-Kernels/releases/download/7.1-rc4/linux-headers-7.1.0-rc4-enux_amd64.deb", "-O", "/linux-headers-7.1.0-rc4-enux_amd64.deb", NULL};

    pid_t pid = fork();
    if (pid == 0) {
        chroot(path);
        chdir("/");
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }

}

int kernel_image(const char *path) {

    char *args[] = {"wget", "https://github.com/ENux-Distro/ENux-Kernels/releases/download/7.1-rc4/linux-image-7.1.0-rc4-enux_amd64.deb", "-O", "/linux-image-7.1.0-rc4-enux_amd64.deb", NULL};

    pid_t pid = fork();
    if (pid == 0) {
        chroot(path);
        chdir("/");
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }


}

int resolv(const char *path) {

    char rpath[PATH_MAX];
    snprintf(rpath, sizeof(rpath), "%s/etc/resolv.conf", path);

    char *args[] = {"cp", "/etc/resolv.conf", rpath, NULL};

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }

}

int apt() {

    char *args[] = {"apt", "install", "debootstrap", "-y", NULL};

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }

}

int debootstrap(const char *path) {

    char *args[] = {"debootstrap", "stable", path, "http://deb.debian.org/debian", NULL};

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }

}

int wget(const char *path) {

    char *args[] = {"apt", "install", "wget", NULL};

    pid_t pid = fork();
    if (pid == 0) {
        chroot(path);
        chdir("/");
        execvp(args[0], args);
    } else {
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
    }

}

int main(int argc, char **argv) {


    if (argc < 2) {
        printf("Usage: %s /path/to/target\n", argv[0]);
        return 0;
    }

    if (access(argv[1], F_OK) == 0) {
        remove(argv[1]);
        printf("");
    } else {
        remove(argv[1]);
        if (mkdir(argv[1], 0755) == -1) {
            perror("mkdir");
            return EXIT_FAILURE;
        }
    }

    if (access("/bin/apt", X_OK) == 0 || access("/usr/bin/apt", X_OK) == 0 || access("/usr/local/bin/apt", X_OK) == 0 || access("/sbin/apt", X_OK) == 0 || access("/usr/sbin/apt", X_OK) == 0 || access("/usr/local/sbin/apt", X_OK) == 0) {
        printf("");
    } else {
        perror("ERROR: apt");
    }

    const char *path;
    const char *epath;
    char target[PATH_MAX];
    path = argv[1];
    epath = argv[1];

    apt();
    debootstrap(path);

    snprintf(target, sizeof(target), "%s/proc", argv[1]);
    mkdir(target, 0755);
    if (mount("proc", target, "proc", 0, NULL) == -1) {
        perror("mount /proc");
        return EXIT_FAILURE;
    }

    snprintf(target, sizeof(target), "%s/sys", argv[1]);
    mkdir(target, 0755);
    if (mount("/sys", target, NULL, MS_BIND, NULL) == -1) {
        perror("mount /sys");
        return EXIT_FAILURE;
    }

    snprintf(target, sizeof(target), "%s/dev", argv[1]);
    mkdir(target, 0755);
    if (mount("/dev", target, NULL, MS_BIND, NULL) == -1) {
        perror("mount /dev");
        return EXIT_FAILURE;
    }

    snprintf(target, sizeof(target), "%s/run", argv[1]);
    mkdir(target, 0755);
    if (mount("/run", target, NULL, MS_BIND, NULL) == -1) {
        perror("mount /run");
        return EXIT_FAILURE;
    }

    resolv(epath);
    wget(path);
    kernel_image(path);
    kernel_headers(path);
    apt_kernel(path);
    rm_kernel(path);

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
