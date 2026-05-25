# init.c

init.c replaces Bedrock Linux's shell-based init (`/bedrock/strata/bedrock/sbin/init`) with a minimal C + x86_64 ASM static binary. It does the same job: pivot into the chosen stratum, enable all strata, hand off to the real init, but it's ~10%* faster than Bedrock Linux's init.

## Why init.c?

Bedrock Linux's init is a shell script. At every boot it:

- Forks subprocesses for `grep`, `awk`, `sed`, `realpath`, `mount`
- Enables strata **sequentially** — one at a time, each taking several seconds
- Pipes data through chains of shell utilities

init.c replaces all of that with direct syscalls and parallel stratum enablement. The result: **the slowest single stratum sets the wall clock, not the sum of all of them**.

## How it works

```
main()
├── check_pid1()           → if not PID 1, exec backup immediately
├── ensure_essential_env() → mount /proc, /sys, /dev, /run
├── setup_term()           → configure TTY, kill plymouth
├── print_logo()           → Bedrock ASCII art
├── parse_bedrock_conf()   → read [init] section from bedrock.conf
├── scan_strata()          → list /bedrock/strata/*
├── run_menu()             → interactive stratum/init selection
├── mount_fstab()          → dmsetup, lvm, mount -a (direct syscalls)
├── pivot_root_to()        → bind-mount stratum + pivot_root
├── preenable_mounts()     → rbind /proc, /dev, /sys, /run
├── enable_strata_parallel() → fork ALL strata at once
│   ├── brl-repair bedrock (foreground)
│   ├── brl-repair <init>  (foreground)
│   └── brl-enable <N>…   (parallel — one child per stratum)
├── brl-apply              → apply configuration (forked)
└── execv(<real init>)     → PID 1 hands off, never returns
```

### Key design decisions

- **Statically linked** — no libc.so dependency; safe to run before ld.so is set up
- **Direct syscalls** — `mount(2)`, `pivot_root(2)`, `open(2)`/`read(2)` for everything; no `system()`, no `popen()`
- **Parallel enable** — all strata except bedrock and the init stratum are `brl-enable`'d simultaneously via `fork()` + `waitpid(-1)`
- **x86_64 ASM helpers** — `asm_memzero`, `asm_strcmp`, `asm_write_str` for hot paths and async-signal-safe panic output
- **No runtime downloads** — the Makefile never fetches files during install; `make install-fallback` is a separate manual step

## Dependencies

### Build-time

| Tool  | Purpose           |
|-------|-------------------|
| gcc   | C compilation     |
| nasm  | Assembly          |
| ld    | Linking (binutils)|
| **static** attr  | Handles fs objects |

All must target **x86_64 Linux**. The binary is statically linked and has no runtime library dependencies.

### Runtime

- Bedrock Linux system with `/bedrock/strata/*` strata directories
- `/bedrock/libexec/brl-repair`, `/bedrock/libexec/brl-enable`, `/bedrock/libexec/brl-apply`
- Kernel with `pivot_root(2)` support

## Installation

### 1. Build

```bash
make
```

Produces `build/init` (stripped, ~850KB ELF64).

### 2. Install

```bash
sudo make install
```

This will:

1. Back up the existing init at `/bedrock/strata/bedrock/sbin/init` → `.sh.bak` (shell script) or `.bin.bak` (binary)
2. Copy the backup to `/sbin/init` as a recovery fallback
3. Place the init.c binary at `/bedrock/strata/bedrock/sbin/init`

### 3. Fallback (optional)

If there was no existing init to back up, or you want a fresh copy of the official Bedrock init:

```bash
sudo make install-fallback
```

Downloads the official Bedrock Linux shell init and saves it as both the `.sh.bak` backup and `/sbin/init`.

### 4. Debug build

```bash
make debug
```

Produces `build/init.debug` with `-g3` and AddressSanitizer (unstripped).

### 5. Verify

```bash
make check
```

Validates ELF type, static linking, forbidden symbols, and ASM symbol presence.

## Booting

On next reboot:

1. init.c starts as PID 1
2. Mounts essential filesystems (/proc, /sys, /dev, /run)
3. Reads `/bedrock/etc/bedrock.conf` for init selection
4. Presents an interactive menu of available stratum/init combinations (with configurable timeout)
5. Pivots root into the chosen stratum
6. Enables all `show_boot` strata in parallel
7. Handles control to the stratum's real init via `execv()`

### Kernel command-line shortcut

```bash
bedrock_init=stratum:/sbin/init
```

Skips the menu entirely.

## Contributing

Contributions are welcome via pull request.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-change`)
3. Make your changes
4. Build and verify (`make clean && make && make check`)
5. Commit with a descriptive message
6. Open a pull request

### Code conventions

- C11 with GNU extensions (`-std=c11 -D_GNU_SOURCE`)
- No `system()`, `popen()`, or dynamic library calls
- Direct `write(2)` for output in signal-safe paths
- Static helper functions where possible
- Follow the existing style (Allman brace style, descriptive variable names)

## License

GPL-v3.

## Note

### *: May or may not be 10% all the time, but the boot speed was 18.42 seconds on my machine

### **init.c** was built and tested the most on **ENux 5.3.3**. If you want to use this init on other systems, be cautious as they haven't been tested yet.

### **init.c** will be the default init for **ENux** soon.

### All of the benchmarks, like timing, strata boot speeds and more, have been tested on a 7 strata **ENux** system.

### The system to test **init.c** consists of:

- i5 12400f (12 processing units)
- 32 GB DDR5 6000 MHz CL30
- Gen 4 NVME (with read/write speeds of around 5000/3500 MBs)
