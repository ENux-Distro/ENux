/*
 * epm - epm Package Manager
 * A lightweight package manager using the .epm archive format.
 *
 */

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <libgen.h>
#include <limits.h>

#define EPM_VERSION        "1.0.0"
#define EPM_VAR_DIR        "/var/epm"
#define EPM_INSTALLED_DIR  "/var/epm/installed"
#define EPM_LOGS_DIR       "/var/epm/logs"
#define EPM_CACHE_DIR      "/var/epm/cache"
#define EPM_ETC_DIR        "/etc/epm"
#define EPM_MIRROR_LIST    "/etc/epm/mirror.list"
#define EPM_TMP_BASE       "/tmp/epm_work"
#define MAX_LINE           4096
#define MAX_PATH           PATH_MAX
/* Working buffer: two MAX_PATH components + separator + NUL */
#define MAX_BUF            (MAX_PATH * 2 + 4)

#define COL_RESET   "\033[0m"
#define COL_BOLD    "\033[1m"
#define COL_RED     "\033[1;31m"
#define COL_GREEN   "\033[1;32m"
#define COL_YELLOW  "\033[1;33m"
#define COL_CYAN    "\033[1;36m"
#define COL_BLUE    "\033[1;34m"

static void epm_log(const char *level_col, const char *prefix,
                    const char *fmt, va_list ap)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char ts[32];
    strftime(ts, sizeof ts, "%Y-%m-%d %H:%M:%S", tm_info);

    /* console — va_copy so ap stays valid for file write below */
    va_list ap_con;
    va_copy(ap_con, ap);
    fprintf(stderr, "%s%s%s %s%s%s ",
            COL_BOLD, level_col, prefix, COL_RESET, COL_BOLD, ts);
    vfprintf(stderr, fmt, ap_con);
    fprintf(stderr, "%s\n", COL_RESET);
    va_end(ap_con);

    /* log file */
    struct stat st;
    if (stat(EPM_LOGS_DIR, &st) == 0 && S_ISDIR(st.st_mode)) {
        char logpath[MAX_PATH];
        snprintf(logpath, sizeof logpath, "%s/epm.log", EPM_LOGS_DIR);
        FILE *lf = fopen(logpath, "a");
        if (lf) {
            va_list ap_log;
            va_copy(ap_log, ap);
            fprintf(lf, "[%s] [%s] ", ts, prefix);
            vfprintf(lf, fmt, ap_log);
            fputc('\n', lf);
            va_end(ap_log);
            fclose(lf);
        }
    }
}

static void info(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    epm_log(COL_CYAN,   "INFO ", fmt, ap); va_end(ap);
}
static void ok(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    epm_log(COL_GREEN,  "OK   ", fmt, ap); va_end(ap);
}
static void warn(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    epm_log(COL_YELLOW, "WARN ", fmt, ap); va_end(ap);
}
static void die(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    epm_log(COL_RED,    "ERROR", fmt, ap); va_end(ap);
    exit(EXIT_FAILURE);
}

static int mkdirp(const char *path, mode_t mode)
{
    char tmp[MAX_PATH];
    snprintf(tmp, sizeof tmp, "%s", path);
    size_t len = strlen(tmp);
    if (len && tmp[len - 1] == '/') tmp[len - 1] = '\0';
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, mode) != 0 && errno != EEXIST) return -1;
            *p = '/';
        }
    }
    if (mkdir(tmp, mode) != 0 && errno != EEXIST) return -1;
    return 0;
}

static int path_exists(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0;
}

/* run a shell command; return exit status */
static int run_cmd(const char *cmd)
{
    pid_t pid = fork();
    if (pid < 0) { warn("fork() failed: %s", strerror(errno)); return -1; }
    if (pid == 0) { execl("/bin/sh", "sh", "-c", cmd, (char *)NULL); _exit(127); }
    int ws;
    waitpid(pid, &ws, 0);
    return WIFEXITED(ws) ? WEXITSTATUS(ws) : -1;
}

/* create /tmp/epm_work_<pid> */
static char *make_tmpdir(void)
{
    static char buf[MAX_PATH];
    snprintf(buf, sizeof buf, "%s_%d", EPM_TMP_BASE, (int)getpid());
    if (mkdirp(buf, 0700) != 0) {
        warn("Cannot create temp dir %s: %s", buf, strerror(errno));
        return NULL;
    }
    return buf;
}

