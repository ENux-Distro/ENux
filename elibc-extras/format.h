/*
 * format.h - Human-readable format specifiers for E-Libc
 * Part of E-Libc, a fork of musl libc
 *
 * Instead of memorizing cryptic format specifiers like %d, %ld, %llu,
 * just use the type name directly inside eprintf() and friends.
 *
 * IMPORTANT — do NOT use these specifiers with raw printf/sprintf/etc.
 * Standard printf interprets %i, %l, %s, %f, %d, %u, %p, %o, %h, %a,
 * %c as its own format characters, so "%int" becomes "%i" + "nt",
 * "%long" becomes "%l" + "ong", etc. and you get garbage output.
 *
 */

#ifndef _ELIBC_FORMAT_H
#define _ELIBC_FORMAT_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 *     SECTION 1: FMT_* compile-time string constants
 *
 *  These expand to standard C specifiers and are safe to
 *  use with plain printf via adjacent string literals:
 *
 *    printf("x=" FMT_INT " name=" FMT_STR "\n", x, name);
 *
 *  Do NOT embed them as "%int" in a raw printf string —
 *  use eprintf() for that syntax instead (see Section 4).
 */

/* Integer */
#define FMT_INT         "%d"
#define FMT_UINT        "%u"
#define FMT_SHORT       "%hd"
#define FMT_USHORT      "%hu"
#define FMT_LONG        "%ld"
#define FMT_ULONG       "%lu"
#define FMT_LLONG       "%lld"
#define FMT_ULLONG      "%llu"

/* Explicit-width integers */
#define FMT_INT8        "%d"
#define FMT_INT16       "%d"
#define FMT_INT32       "%d"
#define FMT_INT64       "%lld"
#define FMT_UINT8       "%u"
#define FMT_UINT16      "%u"
#define FMT_UINT32      "%u"
#define FMT_UINT64      "%llu"

/* Float */
#define FMT_FLOAT       "%f"
#define FMT_DOUBLE      "%lf"
#define FMT_LDOUBLE     "%Lf"
#define FMT_FLOAT_SCI   "%e"
#define FMT_DOUBLE_SCI  "%le"
#define FMT_FLOAT_AUTO  "%g"
#define FMT_DOUBLE_AUTO "%lg"

/* Character / String */
#define FMT_CHAR        "%c"
#define FMT_STR         "%s"
#define FMT_STRING      "%s"

/* Boolean (0 or 1 — see fmt_bool() for "true"/"false") */
#define FMT_BOOL        "%d"

/* Pointer */
#define FMT_PTR         "%p"
#define FMT_ADDR        "%p"

/* Size */
#define FMT_SIZE        "%zu"
#define FMT_SSIZE       "%zd"
#define FMT_PTRDIFF     "%td"

/* Hex */
#define FMT_HEX         "%x"
#define FMT_HEX_UPPER   "%X"
#define FMT_HEX8        "%02x"
#define FMT_HEX16       "%04x"
#define FMT_HEX32       "%08x"
#define FMT_HEX64       "%016llx"

/* Octal */
#define FMT_OCT         "%o"

/* Percent literal */
#define FMT_PERCENT     "%%"


/*
 *          SECTION 2: Runtime format string translation
 *
 *  fmt_translate(src, dst, dst_size)
 *    Translates a format string with human-readable specifiers
 *    into a standard C format string.
 *
 *    src      — input string (read-only)
 *    dst      — output buffer
 *    dst_size — size of output buffer in bytes
 *
 *    Returns dst on success, NULL if dst_size was too small
 *    or src/dst were NULL.
 *
 *  Supported specifiers (checked longest-first to avoid
 *  prefix collisions — %longlong before %long, etc.):
 *
 *    %longdouble  %longlong   %ulonglong  %ushort     %uchar
 *    %uint64      %uint32     %uint16     %uint8
 *    %int64       %int32      %int16      %int8
 *    %ulong       %uint       %long       %short      %int
 *    %float_sci   %float_auto %double_sci %double_auto
 *    %double      %float      %longdouble
 *    %char[]      %charptr    %string     %char
 *    %bool        %pointer    %ptr        %addr
 *    %ssize       %size       %ptrdiff    %intptr
 *    %hex64       %hex32      %hex16      %hex8  %HEX  %hex
 *    %octal       %percent
 */

typedef struct {
    const char *from;
    const char *to;
} _fmt_entry_t;

