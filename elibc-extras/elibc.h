/* E-Libc - elibc.h
elibc.h contains of the usual commands that the creator of ENux uses, like
read, write, system and etc
In my opinion, this is minimal
*/

#ifndef ELIBC_H
#define ELIBC_H

#include <unistd.h>
#include <sys/syscall.h>
#include <stddef.h>
#include <sys/wait.h>

/* Being the commands */
static inline void e_exit(int code)
{
    syscall(SYS_exit, code);
    __builtin_unreachable();
}

/* tiny write helper */
static inline ssize_t e_write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}

static inline void println(const char *s)
{
    if (!s) return;

    size_t len = 0;
    while (s[len]) len++;

    e_write(1, s, len);
    e_write(1, "\n", 1);
}

static inline ssize_t e_read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}

/* raw syscall escape hatch */
#define easy_sysc(name, ...) syscall(SYS_##name, __VA_ARGS__)

/* clean file delete */
static inline int clearn(const char *path)
{
    if (!path) return -1;
    return (int)syscall(SYS_unlink, path);
}

/* process helpers */
static inline pid_t e_fork(void)
{
    return fork();
}

static inline int e_wait(pid_t pid, int *status)
{
    return waitpid(pid, status, 0);
}

static inline int e_execv(const char *path, char *const argv[])
{
    execv(path, argv);
    return -1;
}

static inline int e_execvp(const char *file, char *const argv[])
{
    execvp(file, argv);
    return -1;
}

static inline int e_execve(const char *path, char *const argv[], char *const envp[])
{
    execve(path, argv, envp);
    return -1;
}

/* lighter system() style helper */
static inline int e_system(const char *cmd)
{
    if (!cmd) return -1;

    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127);
    }

    if (pid < 0) return -1;

    int status = 0;
    if (waitpid(pid, &status, 0) < 0)
        return -1;

    return status;
}

#ifndef SYS_write
#define SYS_write 1
#endif

#ifndef SYS_exit
#define SYS_exit 60
#endif

#ifndef SYS_unlink
#define SYS_unlink 87
#endif

#endif
/* End the commands */