static void rmtree(const char *path)
{
    char cmd[MAX_PATH + 8];
    snprintf(cmd, sizeof cmd, "rm -rf -- %s", path);
    run_cmd(cmd);
}

/* strip trailing slash from a URL copy */
static void strip_trailing_slash(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && s[n - 1] == '/') s[--n] = '\0';
}

static void ensure_dirs(void)
{
    const char *dirs[] = {
        EPM_VAR_DIR, EPM_INSTALLED_DIR, EPM_LOGS_DIR,
        EPM_CACHE_DIR, EPM_ETC_DIR, NULL
    };
    for (int i = 0; dirs[i]; i++)
        if (mkdirp(dirs[i], 0755) != 0 && errno != EEXIST)
            die("Cannot create directory %s: %s", dirs[i], strerror(errno));

    if (!path_exists(EPM_MIRROR_LIST)) {
        FILE *f = fopen(EPM_MIRROR_LIST, "w");
        if (f) {
            fprintf(f, "# epm mirror list\n");
            fprintf(f, "# One URL per line. Lines starting with # are ignored.\n");
            fprintf(f, "https://sourceforge.net/projects/epm-repo/files/repo/\n");
            fclose(f);
            info("Created default mirror list at %s", EPM_MIRROR_LIST);
        }
    }
}


/* "path/to/yes.epm" → "yes"   |   "yes" → "yes" */
static const char *pkg_name_from_path(const char *path)
{
    static char buf[MAX_PATH];
    char tmp[MAX_PATH];
    snprintf(tmp, sizeof tmp, "%s", path);
    snprintf(buf, sizeof buf, "%s", basename(tmp));
    size_t len = strlen(buf);
    if (len > 4 && strcmp(buf + len - 4, ".epm") == 0)
        buf[len - 4] = '\0';
    return buf;
}

/* returns 1 if arg ends with ".epm" AND the file exists locally */
static int is_local_pkg(const char *arg)
{
    size_t len = strlen(arg);
    if (len > 4 && strcmp(arg + len - 4, ".epm") == 0 && path_exists(arg))
        return 1;
    return 0;
}


static const char *download_from_mirrors(const char *pkg_name)
{
    static char cached[MAX_PATH];
    snprintf(cached, sizeof cached, "%s/%s.epm", EPM_CACHE_DIR, pkg_name);

    /* prefer curl, fall back to wget */
    int have_curl = (run_cmd("which curl > /dev/null 2>&1") == 0);
    int have_wget = (run_cmd("which wget > /dev/null 2>&1") == 0);

    if (!have_curl && !have_wget)
        die("Neither curl nor wget found. Install one to download packages.");

    FILE *mf = fopen(EPM_MIRROR_LIST, "r");
    if (!mf) die("Cannot open mirror list %s: %s", EPM_MIRROR_LIST, strerror(errno));

    char line[MAX_LINE];
    int tried = 0;

    while (fgets(line, sizeof line, mf)) {
        /* strip newline */
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (!line[0] || line[0] == '#') continue;

        strip_trailing_slash(line);

        /* build the full URL: <mirror>/<pkg>.epm */
        char url[MAX_LINE];
        snprintf(url, sizeof url, "%s/%s.epm", line, pkg_name);

        info("Trying mirror: %s", url);
        tried++;

        /* build download command */
        char dl_cmd[MAX_LINE * 2];
        if (have_curl) {
            /*
             * -f  = fail on HTTP errors (no 200 → non-zero exit)
             * -L  = follow redirects (SourceForge uses these heavily)
             * -#  = progress bar (nicer than -s silence)
             * -o  = output file
             */
            snprintf(dl_cmd, sizeof dl_cmd,
                     "curl -fL# --max-time 120 -o %s -- \"%s\" 2>&1",
                     cached, url);
        } else {
            snprintf(dl_cmd, sizeof dl_cmd,
                     "wget --timeout=120 -O %s -- \"%s\" 2>&1",
                     cached, url);
        }

        if (run_cmd(dl_cmd) == 0 && path_exists(cached)) {
            fclose(mf);
            ok("Downloaded %s.epm from %s", pkg_name, line);
            return cached;
        }

        /* remove a partial/empty file before trying the next mirror */
        unlink(cached);
        warn("Mirror failed: %s", line);
    }
    fclose(mf);

    if (tried == 0)
        die("No mirrors configured in %s", EPM_MIRROR_LIST);

    return NULL;  /* all mirrors failed */
}