/*
 * Translation table.
 *
 * ORDER IS CRITICAL — longer / more-specific entries must come before
 * any entry whose key is a prefix of theirs.  The translator walks this
 * table front-to-back and takes the first match, so if "%long" appeared
 * before "%longlong" it would always win.
 *
 * Danger pairs handled here:
 *   %longlong  before  %long
 *   %ulonglong before  %ulong
 *   %longdouble before %long
 *   %uint64/32/16/8 before %uint
 *   %int64/32/16/8  before %int
 *   %float_sci / %float_auto  before  %float
 *   %double_sci / %double_auto before %double
 *   %char[]  before  %char
 *   %pointer before  %ptr
 *   %ssize   before  %size
 *   %hex64/32/16/8 before %hex
 *   %ptrdiff / %intptr / %percent  (unique, no collision, but kept last
 *                                   to not slow down the common cases)
 */
static const _fmt_entry_t _fmt_table[] = {
    /* --- long variants (longest first) --- */
    { "%longdouble",  "%Lf"       },
    { "%longlong",    "%lld"      },
    { "%ulonglong",   "%llu"      },

    /* --- u-prefixed short variants --- */
    { "%ushort",      "%hu"       },
    { "%uchar",       "%c"        },

    /* --- explicit-width integers (width suffix before plain) --- */
    { "%uint64",      "%llu"      },
    { "%uint32",      "%u"        },
    { "%uint16",      "%u"        },
    { "%uint8",       "%u"        },
    { "%int64",       "%lld"      },
    { "%int32",       "%d"        },
    { "%int16",       "%d"        },
    { "%int8",        "%d"        },

    /* --- plain integer types --- */
    { "%ulong",       "%lu"       },
    { "%uint",        "%u"        },
    { "%long",        "%ld"       },
    { "%short",       "%hd"       },
    { "%int",         "%d"        },

    /* --- float variants (suffixed before plain) --- */
    { "%float_sci",   "%e"        },
    { "%float_auto",  "%g"        },
    { "%double_sci",  "%le"       },
    { "%double_auto", "%lg"       },
    { "%double",      "%lf"       },
    { "%float",       "%f"        },

    /* --- char / string (%char[] before %char) --- */
    { "%char[]",      "%s"        },
    { "%charptr",     "%s"        },
    { "%string",      "%s"        },
    { "%char",        "%c"        },

    /* --- bool --- */
    { "%bool",        "%d"        },

    /* --- pointer (%pointer before %ptr) --- */
    { "%pointer",     "%p"        },
    { "%ptr",         "%p"        },
    { "%addr",        "%p"        },

    /* --- size types (%ssize before %size) --- */
    { "%ssize",       "%zd"       },
    { "%size",        "%zu"       },
    { "%ptrdiff",     "%td"       },
    { "%intptr",      "%td"       },

    /* --- hex (widths before plain, %HEX uppercase) --- */
    { "%hex64",       "%016llx"   },
    { "%hex32",       "%08x"      },
    { "%hex16",       "%04x"      },
    { "%hex8",        "%02x"      },
    { "%HEX",         "%X"        },
    { "%hex",         "%x"        },

    /* --- octal / percent --- */
    { "%octal",       "%o"        },
    { "%percent",     "%%"        },

    { NULL, NULL }
};

/*
 * _fmt_is_word_end — returns 1 if the character cannot be part of a
 * type-name identifier.  We use this to guarantee longest-match:
 * "%intptr" must NOT match "%int" because 'p' continues the token.
 *
 * Allowed mid-token characters: letters, digits, '_', '[', ']'.
 * Everything else ends the token and allows a match to fire.
 */