static void collect_files(const char *base, const char *rel, FILE *manifest)
{
    char full[MAX_BUF];
    snprintf(full, sizeof full, "%s/%s", base, rel[0] ? rel : "");

    DIR *d = opendir(full);
    if (!d) return;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

        char child_rel[MAX_BUF];
        if (rel[0])
            snprintf(child_rel, sizeof child_rel, "%s/%s", rel, ent->d_name);
        else
            snprintf(child_rel, sizeof child_rel, "%s", ent->d_name);

        char child_full[MAX_BUF];
        snprintf(child_full, sizeof child_full, "%s/%s", base, child_rel);

        struct stat st;
        if (lstat(child_full, &st) != 0) continue;

        if (S_ISDIR(st.st_mode))
            collect_files(base, child_rel, manifest);
        else
            fprintf(manifest, "/%s\n", child_rel);
    }
    closedir(d);
}


static int install_local(const char *pkg_path)
{
    const char *pkg_name = pkg_name_from_path(pkg_path);
    info("Installing package: %s%s%s", COL_BOLD, pkg_name, COL_RESET);

    /* ── already installed? ── */
    char installed_record[MAX_PATH];
    snprintf(installed_record, sizeof installed_record,
             "%s/%s", EPM_INSTALLED_DIR, pkg_name);
    if (path_exists(installed_record)) {
        warn("Package '%s' is already installed. Purge it first to reinstall.",
             pkg_name);
        return 1;
    }

    /* ── extract ── */
    char *tmpdir = make_tmpdir();
    if (!tmpdir) return 1;

    char extract_cmd[MAX_BUF];
    snprintf(extract_cmd, sizeof extract_cmd,
             "tar -xf %s -C %s 2>&1", pkg_path, tmpdir);

    info("Extracting archive...");
    if (run_cmd(extract_cmd) != 0) {
        warn("tar extraction failed for: %s", pkg_path);
        rmtree(tmpdir);
        return 1;
    }

    /* find the single top-level directory */
    DIR *d = opendir(tmpdir);
    if (!d) { rmtree(tmpdir); die("Cannot open temp dir: %s", tmpdir); }

    char pkg_dir[MAX_BUF] = {0};
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
        char candidate[MAX_BUF];
        snprintf(candidate, sizeof candidate, "%s/%s", tmpdir, ent->d_name);
        struct stat st;
        if (stat(candidate, &st) == 0 && S_ISDIR(st.st_mode)) {
            snprintf(pkg_dir, sizeof pkg_dir, "%s", candidate);
            break;
        }
    }
    closedir(d);

    if (!pkg_dir[0]) {
        rmtree(tmpdir);
        die("Malformed .epm: no top-level directory inside archive.");
    }

    /* ── Control file ── */
    char control_path[MAX_BUF];
    snprintf(control_path, sizeof control_path, "%s/Control", pkg_dir);
    if (!path_exists(control_path)) {
        rmtree(tmpdir);
        die("Malformed .epm: missing Control file.");
    }
    char chmod_cmd[MAX_BUF];
    snprintf(chmod_cmd, sizeof chmod_cmd, "chmod +x -- %s", control_path);
    run_cmd(chmod_cmd);

    /* ── run Control ── */
    info("Running Control script...");
    char run_control[MAX_BUF];
    snprintf(run_control, sizeof run_control,
             "cd -- %s && sh ./Control 2>&1", pkg_dir);
    int rc = run_cmd(run_control);
    if (rc != 0) {
        rmtree(tmpdir);
        die("Control script exited %d for package '%s'", rc, pkg_name);
    }
    ok("Control script completed successfully.");

    /* ── write manifest ── */
    FILE *mf = fopen(installed_record, "w");
    if (!mf) {
        rmtree(tmpdir);
        die("Cannot create install record at %s: %s",
            installed_record, strerror(errno));
    }

    DIR *pd = opendir(pkg_dir);
    if (!pd) { fclose(mf); rmtree(tmpdir); die("Cannot open package dir."); }
    while ((ent = readdir(pd)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") ||
            !strcmp(ent->d_name, "Control")) continue;
        char child_full[MAX_BUF];
        snprintf(child_full, sizeof child_full, "%s/%s", pkg_dir, ent->d_name);
        struct stat st;
        if (lstat(child_full, &st) != 0) continue;
        if (S_ISDIR(st.st_mode))
            collect_files(pkg_dir, ent->d_name, mf);
        else
            fprintf(mf, "/%s\n", ent->d_name);
    }
    closedir(pd);
    fclose(mf);

    rmtree(tmpdir);
    ok("Package '%s' installed successfully.", pkg_name);
    info("Install record: %s", installed_record);
    return 0;
}

/* ── public cmd_install: resolves local file OR downloads ── */
static int cmd_install(const char *arg)
{
    /* Case 1 — caller gave an explicit local .epm file */
    if (is_local_pkg(arg)) {
        return install_local(arg);
    }

    /* Case 2 — arg is a plain package name (or a .epm that doesn't exist yet):
       strip the .epm suffix if present so we have a bare name, then download. */
    char pkg_name[MAX_PATH];
    snprintf(pkg_name, sizeof pkg_name, "%s", arg);
    size_t len = strlen(pkg_name);
    if (len > 4 && strcmp(pkg_name + len - 4, ".epm") == 0)
        pkg_name[len - 4] = '\0';

    info("Package '%s' not found locally — searching mirrors...", pkg_name);

    const char *cached = download_from_mirrors(pkg_name);
    if (!cached)
        die("Package '%s' could not be downloaded from any mirror.", pkg_name);

    int ret = install_local(cached);

    /* leave the cached .epm so reinstalls are fast (user can epm clean to purge) */
    return ret;
}

static int cmd_purge(const char *pkg_name)
{
    char record_path[MAX_PATH];
    snprintf(record_path, sizeof record_path,
             "%s/%s", EPM_INSTALLED_DIR, pkg_name);

    if (!path_exists(record_path))
        die("Package '%s' is not installed (no record at %s).",
            pkg_name, record_path);

    info("Purging package: %s%s%s", COL_BOLD, pkg_name, COL_RESET);

    FILE *f = fopen(record_path, "r");
    if (!f) die("Cannot open install record: %s", strerror(errno));

    char line[MAX_PATH];
    int removed = 0, failed = 0;

    while (fgets(line, sizeof line, f)) {
        size_t ln = strlen(line);
        while (ln > 0 && (line[ln-1] == '\n' || line[ln-1] == '\r'))
            line[--ln] = '\0';
        if (!line[0]) continue;

        struct stat st;
        if (lstat(line, &st) != 0) { warn("File already gone: %s", line); continue; }
        if (S_ISDIR(st.st_mode))   { warn("Skipping directory: %s",  line); continue; }

        if (unlink(line) == 0) { info("Removed: %s", line); removed++; }
        else { warn("Cannot remove %s: %s", line, strerror(errno)); failed++; }
    }
    fclose(f);

    if (unlink(record_path) != 0)
        warn("Could not remove install record: %s", strerror(errno));

    if (failed == 0)
        ok("Package '%s' purged. %d file(s) removed.", pkg_name, removed);
    else
        warn("Package '%s' purged with %d error(s). %d file(s) removed.",
             pkg_name, failed, removed);
    return failed ? 1 : 0;
}

static int ping_url(const char *url)
{
    char cmd[MAX_LINE * 2];
    if (run_cmd("which curl > /dev/null 2>&1") == 0) {
        snprintf(cmd, sizeof cmd,
                 "curl -fsS --max-time 10 --head -- \"%s\" > /dev/null 2>&1", url);
    } else {
        snprintf(cmd, sizeof cmd,
                 "wget -q --spider --timeout=10 -- \"%s\" > /dev/null 2>&1", url);
    }
    return run_cmd(cmd);
}