static inline int _fmt_is_word_end(char c)
{
    return !( (c >= 'a' && c <= 'z') ||
              (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') ||
               c == '_' || c == '[' || c == ']' );
}

/*
 * fmt_translate — translate human-readable specifiers to standard C ones.
 */
static inline char *fmt_translate(const char *src, char *dst, size_t dst_size)
{
    const char *s   = src;
    char       *d   = dst;
    char       *end = dst + dst_size - 1;

    if (!src || !dst || dst_size == 0)
        return NULL;

    while (*s) {
        if (*s == '%') {
            int matched = 0;
            for (int i = 0; _fmt_table[i].from != NULL; i++) {
                size_t from_len = strlen(_fmt_table[i].from);
                if (strncmp(s, _fmt_table[i].from, from_len) == 0 &&
                    _fmt_is_word_end(s[from_len]))
                {
                    const char *rep    = _fmt_table[i].to;
                    size_t      rep_len = strlen(rep);
                    if (d + rep_len > end)
                        return NULL;
                    memcpy(d, rep, rep_len);
                    d += rep_len;
                    s += from_len;
                    matched = 1;
                    break;
                }
            }
            if (!matched) {
                if (d >= end) return NULL;
                *d++ = *s++;
            }
        } else {
            if (d >= end) return NULL;
            *d++ = *s++;
        }
    }

    *d = '\0';
    return dst;
}


/*
 *   SECTION 3: eprintf / efprintf / esprintf / esnprintf
 *
 *  Drop-in replacements for the printf family that accept
 *  human-readable specifiers directly in the format string.
 *
 *  These call fmt_translate internally, then forward to the
 *  real printf.  The translation buffer is allocated on the
 *  stack (up to FMT_TRANSLATE_STACK_MAX bytes); larger
 *  format strings fall back to heap allocation.
 *
 *  Usage:
 *    int   age  = 15;
 *    float temp = 98.6f;
 *    char  name[] = "Alice";
 *
 *    eprintf("Name: %char[]  Age: %int  Temp: %float\n",
 *            name, age, temp);
 *
 *  All standard printf features (width, precision, flags)
 *  still work — they pass through the translator unchanged:
 *    eprintf("%-10char[]  %08int\n", name, age);
 */

#ifndef FMT_TRANSLATE_STACK_MAX
#  define FMT_TRANSLATE_STACK_MAX 1024
#endif

/*
 * _fmt_translate_auto — translate src into a stack or heap buffer.
 * Caller must call _fmt_free_auto(buf, stack_buf) when done.
 */
static inline char *_fmt_translate_auto(const char *src,
                                        char        stack_buf[FMT_TRANSLATE_STACK_MAX],
                                        int        *used_heap)
{
    size_t src_len = strlen(src);
    /* Worst-case expansion: each 1-char specifier like %i maps to
     * at most 8 chars (%016llx).  So 8× src_len is a safe upper bound. */
    size_t need = src_len * 8 + 1;
    *used_heap = 0;

    if (need <= FMT_TRANSLATE_STACK_MAX) {
        return fmt_translate(src, stack_buf, FMT_TRANSLATE_STACK_MAX);
    }

    char *heap = (char *)malloc(need);
    if (!heap) return NULL;
    *used_heap = 1;
    if (!fmt_translate(src, heap, need)) {
        free(heap);
        return NULL;
    }
    return heap;
}

static inline void _fmt_free_auto(char *buf, char *stack_buf, int used_heap)
{
    if (used_heap && buf != stack_buf)
        free(buf);
}

/*
 * eprintf — printf with human-readable specifiers.
 */
static inline int eprintf(const char *fmt, ...)
{
    char  stack[FMT_TRANSLATE_STACK_MAX];
    int   heap = 0;
    char *tfmt = _fmt_translate_auto(fmt, stack, &heap);
    if (!tfmt) return -1;

    va_list ap;
    va_start(ap, fmt);
    int ret = vprintf(tfmt, ap);
    va_end(ap);

    _fmt_free_auto(tfmt, stack, heap);
    return ret;
}

/*
 * efprintf — fprintf with human-readable specifiers.
 */
static inline int efprintf(FILE *stream, const char *fmt, ...)
{
    char  stack[FMT_TRANSLATE_STACK_MAX];
    int   heap = 0;
    char *tfmt = _fmt_translate_auto(fmt, stack, &heap);
    if (!tfmt) return -1;

    va_list ap;
    va_start(ap, fmt);
    int ret = vfprintf(stream, tfmt, ap);
    va_end(ap);

    _fmt_free_auto(tfmt, stack, heap);
    return ret;
}

/*
 * esprintf — sprintf with human-readable specifiers.
 */
static inline int esprintf(char *buf, const char *fmt, ...)
{
    char  stack[FMT_TRANSLATE_STACK_MAX];
    int   heap = 0;
    char *tfmt = _fmt_translate_auto(fmt, stack, &heap);
    if (!tfmt) return -1;

    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(buf, tfmt, ap);
    va_end(ap);

    _fmt_free_auto(tfmt, stack, heap);
    return ret;
}

/*
 * esnprintf — snprintf with human-readable specifiers.
 */
static inline int esnprintf(char *buf, size_t size, const char *fmt, ...)
{
    char  stack[FMT_TRANSLATE_STACK_MAX];
    int   heap = 0;
    char *tfmt = _fmt_translate_auto(fmt, stack, &heap);
    if (!tfmt) return -1;

    va_list ap;
    va_start(ap, fmt);
    int ret = vsnprintf(buf, size, tfmt, ap);
    va_end(ap);

    _fmt_free_auto(tfmt, stack, heap);
    return ret;
}


/*
 *  SECTION 4: Helper utilities
 */

/*
 * fmt_bool(val) — returns "true" or "false".
 *   eprintf("ok = %char[]\n", fmt_bool(result));
 */
static inline const char *fmt_bool(int val)
{
    return val ? "true" : "false";
}

/*
 * fmt_yn(val) — returns "yes" or "no".
 */
static inline const char *fmt_yn(int val)
{
    return val ? "yes" : "no";
}

/*
 * fmt_nullstr(s) — returns s if non-NULL, else "(null)".
 *   Prevents UB when printing a NULL char*.
 */
static inline const char *fmt_nullstr(const char *s)
{
    return s ? s : "(null)";
}


/*
 *  SECTION 5: Typed single-value print helpers
 *
 *  print_int(42)               → "42\n"
 *  print_labeled_int("x", 42)  → "x = 42\n"
 */

static inline void print_int(int v)              { printf("%d\n",     v); }
static inline void print_uint(unsigned v)        { printf("%u\n",     v); }
static inline void print_long(long v)            { printf("%ld\n",    v); }
static inline void print_ulong(unsigned long v)  { printf("%lu\n",    v); }
static inline void print_llong(long long v)      { printf("%lld\n",   v); }
static inline void print_float(float v)          { printf("%f\n",     v); }
static inline void print_double(double v)        { printf("%lf\n",    v); }
static inline void print_char(char v)            { printf("%c\n",     v); }
static inline void print_str(const char *v)      { printf("%s\n",     fmt_nullstr(v)); }
static inline void print_bool(int v)             { printf("%s\n",     fmt_bool(v)); }
static inline void print_ptr(const void *v)      { printf("%p\n",     v); }
static inline void print_size(size_t v)          { printf("%zu\n",    v); }
static inline void print_hex(unsigned int v)     { printf("0x%x\n",   v); }
static inline void print_hex32(uint32_t v)       { printf("0x%08x\n", v); }
static inline void print_hex64(uint64_t v)       { printf("0x%016llx\n", (unsigned long long)v); }

static inline void print_labeled_int(const char *n, int v)
    { printf("%s = %d\n",  n, v); }
static inline void print_labeled_uint(const char *n, unsigned v)
    { printf("%s = %u\n",  n, v); }
static inline void print_labeled_long(const char *n, long v)
    { printf("%s = %ld\n", n, v); }
static inline void print_labeled_float(const char *n, float v)
    { printf("%s = %f\n",  n, v); }
static inline void print_labeled_double(const char *n, double v)
    { printf("%s = %lf\n", n, v); }
static inline void print_labeled_char(const char *n, char v)
    { printf("%s = %c\n",  n, v); }
static inline void print_labeled_str(const char *n, const char *v)
    { printf("%s = %s\n",  n, fmt_nullstr(v)); }
static inline void print_labeled_bool(const char *n, int v)
    { printf("%s = %s\n",  n, fmt_bool(v)); }
static inline void print_labeled_ptr(const char *n, const void *v)
    { printf("%s = %p\n",  n, v); }
static inline void print_labeled_hex(const char *n, unsigned int v)
    { printf("%s = 0x%x\n", n, v); }


/*
 *               SECTION 6: Quick reference
 * ----------------------------------------------------------
 *
 *  Use eprintf/efprintf/esprintf/esnprintf for %type syntax.
 *  Use plain printf + FMT_* constants for compile-time safety.
 *
 *  Human-readable     Standard C     Notes
 *  -------------------------------------------------------
 *  %int               %d
 *  %uint              %u
 *  %short             %hd
 *  %ushort            %hu
 *  %long              %ld
 *  %ulong             %lu
 *  %longlong          %lld
 *  %ulonglong         %llu
 *  %int8              %d             int8_t
 *  %int16             %d             int16_t
 *  %int32             %d             int32_t
 *  %int64             %lld           int64_t
 *  %uint8             %u             uint8_t
 *  %uint16            %u             uint16_t
 *  %uint32            %u             uint32_t
 *  %uint64            %llu           uint64_t
 *  %float             %f
 *  %double            %lf
 *  %longdouble        %Lf
 *  %float_sci         %e             scientific notation
 *  %double_sci        %le
 *  %float_auto        %g             auto notation
 *  %double_auto       %lg
 *  %char              %c             single character
 *  %uchar             %c             unsigned char
 *  %char[]            %s             string (char* / char[])
 *  %charptr           %s             same
 *  %string            %s             same
 *  %bool              %d             0 or 1; use fmt_bool() for text
 *  %ptr               %p
 *  %pointer           %p
 *  %addr              %p
 *  %size              %zu            size_t
 *  %ssize             %zd            ssize_t
 *  %ptrdiff           %td            ptrdiff_t
 *  %intptr            %td            intptr_t
 *  %hex               %x
 *  %HEX               %X             uppercase hex
 *  %hex8              %02x           zero-padded byte
 *  %hex16             %04x
 *  %hex32             %08x
 *  %hex64             %016llx
 *  %octal             %o
 *  %percent           %%             literal '%'
 *
 *  Width/precision flags work normally:
 *    eprintf("%10int\n", 42);        → "        42"
 *    eprintf("%-10char[]\n", "hi");  → "hi        "
 *    eprintf("%.4float\n", 3.14f);  → "3.1400"
 */


#define printf(...)    eprintf(__VA_ARGS__)
#define fprintf(...)   efprintf(__VA_ARGS__)
#define sprintf(...)   esprintf(__VA_ARGS__)
#define snprintf(...)  esnprintf(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