static int cmd_sync(void)
{
    if (!path_exists(EPM_MIRROR_LIST))
        die("Mirror list not found: %s", EPM_MIRROR_LIST);

    FILE *f = fopen(EPM_MIRROR_LIST, "r");
    if (!f) die("Cannot open mirror list: %s", strerror(errno));

    char line[MAX_LINE];
    int total = 0, ok_count = 0, fail_count = 0;
    info("Syncing mirrors from %s ...", EPM_MIRROR_LIST);

    while (fgets(line, sizeof line, f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (!line[0] || line[0] == '#') continue;

        total++;
        info("Checking mirror: %s", line);
        if (ping_url(line) == 0) { ok("Mirror reachable: %s", line); ok_count++; }
        else {
            fprintf(stderr, "%sERROR%s Mirror failed: %s%s%s\n",
                    COL_RED, COL_RESET, COL_BOLD, line, COL_RESET);
            fail_count++;
        }
    }
    fclose(f);

    if (total == 0) { warn("No mirrors configured in %s", EPM_MIRROR_LIST); return 1; }

    fprintf(stderr, "\n%s==> Sync complete.%s %s%d/%d%s mirror(s) reachable.\n",
            COL_BOLD, COL_RESET,
            fail_count ? COL_YELLOW : COL_GREEN,
            ok_count, total, COL_RESET);
    return fail_count ? 1 : 0;
}

static int cmd_clean(void)
{
    int any = 0;

    if (path_exists(EPM_LOGS_DIR)) {
        info("Removing logs: %s", EPM_LOGS_DIR);
        rmtree(EPM_LOGS_DIR);
        if (path_exists(EPM_LOGS_DIR)) { warn("Failed to remove %s", EPM_LOGS_DIR); }
        else { ok("Logs removed."); any++; }
    } else {
        info("Logs directory already absent.");
    }

    if (path_exists(EPM_CACHE_DIR)) {
        info("Removing package cache: %s", EPM_CACHE_DIR);
        rmtree(EPM_CACHE_DIR);
        if (path_exists(EPM_CACHE_DIR)) { warn("Failed to remove %s", EPM_CACHE_DIR); }
        else { ok("Package cache removed."); any++; }
    } else {
        info("Cache directory already absent.");
    }

    if (!any) info("Nothing to clean.");
    return 0;
}


static void usage(void)
{
    fprintf(stderr,
        "%sepm%s v%s — epm Package Manager\n\n"
        "Usage:\n"
        "  %sepm install%s <name>          Download & install from mirrors\n"
        "  %sepm install%s <pkg.epm>       Install a local .epm file\n"
        "  %sepm purge%s  <name>           Remove an installed package\n"
        "  %sepm sync%s                    Check all configured mirrors\n"
        "  %sepm clean%s                   Remove logs and package cache\n"
        "\nConfiguration:\n"
        "  Mirror list  : %s\n"
        "  Install DB   : %s\n"
        "  Package cache: %s\n"
        "  Logs         : %s\n",
        COL_BOLD, COL_RESET, EPM_VERSION,
        COL_CYAN, COL_RESET,
        COL_CYAN, COL_RESET,
        COL_CYAN, COL_RESET,
        COL_CYAN, COL_RESET,
        COL_CYAN, COL_RESET,
        EPM_MIRROR_LIST, EPM_INSTALLED_DIR, EPM_CACHE_DIR, EPM_LOGS_DIR);
}

int main(int argc, char *argv[])
{
    if (argc < 2) { usage(); return EXIT_FAILURE; }

    const char *cmd = argv[1];
    int needs_root = (strcmp(cmd, "install") == 0 ||
                      strcmp(cmd, "purge")   == 0 ||
                      strcmp(cmd, "clean")   == 0);

    if (needs_root && geteuid() != 0)
        die("This operation requires root privileges. Run as root.");

    ensure_dirs();

    if (strcmp(cmd, "install") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: epm install <package|package.epm>\n"); return EXIT_FAILURE; }
        return cmd_install(argv[2]);

    } else if (strcmp(cmd, "purge") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: epm purge <package_name>\n"); return EXIT_FAILURE; }
        return cmd_purge(argv[2]);

    } else if (strcmp(cmd, "sync") == 0) {
        return cmd_sync();

    } else if (strcmp(cmd, "clean") == 0) {
        return cmd_clean();

    } else {
        fprintf(stderr, "%sUnknown command: %s%s\n", COL_RED, cmd, COL_RESET);
        usage();
        return EXIT_FAILURE;
    }
}
