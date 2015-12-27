//
// The MIT License (MIT)
//
// Copyright (c) 2015 Joakim Soderberg <joakim.soderberg@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include "cargo.h"
#include <stdarg.h>
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define strcasecmp _stricmp
#else // _WIN32 (Unix below)
#include <sys/ioctl.h>
#include <unistd.h>
#include <wordexp.h>
#endif // _WIN32

#ifdef __APPLE__
#include <AvailabilityMacros.h>
#endif // __APPLE__

#if defined(__STDC__)
# define C89
# if defined(__STDC_VERSION__)
#  define C90
#  if (__STDC_VERSION__ >= 199409L)
#   define C94
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define C99
#  endif
# endif
#endif

#ifdef _WIN32
#define CARGO_LONGLONG_FMT "I64d"
#define CARGO_ULONGLONG_FMT "I64u"
#else
#define CARGO_LONGLONG_FMT "lld"
#define CARGO_ULONGLONG_FMT "llu"
#endif

#ifdef C90
#include <math.h>
#define cargo_fabs fabs
#else
double cargo_fabs(double x)
{
    return (x < 0) ? -x : x;
}
#endif // C90


static cargo_malloc_f   replaced_cargo_malloc   = NULL;
static cargo_free_f     replaced_cargo_free     = NULL;
static cargo_realloc_f  replaced_cargo_realloc  = NULL;

void cargo_set_memfunctions(cargo_malloc_f malloc_replacement,
                            cargo_realloc_f realloc_replacement,
                            cargo_free_f free_replacement)
{
    replaced_cargo_malloc = malloc_replacement;
    replaced_cargo_realloc = realloc_replacement;
    replaced_cargo_free = free_replacement;
}

static void *_cargo_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    if (replaced_cargo_malloc)
        return replaced_cargo_malloc(size);

    return malloc(size);
}

static void *_cargo_realloc(void *ptr, size_t size)
{
    return replaced_cargo_realloc ? replaced_cargo_realloc(ptr, size) : realloc(ptr, size);
}

static void _cargo_free(void *ptr)
{
    if (replaced_cargo_free)
        replaced_cargo_free(ptr);
    else
        free(ptr);
}

static void *_cargo_calloc(size_t count, size_t size)
{
    void *p = NULL;

    if (!count || !size)
        return NULL;

    if (replaced_cargo_malloc)
    {
        size_t sz = count * size;
        // TODO: If count > (size_t max / size), goto fail.
        p = replaced_cargo_malloc(sz);

        if (p)
            return memset(p, 0, sz);
    }

    p = calloc(count, size);

    #ifdef _WIN32
    // Windows doesn't set ENOMEM properly.
    if (!p)
    {
        errno = ENOMEM;
        return NULL;
    }
    #endif // _WIN32

    return p;
}

static char *_cargo_strdup(const char *str)
{
    if (!str)
    {
        errno = EINVAL;
        return NULL;
    }

    if (replaced_cargo_malloc)
    {
        size_t len = strlen(str);
        void *p = NULL;

        if (len == ((size_t)-1))
            goto fail;

        if ((p = replaced_cargo_malloc(len + 1)))
        {
            return memcpy(p, str, len + 1);
        }
    }
    else
    {
        #ifdef _WIN32
        return _strdup(str);
        #else
        return strdup(str);
        #endif
    }
fail:
    errno = ENOMEM;
    return NULL;
}

int cargo_suppress_debug;

#ifdef CARGO_DEBUG
#define CARGODBG(level, fmt, ...)                                           \
do                                                                          \
{                                                                           \
    if (!cargo_suppress_debug && (level == 1))                              \
    {                                                                       \
        fprintf(stderr, "%d [cargo.c:%4d]: [ERROR] " fmt, level, __LINE__, ##__VA_ARGS__); \
    }                                                                       \
    else if (!cargo_suppress_debug && (level <= CARGO_DEBUG))               \
    {                                                                       \
        fprintf(stderr, "%d [cargo.c:%4d]: " fmt, level, __LINE__, ##__VA_ARGS__);  \
    }                                                                       \
} while (0)

#define CARGODBGI(level, fmt, ...)                                          \
do                                                                          \
{                                                                           \
    if (!cargo_suppress_debug && (level <= CARGO_DEBUG))                    \
    {                                                                       \
        fprintf(stderr, fmt, ##__VA_ARGS__);                                \
    }                                                                       \
} while (0)

#else
#define CARGODBG(level, fmt, ...)
#define CARGODBGI(level, fmt, ...)
#endif

#ifndef va_copy
  #ifdef __va_copy
    #define va_copy __va_copy
  #else
    #define va_copy(a, b) memcpy(&(a), &(b), sizeof(va_list))
  #endif
#endif

#define CARGO_MIN(a, b) ((a) < (b) ? a : b)
#define CARGO_MAX(a, b) ((a) > (b) ? a : b)

#ifndef CARGO_NOLIB

static int _cargo_get_console_width()
{
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        return -1;
    }

    CARGODBG(3, "Console width: %d\n",
        (int)(csbi.srWindow.Right - csbi.srWindow.Left));

    return (int)(csbi.srWindow.Right - csbi.srWindow.Left);

    #else // Unix

    struct winsize w;
    ioctl(0, TIOCGWINSZ , &w);

    if (!ioctl(0, TIOCGWINSZ , &w))
    {
        return w.ws_col;
    }

    return -1;
    #endif // _WIN32
}

int cargo_vsnprintf(char *buf, size_t buflen, const char *format, va_list ap)
{
    int r;
    if (!buflen)
        return 0;
    #if defined(_MSC_VER) || defined(_WIN32)
    r = _vsnprintf(buf, buflen, format, ap);
    if (r < 0)
        r = _vscprintf(format, ap);
    #elif defined(sgi)
    /* Make sure we always use the correct vsnprintf on IRIX */
    extern int      _xpg5_vsnprintf(char * __restrict,
        __SGI_LIBC_NAMESPACE_QUALIFIER size_t,
        const char * __restrict, /* va_list */ char *);

    r = _xpg5_vsnprintf(buf, buflen, format, ap);
    #else
    r = vsnprintf(buf, buflen, format, ap);
    #endif
    buf[buflen - 1] = '\0';
    return r;
}

int cargo_snprintf(char *buf, size_t buflen, const char *format, ...)
{
    int r;
    va_list ap;
    va_start(ap, format);
    r = cargo_vsnprintf(buf, buflen, format, ap);
    va_end(ap);
    return r;
}

char *cargo_strndup(const char *s, size_t n)
{
    char *res;
    size_t len = strlen(s);

    if (n < len)
    {
        len = n;
    }

    if (!(res = (char *)_cargo_malloc(len + 1)))
    {
        return NULL;
    }

    res[len] = '\0';
    return (char *)memcpy(res, s, len);
}

typedef struct cargo_str_s
{
    char *s;
    size_t l;
    size_t offset;
    size_t diff;
} cargo_str_t;

int cargo_vappendf(cargo_str_t *str, const char *format, va_list ap)
{
    int ret;
    assert(str);

    str->diff = (str->l - str->offset);

    if ((ret = cargo_vsnprintf(&str->s[str->offset],
            str->diff, format, ap)) < 0)
    {
        return -1;
    }

    str->offset += ret;

    return ret;
}

#define CARGO_ASTR_DEFAULT_SIZE 256
typedef struct cargo_astr_s
{
    char **s;
    size_t l;
    size_t offset;
    size_t diff;
} cargo_astr_t;

int cargo_avappendf(cargo_astr_t *str, const char *format, va_list ap)
{
    int ret = 0;
    va_list apc;
    assert(str);
    assert(str->s);

    if (!(*str->s))
    {
        if (str->l == 0)
        {
            str->l = CARGO_ASTR_DEFAULT_SIZE;
        }

        str->offset = 0;

        if (!(*str->s = _cargo_calloc(1, str->l)))
        {
            CARGODBG(1, "Out of memory!\n");
            return -1;
        }
    }

    while (1)
    {
        // We must copy the va_list otherwise it will be
        // out of sync on a realloc.
        va_copy(apc, ap);

        str->diff = (str->l - str->offset);

        if ((ret = cargo_vsnprintf(&(*str->s)[str->offset],
                str->diff, format, apc)) < 0)
        {
            CARGODBG(1, "Formatting error\n");
            return -1;
        }

        va_end(apc);

        if ((size_t)ret >= str->diff)
        {
            if (str->l == 0) str->l = CARGO_ASTR_DEFAULT_SIZE;
            str->l *= 2;
            CARGODBG(4, "Realloc %lu\n", str->l);

            if (!(*str->s = _cargo_realloc(*str->s, str->l)))
            {
                CARGODBG(1, "Out of memory!\n");
                return -1;
            }

            continue;
        }

        break;
    }

    str->offset += ret;

    return ret;
}

int cargo_appendf(cargo_str_t *str, const char *fmt, ...)
{
    int ret;
    va_list ap;
    assert(str);
    va_start(ap, fmt);
    ret = cargo_vappendf(str, fmt, ap);
    va_end(ap);
    return ret;
}

int cargo_aappendf(cargo_astr_t *str, const char *fmt, ...)
{
    int ret;
    va_list ap;
    assert(str);
    va_start(ap, fmt);
    ret = cargo_avappendf(str, fmt, ap);
    va_end(ap);
    return ret;
}

#ifndef _WIN32
int _vscprintf(const char *format, va_list argptr)
{
    return(vsnprintf(NULL, 0, format, argptr));
}
#endif

//
// Below is code for translating ANSI color escape codes to
// windows equivalent API calls.
//
int cargo_vasprintf(char **strp, const char *format, va_list ap)
{
    int count;
    va_list apc;
    assert(strp);

    // Find out how long the resulting string is
    va_copy(apc, ap);
    count = _vscprintf(format, apc);
    va_end(apc);

    if (count == 0)
    {
        *strp = _cargo_strdup("");
        return 0;
    }
    else if (count < 0)
    {
        // Something went wrong, so return the error code (probably still requires checking of "errno" though)
        return count;
    }

    // Allocate memory for our string
    if (!(*strp = _cargo_malloc(count + 1)))
    {
        return -1;
    }

    // Do the actual printing into our newly created string
    return vsprintf(*strp, format, ap);
}

int cargo_asprintf(char** strp, const char* format, ...)
{
    va_list ap;
    int count;

    va_start(ap, format);
    count = cargo_vasprintf(strp, format, ap);
    va_end(ap);

    return count;
}

#ifdef _WIN32
//
// Conversion tables and structs below are from
// https://github.com/adoxa/ansicon
//
#define CARGO_ANSI_MAX_ARG 16
#define CARGO_ANSI_ESC '\x1b'

#define FOREGROUND_BLACK 0
#define FOREGROUND_WHITE FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE

#define BACKGROUND_BLACK 0
#define BACKGROUND_WHITE BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE

const BYTE foregroundcolor[8] =
{
    FOREGROUND_BLACK,                   // black foreground
    FOREGROUND_RED,                     // red foreground
    FOREGROUND_GREEN,                   // green foreground
    FOREGROUND_RED | FOREGROUND_GREEN,  // yellow foreground
    FOREGROUND_BLUE,                    // blue foreground
    FOREGROUND_BLUE | FOREGROUND_RED,   // magenta foreground
    FOREGROUND_BLUE | FOREGROUND_GREEN, // cyan foreground
    FOREGROUND_WHITE                    // white foreground
};

const BYTE backgroundcolor[8] =
{
    BACKGROUND_BLACK,                   // black background
    BACKGROUND_RED,                     // red background
    BACKGROUND_GREEN,                   // green background
    BACKGROUND_RED | BACKGROUND_GREEN,  // yellow background
    BACKGROUND_BLUE,                    // blue background
    BACKGROUND_BLUE | BACKGROUND_RED,   // magenta background
    BACKGROUND_BLUE | BACKGROUND_GREEN, // cyan background
    BACKGROUND_WHITE,                   // white background
};

// Map windows console attribute to ANSI number.
const BYTE attr2ansi[8] =
{
    0,                  // black
    4,                  // blue
    2,                  // green
    6,                  // cyan
    1,                  // red
    5,                  // magenta
    3,                  // yellow
    7                   // white
};

// Used to save the state while parsing the render attributes.
typedef struct cargo_ansi_state_s
{
    WORD    def;        // The default attribute.
    BYTE    foreground; // ANSI base color (0 to 7; add 30)
    BYTE    background; // ANSI base color (0 to 7; add 40)
    BYTE    bold;       // console FOREGROUND_INTENSITY bit
    BYTE    underline;  // console BACKGROUND_INTENSITY bit
    BYTE    rvideo;     // swap foreground/bold & background/underline
    BYTE    concealed;  // set foreground/bold to background/underline
    BYTE    reverse;    // swap console foreground & background attributes
} cargo_ansi_state_t;

// ANSI render modifiers used by \x1b[#;#;...;#m
//
// 00 for normal display (or just 0)
// 01 for bold on (or just 1)
// 02 faint (or just 2)
// 03 standout (or just 3)
// 04 underline (or just 4)
// 05 blink on (or just 5)
// 07 reverse video on (or just 7)
// 08 nondisplayed (invisible) (or just 8)
// 22 normal
// 23 no-standout
// 24 no-underline
// 25 no-blink
// 27 no-reverse
// 30 black foreground
// 31 red foreground
// 32 green foreground
// 33 yellow foreground
// 34 blue foreground
// 35 magenta foreground
// 36 cyan foreground
// 37 white foreground
// 39 default foreground
// 40 black background
// 41 red background
// 42 green background
// 43 yellow background
// 44 blue background
// 45 magenta background
// 46 cyan background
// 47 white background
// 49 default background

void cargo_ansi_to_win32(cargo_ansi_state_t *state, int ansi)
{
    assert(state);

    if ((ansi >= 30) && (ansi <= 37))
    {
        // Foreground colors.
        // We remove 30 so we can use our translation table later.
        state->foreground = (ansi - 30);
    }
    else if ((ansi >= 40) && (ansi <= 47))
    {
        // Background colors.
        state->background = (ansi - 40);
    }
    else switch (ansi)
    {
        // Reset to defaults.
        case 0:  // 00 for normal display (or just 0)
        case 39: // 39 default foreground
        case 49: // 49 default background
        {
            WORD a = 7;

            if (a < 0)
            {
                state->reverse = 1;
                a = -a;
            }

            if (ansi != 49)
                state->foreground = attr2ansi[a & 7];

            if (ansi != 39)
                state->background = attr2ansi[(a >> 4) & 7];

            // Reset all to default
            // (or rather what the console was set to before we started parsing)
            if (ansi == 0)
            {
                state->bold = 0;
                state->underline = 0;
                state->rvideo = 0;
                state->concealed = 0;
            }
            break;
        }
        case 1: state->bold = FOREGROUND_INTENSITY; break;
        case 5: // blink.
        case 4: state->underline = BACKGROUND_INTENSITY; break;
        case 7: state->rvideo = 1; break;
        case 8: state->concealed = 1; break;
        case 21: // Double underline says ansicon
        case 22: state->bold = 0; break;
        case 25: // no blink.
        case 24: state->underline = 0; break;
        case 27: state->rvideo = 0; break;
        case 28: state->concealed = 0; break;
    }
}

WORD cargo_ansi_state_to_attr(cargo_ansi_state_t *state)
{
    WORD attr = 0;
    assert(state);

    if (state->concealed)
    {
        // Reversed video.
        if (state->rvideo)
        {
            attr = foregroundcolor[state->foreground]
                 | backgroundcolor[state->foreground];

            if (state->bold)
            {
                attr |= FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
            }
        }
        else // Normal.
        {
            attr = foregroundcolor[state->background]
                 | backgroundcolor[state->background];

            if (state->underline)
            {
                attr |= FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
            }
        }
    }
    else if (state->rvideo)
    {
        attr = foregroundcolor[state->background]
             | backgroundcolor[state->foreground];

        if (state->bold) attr |= BACKGROUND_INTENSITY;
        if (state->underline) attr |= FOREGROUND_INTENSITY;
    }
    else
    {
        attr = foregroundcolor[state->foreground] | state->bold
             | backgroundcolor[state->background] | state->underline;
    }

    if (state->reverse)
    {
        attr = ((attr >> 4) & 15) | ((attr & 15) << 4);
    }

    return attr;
}

void cargo_print_ansicolor(FILE *fd, const char *buf)
{
    const char *end = NULL;
    char *numend = NULL;
    const char *s = buf;
    int i = 0;
    cargo_ansi_state_t state;
    HANDLE handle = INVALID_HANDLE_VALUE;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    memset(&state, 0, sizeof(cargo_ansi_state_t));

    handle = (HANDLE)_get_osfhandle(fileno(fd));
    GetConsoleScreenBufferInfo(handle, &csbi);

    // We reset to these attributes if we get a reset code.
    state.def = csbi.wAttributes;

    while (*s)
    {
        if (*s == CARGO_ANSI_ESC)
        {
            s++;

            // ANSI colors are in the following format:
            // \x1b[#;#;...;#m
            //
            // Where # is a set of colors and modifiers, see above for table.
            if (*s == '[')
            {
                // TODO: Add support for other Escape codes than render ones.
                s++;
                end = strchr(s, 'm');

                if (!end || ((end - s) > CARGO_ANSI_MAX_ARG))
                {
                    // Output other escaped sequences
                    // like normal, we just support colors!
                    continue;
                }

                // Parses #;#;#;# until there is no more ';'
                // at the end.
                do
                {
                    i = strtol(s, &numend, 0);
                    cargo_ansi_to_win32(&state, i);
                    s = numend + 1;
                } while (*numend == ';');

                SetConsoleTextAttribute(handle,
                    cargo_ansi_state_to_attr(&state));
            }
        }
        else
        {
            putchar(*s);
            s++;
        }
    }
}

void cargo_fprintf(FILE *fd, const char *fmt, ...)
{
    int ret;
    va_list ap;
    char *s = NULL;

    va_start(ap, fmt);
    ret = cargo_vasprintf(&s, fmt, ap);
    va_end(ap);

    if (ret != -1)
    {
        cargo_print_ansicolor(fd, s);
    }

    if (s) _cargo_free(s);
}

// Overload the normal printf!
#define fprintf cargo_fprintf
#define printf cargo_printf

#else // Unix below (already has ANSI support).

void cargo_print_ansicolor(FILE *fd, const char *buf)
{
    fprintf(fd, "%s", buf);
}

#define cargo_fprintf fprintf

#endif // End Unix

#define cargo_printf(fmt, ...) cargo_fprintf(stdout, fmt, ##__VA_ARGS__)

#define CARGO_NARGS_ONE_OR_MORE     -1
#define CARGO_NARGS_ZERO_OR_MORE    -2
#define CARGO_NARGS_ZERO_OR_ONE     -3

const char *_cargo_type_to_str(cargo_type_t type)
{
    switch (type)
    {
        case CARGO_BOOL: return "bool";
        case CARGO_INT: return "int";
        case CARGO_UINT: return "unsigned int";
        case CARGO_FLOAT: return "float";
        case CARGO_DOUBLE: return "double";
        case CARGO_STRING: return "string";
        case CARGO_LONGLONG: return "long long";
        case CARGO_ULONGLONG: return "unsigned long long";
    }

    return NULL;
}

typedef enum cargo_bool_acc_op_e
{
    CARGO_BOOL_OP_PLUS,
    CARGO_BOOL_OP_OR,
    CARGO_BOOL_OP_AND,
    CARGO_BOOL_OP_STORE
} cargo_bool_acc_op_t;

struct cargo_validation_s
{
    const char *name;
    cargo_validation_f validator;
    cargo_validation_destroy_f destroy;
    cargo_type_t types;
    size_t ref_count;
    void *user;
};

typedef struct cargo_group_s cargo_group_t;

typedef struct cargo_opt_s
{
    char *name[CARGO_NAME_COUNT];
    size_t name_count;
    char *description;
    char *metavar;
    int positional;
    cargo_type_t type;
    int nargs;
    int alloc;
    int str_alloc_items;        // If we should allocate string items
                                // (but not the array).

    int group_index;
    size_t mutex_group_idxs[CARGO_MAX_OPT_MUTEX_GROUP];
    size_t mutex_group_count;
    char **mutex_group_names;

    cargo_custom_f custom;      // Custom callback function.
    void *custom_user;          // Custom user data passed to user callback.
    size_t *custom_user_count;  // Used to return array count when parsing
                                // custom callbacks.

    char **custom_target;       // Internal storage for args passed to callback.
    size_t custom_target_count; // Internal count for args passed to callbac.

    void **target;              // Pointer to target values.
    size_t target_idx;          // Current index into target values.
    size_t *target_count;       // Return value or number of parsed target values.
    size_t lenstr;              // String length.
    size_t max_target_count;    // Max values to store in an array.

    int array;                  // Is this option being parsed as an array?
    int parsed;                 // The argv index when we last parsed the option
    cargo_option_flags_t flags;
    int num_eaten;              // How many arguments consumed by this option.
    int first_parse;            // First time we parse this? (cargo_parse can be called more than once)

    int bool_store;             // Value to store when a bool flag is set.
    int bool_count;             // If we should count occurances for bool flag.

    // Bool accumulator related.
    int *bool_acc;              // Values to accumulate.
    cargo_bool_acc_op_t bool_acc_op;    // Operation used to accumulate.
    size_t bool_acc_count;              // Current index into the accumulate vals.
    size_t bool_acc_max_count;          // Number of accumulation values.

    char *zero_or_one_default;  // Default value used for target value when

    cargo_validation_t *validation; // Validation for target values.
    cargo_validation_flags_t validation_flags;
} cargo_opt_t;

#define CARGO_DEFAULT_MAX_GROUPS 4
#define CARGO_DEFAULT_MAX_GROUP_OPTS 8

struct cargo_group_s
{
    char *name;
    char *title;
    char *description;
    char *metavar;
    size_t flags;           // This is either cargo_group_flags_t
                            // or cargo_mutex_group_flags_t depending on type.

    size_t *option_indices;
    size_t opt_count;
    size_t max_opt_count;
    void *user;
};

typedef struct cargo_s
{
    char *progname;
    char *description;
    char *epilog;
    size_t max_width;
    cargo_usage_t usage_flags;
    cargo_flags_t flags;

    int i;  // argv index.
    int j;  // sub-argv index (when getting arguments for options)
    int argc;
    char **argv;
    int start;
    int stopped;
    int stopped_hard;

    int help;

    cargo_group_t *groups;
    size_t group_count;
    size_t max_groups;

    cargo_group_t *mutex_groups;
    size_t mutex_group_count;
    size_t mutex_max_groups;

    cargo_opt_t *options;
    size_t opt_count;
    size_t max_opts;
    const char *prefix;

    char **unknown_opts;
    int *unknown_opts_idxs;
    size_t unknown_opts_count;

    char **args;
    size_t arg_count;

    char *error;
    char *short_usage;
    char *usage;

    void *user;
} cargo_s;

static void _cargo_xfree(void *p)
{
    void **pp;
    assert(p);

    pp = (void **)p;

    if (*pp)
    {
        _cargo_free(*pp);
        *pp = NULL;
    }
}

static void _cargo_set_error(cargo_t ctx, char *error)
{
    assert(ctx);

    _cargo_xfree(&ctx->error);
    ctx->error = error;
}

static cargo_fprint_flags_t _cargo_get_cflag(cargo_t ctx)
{
    assert(ctx);
    return (ctx->flags & CARGO_NOCOLOR) ? CARGO_FPRINT_NOCOLOR : 0;
}

static size_t _cargo_get_type_size(cargo_type_t t)
{
    assert((t >= CARGO_BOOL) && (t <= CARGO_ULONGLONG));

    switch (t)
    {
        case CARGO_BOOL:
        case CARGO_INT: return sizeof(int);
        case CARGO_UINT: return sizeof(unsigned int);
        case CARGO_FLOAT: return sizeof(float);
        case CARGO_DOUBLE: return sizeof(double);
        case CARGO_STRING: return sizeof(char *);
        case CARGO_LONGLONG: return sizeof(long long int);
        case CARGO_ULONGLONG: return sizeof(unsigned long long int);
    }

    return 0;
}

static int _cargo_nargs_is_valid(int nargs)
{
    return (nargs >= 0)
        || (nargs == CARGO_NARGS_ZERO_OR_MORE)
        || (nargs == CARGO_NARGS_ONE_OR_MORE)
        || (nargs == CARGO_NARGS_ZERO_OR_ONE);
}

static int _cargo_starts_with_prefix(cargo_t ctx, const char *arg)
{
    return (strpbrk(arg, ctx->prefix) == arg);
}

static char _cargo_is_prefix(cargo_t ctx, char c)
{
    size_t i;
    size_t prefix_len = strlen(ctx->prefix);

    for (i = 0; i < prefix_len; i++)
    {
        if (c == ctx->prefix[i])
        {
            return c;
        }
    }

    return 0;
}

static int _cargo_find_option_name(cargo_t ctx, const char *name,
                                    size_t *opt_i, size_t *name_i)
{
    size_t i = 0;
    size_t j = 0;
    cargo_opt_t *opt = NULL;
    assert(name);

    for (i = 0; i < ctx->opt_count; i++)
    {
        opt = &ctx->options[i];

        for (j = 0; j < opt->name_count; j++)
        {
            if (!strcmp(opt->name[j], name))
            {
                if (opt_i) *opt_i = i;
                if (name_i) *name_i = j;
                return 0;
            }
        }
    }

    return -1;
}

static int _cargo_validate_option_args(cargo_t ctx, cargo_opt_t *o)
{
    assert(ctx);
    assert(o);

    if (!_cargo_nargs_is_valid(o->nargs))
    {
        CARGODBG(1, "%s: nargs is invalid %d\n", o->name[0], o->nargs);
        return -1;
    }

    if (!o->custom)
    {
        if (!o->target)
        {
            CARGODBG(1, "%s: target NULL\n", o->name[0]);
            return -1;
        }

        if (!o->target_count
            && ((o->nargs > 1) || (o->nargs == CARGO_NARGS_ONE_OR_MORE)))
        {
            CARGODBG(1, "%s: target_count NULL, when nargs > 1\n", o->name[0]);
            return -1;
        }
    }

    return 0;
}

static int _cargo_grow_options(cargo_opt_t **options,
                                size_t *opt_count, size_t *max_opts)
{
    assert(options);
    assert(opt_count);
    assert(max_opts);
    assert(*max_opts > 0);

    if (!*options)
    {
        if (!((*options) = _cargo_calloc(*max_opts, sizeof(cargo_opt_t))))
        {
            CARGODBG(1, "Out of memory\n");
            return -1;
        }
    }

    if (*opt_count >= *max_opts)
    {
        cargo_opt_t *new_options = NULL;
        CARGODBG(2, "Option count (%lu) >= Max option count (%lu)\n",
            *opt_count, *max_opts);

        (*max_opts) *= 2;

        if (!(new_options = _cargo_realloc(*options,
                                    (*max_opts) * sizeof(cargo_opt_t))))
        {
            CARGODBG(1, "Out of memory!\n");
            return -1;
        }

        *options = new_options;
    }

    return 0;
}

static int _cargo_get_positional(cargo_t ctx, size_t *opt_i)
{
    size_t i;
    cargo_opt_t *opt = NULL;
    assert(ctx);

    *opt_i = 0;

    for (i = 0; i < ctx->opt_count; i++)
    {
        opt = &ctx->options[i];

        if (opt->positional && (opt->num_eaten != opt->nargs))
        {
            *opt_i = i;
            return 0;
        }
    }

    return -1;
}

static const char *_cargo_is_option_name(cargo_t ctx,
                    cargo_opt_t *opt, const char *arg)
{
    size_t i;
    const char *name;

    if (!_cargo_starts_with_prefix(ctx, arg))
        return NULL;

    for (i = 0; i < opt->name_count; i++)
    {
        name = opt->name[i];

        if (!strcmp(name, arg))
        {
            CARGODBG(3, "  Found matching option \"%s\", alias \"%s\"\n",
                    opt->name[0], opt->name[i]);
            return name;
        }
    }

    return NULL;
}

static const char *_cargo_is_option_name_compact(cargo_t ctx,
                    cargo_opt_t *opt, const char *arg)
{
    size_t i;
    size_t j;
    int found = 0;
    const char *name;

    // This looks for the format -vvv when we have
    // an option "--verbosity -v" of type CARGO_BOOL
    // that has 'b!' format specifier set.
    // Which means to count each flag occurance.
    // Then "-v -v -v" and "-vvv" is equivalent and should
    // parse the bool as 3.

    if (!_cargo_starts_with_prefix(ctx, arg))
        return NULL;

    arg += strspn(arg, ctx->prefix);

    for (i = 0; i < opt->name_count; i++)
    {
        name = opt->name[i];

        if (!_cargo_starts_with_prefix(ctx, name))
            continue;

        // "-vvv" -> "vvv"
        name += strspn(name, ctx->prefix);
        found = 0;

        // Compare only the beginning of the arg we're given.
        // So if the opt has an alias "-v", we remove the "-"
        // and compare "v" with strlen("v") characters in the
        // input string: strncmp("v", "vvv", 1) == 0
        for (j = 0; j < strlen(arg); j++)
        {
            found = (*name == arg[j]);
        }

        if (found)
        {
            if (opt->bool_count || opt->bool_acc)
            {
                CARGODBG(3, "  Found matching option \"%s\", alias \"%s\"\n",
                        opt->name[0], opt->name[i]);
                return name;
            }
        }
    }

    return NULL;
}

static void _cargo_free_str_list(char ***s, size_t *count)
{
    size_t i;

    if (!s || !*s)
        goto done;

    // Only free elements if we have a count.
    if (count)
    {
        for (i = 0; i < *count; i++)
        {
            _cargo_free((*s)[i]);
            (*s)[i] = NULL;
        }
    }

    _cargo_free(*s);
    *s = NULL;
done:
    if (count)
        *count = 0;
}

static void _cargo_cleanup_option_value(cargo_t ctx,
                                        cargo_opt_t *opt,
                                        int free_target)
{
    assert(opt);

    opt->target_idx = 0;
    opt->parsed = -1;    // index into argv that we parsed this option at.
    opt->num_eaten = 0;

    CARGODBG(3, "Cleanup option (%s) value: %s\n",
            _cargo_type_to_str(opt->type), opt->name[0]);

    // We don't want to always free the target, just reset it for parsing again.
    // For instance when running cargo_parse multiple times in a row
    // we only want the values we have already parsed to be freed if
    // the new parse overwrites it.
    if (!free_target)
        return;

    // To enable the user to have string literals as
    // default values, on the first time cargo_parse is called
    // we do not try to free the value, unless it has explicitly
    // been overwritten by the user.
    if (opt->first_parse)
    {
        if ((ctx->flags & CARGO_DEFAULT_LITERALS)
         || (opt->flags & CARGO_OPT_DEFAULT_LITERAL))
        {
            // Don't free a default value set like this:
            // char *s = "abc";
            // cargo_add_option(...., "s", &s);
            // cargo_parse(...) <-- Don't try to free 's' here before assigning
            //                      a new value!
            // cargo_parse(...) <-- However, if we overwrote it in previous
            //                      parse, we do want to free it here.
            CARGODBG(3, "    Not freeing target on first parse!\n");
            goto skip_free;
        }
        else
        {
            // This is for the case when the user did this:
            // char *s = strdup("abc");
            // cargo_add_option(...., "s", &s);
            CARGODBG(3, "    Default values will be freed\n");
        }
    }

    CARGODBG(3, "  Freeing target:\n");

    if (opt->custom)
    {
        _cargo_free_str_list(&opt->custom_target, &opt->custom_target_count);
        return;
    }
    else if (opt->alloc)
    {
        CARGODBG(4, "    Allocated value\n");

        if (opt->target && *opt->target)
        {
            if (opt->array)
            {
                CARGODBG(4, "    Array\n");

                if (opt->type == CARGO_STRING)
                {
                    _cargo_free_str_list(((char ***)opt->target),
                        opt->target_count);
                }
                else
                {
                    _cargo_free(*opt->target);
                    *opt->target = NULL;
                }
            }
            else
            {
                CARGODBG(4, "    Not array\n");

                if (opt->type == CARGO_STRING)
                {
                    CARGODBG(4, "    String\n");
                    _cargo_free(*opt->target);
                    *opt->target = NULL;
                }
            }
        }
        else
        {
            CARGODBG(4, "    Target value NULL\n");
        }
    }
    else
    {
        if (opt->target && opt->target_count)
        {
            memset(opt->target, 0,
                _cargo_get_type_size(opt->type) * (*opt->target_count));
        }
    }

skip_free:
    if (opt->target_count)
        *opt->target_count = 0;

    if (opt->alloc && opt->target)
        *opt->target = NULL;
}

static void _cargo_cleanup_option_values(cargo_t ctx, int free_targets)
{
    size_t i;
    assert(ctx);

    for (i = 0; i < ctx->opt_count; i++)
    {
        _cargo_cleanup_option_value(ctx, &ctx->options[i], free_targets);
    }
}

static void _cargo_free_validation(cargo_validation_t **vd)
{
    cargo_validation_t *v;

    if (!vd || !(*vd))
        return;

    v = *vd;

    v->ref_count--;

    if (v->ref_count <= 0)
    {
        if (v->destroy)
        {
            CARGODBG(3, "Calling validation destroy function\n");
            v->destroy(v->user);
        }

        _cargo_xfree(&v->user);
        _cargo_xfree(vd);
    }
}

void _cargo_option_destroy_validation(cargo_opt_t *o)
{
    assert(o);

    if (o->validation)
    {
        CARGODBG(3, "Destroying validation \"%s\" for \"%s\"\n",
                o->validation->name, o->name[0]);

        _cargo_free_validation(&o->validation);
    }
}

int _cargo_validate_option_value(cargo_t ctx, cargo_opt_t *o, void *value)
{
    assert(ctx);
    assert(o);
    assert(o->validation);
    assert(o->validation->validator);

    if (o->validation->validator(ctx, o->validation_flags, o->name[0],
                                o->validation, value))
    {
        return -1;
    }

    return 0;
}

int _cargo_validate_option_values(cargo_t ctx, cargo_opt_t *o)
{
    size_t i;
    assert(ctx);
    assert(o);

    if (!o->validation)
        return 0;

    for (i = 0; i < o->target_idx; i++)
    {
        if (_cargo_validate_option_value(ctx, o, o->target[i]))
        {
            return i;
        }
    }

    return 0;
}

int _cargo_validate_options(cargo_t ctx)
{
    size_t i;
    assert(ctx);

    for (i = 0; i < ctx->opt_count; i++)
    {
        if (_cargo_validate_option_values(ctx, &ctx->options[i]))
        {
            return -1;
        }
    }

    return 0;
}

static char *_cargo_highlight_current_target_value(cargo_t ctx)
{
    return cargo_get_fprint_args(ctx->argc, ctx->argv, ctx->start,
                        _cargo_get_cflag(ctx), ctx->max_width,
                        2,
                        ctx->i - 1, "^"CARGO_COLOR_YELLOW,
                        ctx->j, "~"CARGO_COLOR_RED);
}

static int _cargo_set_target_value(cargo_t ctx, cargo_opt_t *opt,
                                    const char *name, char *val)
{
    void *target;
    char *end = NULL;
    assert(ctx);
    assert((opt->type >= CARGO_BOOL) && (opt->type <= CARGO_ULONGLONG));

    if ((opt->type != CARGO_BOOL)
        && (opt->target_idx >= opt->max_target_count))
    {
        CARGODBG(1, "Target index out of bounds (%lu > max %lu)\n",
                opt->target_idx, opt->max_target_count);
        return 1;
    }

    CARGODBG(2, "_cargo_set_target_value:\n");
    CARGODBG(2, "  alloc: %d\n", opt->alloc);
    CARGODBG(2, "  nargs: %d\n", opt->nargs);

    // If number of arguments is just 1 don't allocate an array
    // (Except for custom callback, then we always use an array
    //  since we store the arguments internally).
    if (opt->custom || (opt->alloc && (opt->nargs != 1)))
    {
        // Allocate the memory needed.
        if (!*(opt->target))
        {
            // TODO: Break out into function.
            void **new_target;
            int alloc_count = opt->nargs;

            if (opt->nargs < 0)
            {
                // In this case we don't want to preallocate everything
                // since we might have "unlimited" arguments.
                // CARGO_NARGS_ONE_OR_MORE
                // CARGO_NARGS_ZERO_OR_MORE
                // TODO: Don't allocate all of these right away.
                alloc_count = ctx->argc - ctx->i;
                assert(alloc_count >= 0);

                // Don't allocate more than necessary.
                if (opt->max_target_count < (size_t)alloc_count)
                    alloc_count = opt->max_target_count;
            }

            if (!(new_target = (void **)_cargo_calloc(alloc_count,
                        _cargo_get_type_size(opt->type))))
            {
                CARGODBG(1, "Out of memory!\n");
                return -1;
            }

            CARGODBG(3, "Allocated %dx %s!\n",
                    alloc_count, _cargo_type_to_str(opt->type));

            *(opt->target) = new_target;
        }

        target = *(opt->target);
    }
    else if (opt->str_alloc_items)
    {
        CARGODBG(3, "Static list size %lu of allocated strings\n",
                    opt->max_target_count);

        target = (void *)opt->target;
    }
    else
    {
        // Just a normal pointer.
        CARGODBG(3, "Not allocating array or single %s\n",
                _cargo_type_to_str(opt->type));

        target = (void *)opt->target;
    }

    switch (opt->type)
    {
        case CARGO_BOOL:
        {
            int *val = &((int *)target)[opt->target_idx];
            CARGODBG(2, "      bool\n");

            // TODO: This could be more general to support
            //       short command lines bundled together:
            //       "-vaet" same as "-v -a -e -t"...

            // If BOOL COUNT is turned on, we allow multiple occurances of
            // a bool option. "-v -v -v" will be parsed as 3.
            if (opt->bool_count)
            {
                char *arg = ctx->argv[ctx->i];
                CARGODBG(2, "        bool count enabled\n");

                // We can specify it as "-vvv" as well.
                if (!_cargo_is_option_name(ctx, opt, arg)
                  && _cargo_is_option_name_compact(ctx, opt, arg))
                {
                    int amount;
                    CARGODBG(2, "          Compact %s\n", arg);
                    arg += strspn(arg, ctx->prefix);
                    amount = (int)strlen(arg);
                    CARGODBG(2, "              %d\n", amount);
                    (*val) += amount;
                }
                else
                {
                    CARGODBG(2, "          Normal\n");
                    (*val)++;
                }
            }
            else if (opt->bool_acc)
            {
                // TODO: Maybe move all this complexity from
                // handling -vvv as a special case here, to instead expanding it
                // at the beginning of the parse... "-vvv" -> "-v", "-v", "-v"
                size_t count;
                int acc_val;
                size_t i = 0;
                char *arg = ctx->argv[ctx->i];
                CARGODBG(2, "           ARG: %s\n", arg);

                // "-vvv" support.
                if (!_cargo_is_option_name(ctx, opt, arg)
                  && _cargo_is_option_name_compact(ctx, opt, arg))
                {
                    CARGODBG(2, "          Compact %s\n", arg);
                    arg += strspn(arg, ctx->prefix);
                    count = strlen(arg);
                    CARGODBG(2, "              %lu\n", count);
                }
                else
                {
                    count = 1;
                }

                for (i = opt->bool_acc_count;
                    (i < opt->bool_acc_count + count)
                    && (i < opt->bool_acc_max_count);
                    i++)
                {
                    acc_val = opt->bool_acc[i];

                    CARGODBG(2, "       %lu Bool acc %x\n", i, acc_val);
                    switch (opt->bool_acc_op)
                    {
                        case CARGO_BOOL_OP_OR:
                        {
                            CARGODBG(2, "       OR\n");
                            *val |= acc_val;
                            break;
                        }
                        case CARGO_BOOL_OP_AND:
                        {
                            CARGODBG(2, "       AND\n");
                            *val &= acc_val;
                            break;
                        }
                        case CARGO_BOOL_OP_PLUS:
                        {
                            CARGODBG(2, "       PLUS\n");
                            *val += acc_val;
                            break;
                        }
                        case CARGO_BOOL_OP_STORE:
                        {
                            CARGODBG(2, "       STORE\n");
                            *val = acc_val;
                            break;
                        }
                        default:
                        {
                            CARGODBG(1, "Got unknown operation\n");
                            break;
                        }
                    }
                }

                opt->bool_acc_count = i;

                if (opt->bool_acc_count >= opt->bool_acc_max_count)
                {
                    CARGODBG(2, "       Bool acc reached maxcount %lu\n",
                            opt->bool_acc_max_count);
                    break;
                }
            }
            else
            {
                CARGODBG(2, "       No bool count\n");
                *val = opt->bool_store;
            }
            break;
        }
        case CARGO_INT:
        {
            CARGODBG(2, "      int %s\n", val);
            ((int *)target)[opt->target_idx] = strtol(val, &end, 10);
            break;
        }
        case CARGO_UINT:
        {
            CARGODBG(2, "      uint %s\n", val);
            ((unsigned int *)target)[opt->target_idx] = strtoul(val, &end, 10);
            break;
        }
        case CARGO_LONGLONG:
        {
            CARGODBG(2, "      long long %s\n", val);
            ((long long int *)target)[opt->target_idx] = strtoll(val, &end, 10);
            break;
        }
        case CARGO_ULONGLONG:
        {
            CARGODBG(2, "      unsigned long long %s\n", val);
            ((unsigned long long int *)target)[opt->target_idx] = strtoull(val, &end, 10);
            break;
        }
        case CARGO_FLOAT:
        {
            CARGODBG(2, "      float %s\n", val);
            ((float *)target)[opt->target_idx] = (float)strtof(val, &end);
            break;
        }
        case CARGO_DOUBLE:
        {
            CARGODBG(2, "      double %s\n", val);
            ((double *)target)[opt->target_idx] = (double)strtod(val, &end);
            break;
        }
        case CARGO_STRING:
            CARGODBG(2, "      string \"%s\"\n", val);
            if (opt->alloc)
            {
                CARGODBG(2, "       ALLOCATED STRING\n");
                if (opt->lenstr == 0)
                {
                    //char **t = (char **)((char *)target + opt->target_idx * sizeof(char *));
                    CARGODBG(2, "          COPY FULL STRING\n");

                    if (!(((char **)target)[opt->target_idx]
                            = _cargo_strdup(val)))
                    {
                        return -1;
                    }
                }
                else
                {
                    CARGODBG(2, "          MAX LENGTH: %lu\n", opt->lenstr);
                    if (!(((char **)target)[opt->target_idx]
                            = cargo_strndup(val, opt->lenstr)))
                    {
                        return -1;
                    }
                }
            }
            else if (opt->str_alloc_items)
            {
                // Special case for static lists of allocated strings:
                //  char *strs[5];
                CARGODBG(2, "          COPY FULL STRING INTO STATIC LIST %lu\n", opt->target_idx);
                CARGODBG(2, "          loc: %p\n", &((char **)target)[opt->target_idx]);
                if (!(((char **)target)[opt->target_idx] = _cargo_strdup(val)))
                {
                    return -1;
                }
            }
            else // static string.
            {
                char *t = (char *)((char *)target + opt->target_idx * opt->lenstr);
                CARGODBG(2, "       STATIC STRING, bufsize = %lu\n", opt->lenstr);
                strncpy(t, val, opt->lenstr);
                CARGODBG(2, "       \"%s\"\n", t);
            }
            break;
    }

    // Error checks.
    {
        cargo_astr_t str;
        char *error = NULL;
        char *highlight = NULL;
        memset(&str, 0, sizeof(cargo_astr_t));
        str.s = &error;

        // This indicates error for the strtox functions.
        // (Don't include bool here, since val will be NULL in that case).
        if ((opt->type != CARGO_BOOL) && (end == val))
        {
            CARGODBG(1, "Cannot parse \"%s\" as %s\n",
                    val, _cargo_type_to_str(opt->type));

            highlight = _cargo_highlight_current_target_value(ctx);

            cargo_aappendf(&str, "%s\nCannot parse \"%s\" as %s for option \"%s\"\n",
                    highlight, val, _cargo_type_to_str(opt->type), opt->name[0]);

            _cargo_xfree(&highlight);
            _cargo_set_error(ctx, error);
            return -1;
        }

        // Use validation function to verify target value.
        if (opt->validation)
        {
            // Cast the current target index properly.
            void *trg = NULL;

            switch (opt->type)
            {
                case CARGO_BOOL:
                case CARGO_INT:
                    trg = (void *)&((int *)target)[opt->target_idx];
                    break;
                case CARGO_UINT:
                    trg = (void *)&((unsigned int *)target)[opt->target_idx];
                    break;
                case CARGO_FLOAT:
                    trg = (void *)&((float *)target)[opt->target_idx];
                    break;
                case CARGO_DOUBLE:
                    trg = (void *)&((double *)target)[opt->target_idx];
                    break;
                case CARGO_LONGLONG:
                    trg = (void *)&((long long int *)target)[opt->target_idx];
                    break;
                case CARGO_ULONGLONG:
                    trg = (void *)&((unsigned long long int *)target)[opt->target_idx];
                    break;
                case CARGO_STRING:
                    trg = (void *)((char **)target)[opt->target_idx];
                    break;
            }

            if (_cargo_validate_option_value(ctx, opt, trg))
            {
                CARGODBG(1, "Failed to validate \"%s\" for \"%s\"\n", val, opt->name[0]);
                highlight = _cargo_highlight_current_target_value(ctx);

                // The validation can set an error. So use that.
                if (ctx->error)
                {
                    cargo_aappendf(&str, "%s\n%s\n", highlight, ctx->error);
                }
                else
                {
                    cargo_aappendf(&str, "%s\nFailed to validate value for \"%s\"\n",
                                    highlight, opt->name[0]);
                }

                _cargo_xfree(&highlight);
                _cargo_set_error(ctx, error);
                return -1;
            }
        }
    }

    opt->target_idx++;
    CARGODBG(3, "UPDATED TARGET INDEX: %lu\n", opt->target_idx);

    if (opt->target_count)
    {
        *opt->target_count = opt->target_idx;
    }

    return 0;
}

static const char *_cargo_check_options(cargo_t ctx, cargo_opt_t **opt, char *arg)
{
    size_t j;
    const char *name = NULL;
    assert(opt);

    if (!_cargo_starts_with_prefix(ctx, arg))
        return NULL;

    // Look for completely matching options first.
    for (j = 0; j < ctx->opt_count; j++)
    {
        name = NULL;
        *opt = &ctx->options[j];

        if ((name = _cargo_is_option_name(ctx, *opt, arg)))
        {
            return name;
        }
    }

    // Now look for the special case "-vvv" for bools.
    for (j = 0; j < ctx->opt_count; j++)
    {
        name = NULL;
        *opt = &ctx->options[j];

        if ((name = _cargo_is_option_name_compact(ctx, *opt, arg)))
        {
            return name;
        }
    }

    *opt = NULL;

    return NULL;
}

static int _cargo_is_another_option(cargo_t ctx, char *arg)
{
    cargo_opt_t *opt = NULL;
    const char *name = _cargo_check_options(ctx, &opt, arg);
    return (name != NULL);
}

static int _cargo_check_if_already_parsed(cargo_t ctx,
                                          cargo_opt_t *opt, const char *name)
{
    if (opt->parsed >= 0)
    {
        cargo_astr_t str;
        char *error = NULL;
        char *s = NULL;
        memset(&str, 0, sizeof(cargo_astr_t));
        str.s = &error;

        if ((opt->type == CARGO_BOOL)
         && (opt->bool_count || opt->bool_acc))
        {
            // This is for parsing multiple arguments of the same type
            // for instance -v -v -v.
            // We need to reset the target index when doing this, otherwise
            // we would be treating the target value for the bool option as an
            // array since target_idx is incremented on each parse.
            opt->target_idx = 0;
        }
        else if ((ctx->flags & CARGO_UNIQUE_OPTS)
              || (opt->flags & CARGO_OPT_UNIQUE))
        {
            CARGODBG(2, "%s: Parsing option as unique\n", name);
            s = cargo_get_fprint_args(ctx->argc, ctx->argv, ctx->start,
                            _cargo_get_cflag(ctx), ctx->max_width,
                            2, // Number of highlights.
                            opt->parsed, "^"CARGO_COLOR_GREEN,
                            ctx->i, "~"CARGO_COLOR_RED);
            cargo_aappendf(&str,
                "%s\n Error: %s was already specified before.\n", s, name);
            _cargo_xfree(&s);
            _cargo_set_error(ctx, error);
            return -1;
        }
        else
        {
            CARGODBG(2,
                "%s: Parsing option that has already been parsed\n", name);
            s = cargo_get_fprint_args(ctx->argc, ctx->argv, ctx->start,
                            _cargo_get_cflag(ctx), ctx->max_width,
                            2,
                            opt->parsed, "^"CARGO_COLOR_DARK_GRAY,
                            ctx->i, "~"CARGO_COLOR_YELLOW);

            cargo_aappendf(&str, " Warning: %s was already specified before, "
                            "the latter value will be used.\n", name);

            // TODO: Should we always do this?
            // Say --abc takes a list of integers.
            // --abc 1 2 3 ... or why not --abc 1 --def 5 --abc 2 3
            // (probably a bad idea :D)
            _cargo_cleanup_option_value(ctx, opt, 1);
            _cargo_xfree(&s);
            _cargo_set_error(ctx, error);
        }
    }
    else
    {
        _cargo_cleanup_option_value(ctx, opt, 1);
    }

    return 0;
}

static const char *_cargo_nargs_str(int nargs)
{
    static char s[32];

    *s = '\0';

    switch (nargs)
    {
        case CARGO_NARGS_ZERO_OR_ONE: return "0 or 1";
        case CARGO_NARGS_ONE_OR_MORE: return "1 or more";
        case CARGO_NARGS_ZERO_OR_MORE: return "0 or more";
        default: cargo_snprintf(s, sizeof(s), "%d", nargs); return s;
    }
}

static cargo_parse_result_t _cargo_parse_option(cargo_t ctx,
                                cargo_opt_t *opt,
                                const char *name,
                                int argc, char **argv)
{
    int ret;
    int args_to_look_for;
    int start = opt->positional ? ctx->i : (ctx->i + 1);

    CARGODBG(2, "------ Parse option %s ------\n", opt->name[0]);
    CARGODBG(2, "argc: %d\n", argc);
    CARGODBG(2, "i: %d\n", ctx->i);
    CARGODBG(2, "start: %d\n", start);
    CARGODBG(2, "parsed: %d\n", opt->parsed);
    CARGODBG(2, "positional: %d\n", opt->positional);

    if (!opt->positional
        && _cargo_check_if_already_parsed(ctx, opt, name))
    {
        return CARGO_PARSE_OPT_ALREADY_PARSED;
    }

    // Keep looking until the end of the argument list.
    if ((opt->nargs == CARGO_NARGS_ONE_OR_MORE) ||
        (opt->nargs == CARGO_NARGS_ZERO_OR_MORE) ||
        (opt->nargs == CARGO_NARGS_ZERO_OR_ONE))
    {
        args_to_look_for = (argc - start);

        switch (opt->nargs)
        {
            case CARGO_NARGS_ONE_OR_MORE:
                args_to_look_for = CARGO_MAX(args_to_look_for, 1);
                break;
            case CARGO_NARGS_ZERO_OR_ONE:
                args_to_look_for = CARGO_MIN(args_to_look_for, 1);
                break;
        }
    }
    else
    {
        // Read (number of expected arguments) - (read so far).
        args_to_look_for = (opt->nargs - opt->target_idx);
    }

    assert(args_to_look_for >= 0);

    if (start + args_to_look_for >= ctx->argc)
    {
        args_to_look_for = ctx->argc - start;
    }

    CARGODBG(3, "Looking for %d args\n", args_to_look_for);
    CARGODBG(3, "Start %d, End %d (argc %d, nargs %d)\n",
            start, (start + args_to_look_for), argc, opt->nargs);

    ctx->j = start;

    if (opt->type == CARGO_BOOL)
    {
        if ((ret = _cargo_set_target_value(ctx, opt, name, argv[ctx->j])) < 0)
        {
            CARGODBG(1, "Failed to set value for no argument option\n");
            return CARGO_PARSE_FAIL_OPT;
        }
    }
    else if (opt->nargs == CARGO_NARGS_ZERO_OR_ONE)
    {
        char *arg;

        if ((args_to_look_for == 0)
            || _cargo_is_another_option(ctx, argv[ctx->j]))
        {
            arg = opt->zero_or_one_default;
        }
        else
        {
            arg = argv[ctx->j];
        }

        if ((ret = _cargo_set_target_value(ctx,
                        opt, name, arg) < 0))
        {
            CARGODBG(1, "Failed to set value for no argument option\n");
            return CARGO_PARSE_FAIL_OPT;
        }
    }
    else
    {
        // Read until we find another option, or we've "eaten" the
        // arguments we want.
        for (ctx->j = start; ctx->j < (start + args_to_look_for); ctx->j++)
        {
            CARGODBG(3, "    argv[%i]: %s\n", ctx->j, argv[ctx->j]);

            if (_cargo_is_another_option(ctx, argv[ctx->j]))
            {
                // We found another option, stop parsing arguments
                // for this option.
                CARGODBG(3, "%s", "    Found other option\n");
                break;
            }

            if ((ret = _cargo_set_target_value(ctx, opt, name, argv[ctx->j])) < 0)
            {
                CARGODBG(1, "Failed to set target value for %s: \n", name);
                return CARGO_PARSE_FAIL_OPT;
            }

            // If we have exceeded opt->max_target_count
            // for CARGO_NARGS_ZERO_OR_MORE or CARGO_NARGS_ONE_OR_MORE
            // we should stop so we don't eat all the remaining arguments.
            if (ret)
                break;
        }
    }

    opt->parsed = ctx->i;
    opt->first_parse = 0; // This is not reset between calls to cargo_parse

    // Number of arguments eaten.
    opt->num_eaten = (ctx->j - start);

    CARGODBG(2, "_cargo_parse_option ate %d\n", opt->num_eaten);

    // If we're parsing using a custom callback, pass it onto that.
    if (opt->custom)
    {
        int custom_eaten = 0;

        // Set the value of the return count for the caller as well:
        // ... "[c]#", callback_func, &data, &data_count, DATA_COUNT);
        //                                   ^^^^^^^^^^^
        if (opt->custom_user_count)
        {
            CARGODBG(3, "Set custom user count: %lu\n", opt->custom_target_count);
            *opt->custom_user_count = opt->custom_target_count;
        }

        custom_eaten = opt->custom(ctx, opt->custom_user, opt->name[0],
                                    opt->custom_target_count, opt->custom_target);

        if (custom_eaten < 0)
        {
            CARGODBG(1, "Custom callback indicated error\n");
            return CARGO_PARSE_CALLBACK_ERR;
        }

        CARGODBG(2, "Custom call back ate: %d\n", custom_eaten);
    }

    if (opt->flags & CARGO_OPT_STOP)
    {
        ctx->stopped = ctx->j;
        CARGODBG(2, "%s: Stopping parse (index %d)\n", opt->name[0], ctx->stopped);

        if (opt->flags & CARGO_OPT_STOP_HARD)
        {
            ctx->stopped_hard = 1;
            CARGODBG(2, "%s: Stopping HARD (Mutex groups won't be checked)\n", opt->name[0]);
        }
    }

    return (opt->positional) ? opt->num_eaten : (opt->num_eaten + 1);
}

static int _cargo_compare_strlen(const void *a, const void *b)
{
    const char *as = *((const char **)a);
    const char *bs = *((const char **)b);
    size_t alen = strlen(as);
    size_t blen = strlen(bs);
    CARGODBG(4, "Compare length: %s (%lu) < %s (%lu) (%lu)\n",
            as, alen, bs, blen, (alen - blen));
    return (alen - blen);
}

static int _cargo_generate_metavar(cargo_t ctx, cargo_opt_t *opt, cargo_astr_t *str)
{
    int j = 0;
    int i = 0;
    char metavarname[20];
    assert(ctx);
    assert(opt);

    if (opt->type == CARGO_BOOL)
    {
        return 0;
    }

    while (_cargo_is_prefix(ctx, opt->name[0][i]))
    {
        i++;
    }

    while (opt->name[0][i] && (j < (sizeof(metavarname) - 1)))
    {
        metavarname[j++] = toupper(opt->name[0][i++]);
    }

    metavarname[j] = '\0';

    if (opt->nargs < 0)
    {
        // List the number of arguments.
        if (cargo_aappendf(str, "%s [%s ...]", metavarname, metavarname) < 0)
            return -1;
    }
    else if (opt->nargs > 0)
    {
        if (cargo_aappendf(str, "%s", metavarname) < 0) return -1;

        for (i = 1; (int)i < opt->nargs; i++)
        {
            if (cargo_aappendf(str, " %s", metavarname) < 0) return -1;
        }
    }
    else
    {
        if (cargo_aappendf(str, "%s", metavarname) < 0) return -1;
    }

    return 0;
}

static int _cargo_get_option_name_str(cargo_t ctx, cargo_opt_t *opt,
    char *namebuf, size_t buf_size)
{
    int ret = -1;
    size_t i;
    char **sorted_names = NULL;
    cargo_astr_t str;
    char *opt_name = NULL;
    assert(ctx);
    assert(opt);

    namebuf[0] = '\0';

    memset(&str, 0, sizeof(str));
    str.s = &opt_name;

    CARGODBG(3, "%s: Sorting %lu option names:\n", opt->name[0], opt->name_count);

    // Sort the names by length.
    {
        if (!(sorted_names = _cargo_calloc(opt->name_count, sizeof(char *))))
        {
            CARGODBG(1, "%s", "Out of memory\n");
            return -1;
        }

        for (i = 0; i < opt->name_count; i++)
        {
            if (!(sorted_names[i] = _cargo_strdup(opt->name[i])))
            {
                ret = -1; goto fail;
            }
        }

        qsort(sorted_names, opt->name_count,
            sizeof(char *), _cargo_compare_strlen);
    }

    // Print the option names.
    for (i = 0; i < opt->name_count; i++)
    {
        if (opt->positional)
            continue;

        if (cargo_aappendf(&str, "%s%s",
            sorted_names[i],
            (i + 1 != opt->name_count) ? ", " : "") < 0)
        {
            goto fail;
        }
    }

    // If the option has an argument, add a "metavar".
    if ((opt->nargs != 0) || opt->positional)
    {
        char *metavar = NULL;

        if (opt->metavar)
        {
            metavar = _cargo_strdup(opt->metavar);
        }
        else
        {
            cargo_astr_t metavar_str;
            memset(&metavar_str, 0, sizeof(metavar_str));
            metavar_str.s = &metavar;

            if (_cargo_generate_metavar(ctx, opt, &metavar_str))
            {
                CARGODBG(1, "Failed to generate metavar for %s\n", opt->name[0]);
                _cargo_xfree(&metavar);
                metavar = _cargo_strdup(opt->name[0]);
            }
        }

        cargo_aappendf(&str, " %s", metavar);
        _cargo_xfree(&metavar);
    }

    strncpy(namebuf, opt_name, buf_size);
    ret = strlen(namebuf);

fail:
    i = opt->name_count;
    _cargo_free_str_list(&sorted_names, &i);
    _cargo_xfree(&opt_name);
    return ret;
}

static char **_cargo_split(const char *s, const char *splitchars, size_t *count)
{
    char **ss;
    size_t i = 0;
    char *p = NULL;
    char *scpy = NULL;
    char *end = NULL;
    size_t splitlen = strlen(splitchars);
    assert(count);

    *count = 0;

    if (!s)
        return NULL;

    if (!*s)
        return NULL;

    if (!(scpy = _cargo_strdup(s)))
        return NULL;

    p = scpy;
    end = scpy + strlen(scpy);
    p += strspn(p, splitchars);

    *count = 1;

    while (*p)
    {
        // If the string ends in just splitchars
        // don't count the last empty string.
        if ((p + strspn(p, splitchars)) >= end)
        {
            break;
        }

        // Look for a split character.
        for (i = 0; i < splitlen; i++)
        {
            if (*p == splitchars[i])
            {
                (*count)++;
                break;
            }
        }

        // Consume all duplicates so we don't get
        // a bunch of empty strings.
        p += strspn(p, splitchars) + 1;
    }

    if (!(ss = _cargo_calloc(*count, sizeof(char *))))
        goto fail;

    p = strtok(scpy, splitchars);
    i = 0;

    while (p && (i < (*count)))
    {
        p += strspn(p, splitchars);

        if (!(ss[i] = _cargo_strdup(p)))
        {
            goto fail;
        }

        p = strtok(NULL, splitchars);
        i++;
    }

    // The count and number of strings must match.
    assert(i == *count);

    _cargo_free(scpy);

    return ss;
fail:
    _cargo_xfree(&scpy);
    _cargo_free_str_list(&ss, count);
    return NULL;
}

static char *_cargo_linebreak(cargo_t ctx, const char *str, size_t width)
{
    char *s = _cargo_strdup(str);
    char *start = s;
    char *prev = s;
    char *p = s;

    if (!s)
        return NULL;

    p = strpbrk(p, " \n");
    while (p != NULL)
    {
        CARGODBG(6, "(p - start) = %ld: %.*s\n",
            (p - start), (int)(p - start), start);

        // Restart on already existing explicit line breaks.
        if (*p == '\n')
        {
            CARGODBG(5, "EXISTING NEW LINE len %ld:\n%.*s\n\n",
                (p - start), (int)(p - start), start);
            start = p;
        }
        else if ((size_t)(p - start) > width)
        {
            // We found a word that goes beyond the width we're
            // aiming for, so add the line break before that word.
            *prev = '\n';
            CARGODBG(5, "ADD NEW LINE len %ld:\n%.*s\n\n",
                (prev - start), (int)(prev - start), start);
            start = prev;
            continue;
        }

        prev = p;
        p = strpbrk(p + 1, " \n");

        CARGODBG(5, "\n");
    }

    // Make sure the last line is also within "width".
    p = strrchr(s, '\n');
    if (!p) p = s;

    if ((size_t)((s + strlen(s)) - p) > width)
    {
        *prev = '\n';
    }

    return s;
}

static void _cargo_add_help_if_missing(cargo_t ctx)
{
    assert(ctx);

    if (!(ctx->flags & CARGO_NO_AUTOHELP)
        && _cargo_find_option_name(ctx, "--help", NULL, NULL))
    {
        if (cargo_add_option(ctx, 0, "--help", "Show this help", "b", &ctx->help))
        {
            CARGODBG(1, "Failed to add --help\n");
            return;
        }

        if (_cargo_find_option_name(ctx, "-h", NULL, NULL))
        {
            cargo_add_alias(ctx, "--help", "-h");
        }
    }
}

static int _cargo_damerau_levensthein_dist(const char *s, const char *t)
{
    #define d(i, j) dd[(i) * (m + 2) + (j) ]
    #define min(x, y) ((x) < (y) ? (x) : (y))
    #define min3(a, b, c) ((a) < (b) ? min((a), (c)) : min((b), (c)))
    #define min4(a, b, c, d) ((a) < (b) ? min3((a), (c), (d)) : min3((b), (c),(d)))

    int *dd;
    int DA[256];
    int i;
    int j;
    int cost;
    int i1;
    int j1;
    int DB;
    int n = (int)strlen(s);
    int m = (int)strlen(t);
    int max_dist = n + m;

    if (!(dd = (int *)_cargo_malloc((n + 2) * (m + 2) * sizeof(int))))
    {
        return -1;
    }

    memset(DA, 0, sizeof(DA));

    d(0, 0) = max_dist;

    for (i = 0; i < (n + 1); i++)
    {
        d(i + 1, 1) = i;
        d(i + 1, 0) = max_dist;
    }

    for (j = 0; j < (m + 1); j++)
    {
        d(1, j + 1) = j;
        d(0, j + 1) = max_dist;
    }

    for (i = 1; i < (n + 1); i++)
    {
        DB = 0;

        for(j = 1; j < (m + 1); j++)
        {
            i1 = DA[(int)t[j - 1]];
            j1 = DB;
            cost = ((s[ i - 1] == t[j - 1]) ? 0 : 1);

            if (cost == 0)
                DB = j;

            d(i + 1, j + 1) = min4(d(i, j) + cost,
                              d(i + 1, j) + 1,
                              d(i, j + 1) + 1,
                              d(i1, j1) + (i - i1 - 1) + 1 + (j - j1 - 1));
        }

        DA[(int)s[i - 1]] = i;
    }

    cost = d(n + 1, m + 1);
    _cargo_free(dd);
    return cost;

    #undef d
}

const char *_cargo_find_closest_opt(cargo_t ctx, const char *unknown)
{
    size_t i;
    size_t j;
    size_t maxi = 0;
    size_t maxj = 0;
    int min_dist = INT_MAX;
    int dist = 0;
    char *name = NULL;

    unknown += strspn(unknown, ctx->prefix);

    for (i = 0; i < ctx->opt_count; i++)
    {
        for (j = 0; j < ctx->options[i].name_count; j++)
        {
            name = ctx->options[i].name[j];
            name += strspn(name, ctx->prefix);

            dist = _cargo_damerau_levensthein_dist(unknown, name);

            if (dist < min_dist)
            {
                min_dist = dist;
                maxi = i;
                maxj = j;
            }
        }
    }

    return (min_dist <= 1) ? ctx->options[maxi].name[maxj] : NULL;
}

static int _cargo_fit_optnames_and_description(cargo_t ctx, cargo_astr_t *str,
                size_t i, int name_padding, int option_causes_newline, int max_name_len)
{
    size_t j;
    cargo_opt_t *opt = NULL;
    int ret = -1;
    assert(str);
    assert(ctx);

    char **desc_lines = NULL;
    size_t line_count = 0;

    int padding = 0;

    // We want to fit the opt names + description within max_width
    // We already know the width of the opt names (max_name_len)
    // so calculate how wide the description is allowed to be
    // including any spaces and padding between them.
    size_t max_desc_len =
        ctx->max_width  // Total width of names + description.
        - 2             // 2 for spaces before/after opt " --opt ".
        - max_name_len  // The longest of the opt names.
        - name_padding; // Padding.

    char *opt_description = NULL;

    CARGODBG(2, "max_desc_len = %lu\n", max_desc_len);
    CARGODBG(2, "str.l = %lu\n", str->l);
    CARGODBG(2, "str.offset = %lu\n", str->offset);
    opt = &ctx->options[i];

    // No description for option.
    if (!opt->description)
    {
        cargo_aappendf(str, "\n");
        return 0;
    }

    opt_description = _cargo_linebreak(ctx, opt->description,
                                            max_desc_len);

    if (!opt_description)
    {
        CARGODBG(1, "%s: Failed to line break option description\n", opt->name[0]);
        goto fail;
    }

    CARGODBG(5, "ctx->max_width - 2 - max_name_len - (2 * NAME_PADDING) =\n");
    CARGODBG(5, "%lu - 2 - %d - (2 * %d) = %lu\n",
        ctx->max_width, max_name_len,
        name_padding,
        max_desc_len);

    if (!(desc_lines = _cargo_split(opt_description, "\n", &line_count)))
    {
        CARGODBG(1, "Failed to split option description\n");
        goto fail;
    }

    for (j = 0; j < line_count; j++)
    {
        if ((j == 0) && !option_causes_newline)
        {
            // --theoption  Description <- First line of description.
            padding = 0;
        }
        else
        {
            // --theoption  Description
            //              continues here <- Now we want pre-padding.
            // ---------------------------------------------------------
            // --reallyreallyreallyreallylongoption
            //              Description    <- First line but pad anyway.
            //              continues here
            padding = max_name_len + name_padding;
        }

        if (cargo_aappendf(str, "  %*s%s\n",
            padding, "", desc_lines[j]) < 0)
        {
            CARGODBG(1, "Failed to append\n");
            goto fail;
        }
    }

    ret = 0;
fail:
    _cargo_xfree(&opt_description);
    _cargo_free_str_list(&desc_lines, &line_count);

    return ret;
}

static int _cargo_mutex_group_should_be_grouped(cargo_t ctx,
            cargo_opt_t *opt, int short_usage)
{
    cargo_group_t *mgrp = NULL;
    size_t i;
    assert(opt);

    for (i = 0; i < opt->mutex_group_count; i++)
    {
        assert(opt->mutex_group_idxs[i] < ctx->mutex_group_count);
        mgrp = &ctx->mutex_groups[opt->mutex_group_idxs[i]];

        if (mgrp->flags & (CARGO_MUTEXGRP_ORDER_BEFORE | CARGO_MUTEXGRP_ORDER_AFTER))
            continue;

        if (short_usage)
        {
            if (!(mgrp->flags & CARGO_MUTEXGRP_NO_GROUP_SHORT_USAGE))
            {
                return 1;
            }
        }
        else
        {
            if (mgrp->flags & CARGO_MUTEXGRP_GROUP_USAGE)
            {
                return 1;
            }
        }
    }

    return 0;
}

static int _cargo_print_options(cargo_t ctx,
                                size_t *opt_indices, size_t opt_count,
                                int show_positional, cargo_astr_t *str,
                                int max_name_len, int indent, int is_mutex,
                                cargo_usage_t flags)
{
    #define NAME_PADDING 2
    size_t i = 0;
    size_t opt_i;
    cargo_opt_t *opt = NULL;
    int option_causes_newline = 0;
    size_t namelen = 0;
    char *name = NULL;
    int ret = -1;
    assert(str);
    assert(ctx);

    // TODO: Replace with cargo_astr_t so we don't have to prealloc max_width
    if (!(name = _cargo_malloc(ctx->max_width)))
    {
        CARGODBG(1, "Out of memory!\n");
        return -1;
    }

    // Option names + descriptions.
    for (i = 0; i < opt_count; i++)
    {
        opt_i = opt_indices[i];
        opt = &ctx->options[opt_i];

        if (opt->flags & CARGO_OPT_HIDE)
        {
            continue;
        }

        // We don't show this in its normal position since it is a member of
        // a mutex group that should be grouped in the usage.
        if (_cargo_mutex_group_should_be_grouped(ctx, opt, 0) && !is_mutex)
        {
            continue;
        }

        // Skip options depending on type so we can group them by
        // calling this function multiple times.
        if ((show_positional && !opt->positional)
         || (!show_positional && opt->positional))
        {
            continue;
        }

        // TODO: Hmmm maybe store option max length in the opt struct instead when adding?
        namelen = _cargo_get_option_name_str(ctx, opt,
                                            name, ctx->max_width);

        // Is the option name so long we need a new line before the description?
        option_causes_newline = (int)strlen(name) > max_name_len;

        // Print the option names.
        // "  --ducks [DUCKS ...]  "
        if (cargo_aappendf(str, "%*s%-*s%s",
                indent + NAME_PADDING, " ",
                max_name_len, name,
                (option_causes_newline ? "\n" : "")) < 0)
        {
            goto fail;
        }

        // Option description.
        if ((flags & CARGO_USAGE_RAW_OPT_DESCRIPTIONS)
            || (opt->flags & CARGO_OPT_RAW_DESCRIPTION))
        {
            CARGODBG(5, "%s: RAW DESCRIPTION\n", opt->name[0]);

            if (cargo_aappendf(str, "%*s%s\n",
                NAME_PADDING, "", opt->description) < 0)
            {
                goto fail;
            }
        }
        else
        {
            // Add line breaks to fit the width we want.
            if (_cargo_fit_optnames_and_description(ctx, str, opt_i,
                    indent + NAME_PADDING, option_causes_newline, max_name_len))
            {
                goto fail;
            }
        }
    }

    ret = 0;

fail:
    _cargo_xfree(&name);
    return ret;
}

static int _cargo_get_short_option_usage(cargo_t ctx,
                                        cargo_opt_t *opt,
                                        cargo_astr_t *str,
                                        int is_positional,
                                        int show_is_optional)
{
    int is_req;
    char *metavar = NULL;
    assert(ctx);
    assert(opt);
    assert(str);

    if ((opt->flags & CARGO_OPT_HIDE) || (opt->flags & CARGO_OPT_HIDE_SHORT))
    {
        return 1;
    }

    // Positional arguments.
    if (is_positional && !opt->positional)
    {
        return 1;
    }

    // Options.
    if (!is_positional && opt->positional)
    {
        return 1;
    }

    if (opt->metavar)
    {
        metavar = _cargo_strdup(opt->metavar);
    }
    else
    {
        cargo_astr_t metavar_str;
        memset(&metavar_str, 0, sizeof(metavar_str)),
        metavar_str.s = &metavar;

        if (_cargo_generate_metavar(ctx, opt, &metavar_str))
        {
            CARGODBG(1, "Failed to generate metavar\n");
            return -1;
        }
    }

    is_req = (opt->flags & CARGO_OPT_REQUIRED);

    if (show_is_optional && !is_req)
    {
        cargo_aappendf(str, "[");
    }

    if (!opt->positional)
    {
        cargo_aappendf(str, "%s", opt->name[0]);
    }

    if (metavar && *metavar)
    {
        cargo_aappendf(str, " %s", metavar);
    }

    if (show_is_optional && !is_req)
    {
        cargo_aappendf(str, "]");
    }

    if (metavar)
        _cargo_free(metavar);

    return 0;
}

static void _cargo_fit_on_short_usage_line(cargo_t ctx, cargo_astr_t *str,
                                            size_t indent,
                                            size_t cur_offset,
                                            size_t opt_offset,
                                            size_t *prev_offset)
{
    assert(ctx);
    assert(prev_offset);
    assert(str);

    if ((cur_offset + opt_offset - (*prev_offset)) >= ctx->max_width)
    {
        *prev_offset = cur_offset;
        cargo_aappendf(str, "\n%*s", indent, " ");
    }
}

static int _cargo_get_short_option_usages(cargo_t ctx,
                                        cargo_astr_t *str,
                                        size_t indent,
                                        int is_positional)
{
    size_t i;
    int ret;
    cargo_astr_t opt_str;
    size_t prev_offset = 0;
    cargo_opt_t *opt = NULL;
    char *opt_s = NULL;
    assert(ctx);
    assert(str);

    for (i = 0; i < ctx->opt_count; i++)
    {
        memset(&opt_str, 0, sizeof(opt_str));
        opt_str.s = &opt_s;
        opt = &ctx->options[i];

        if (opt->flags & CARGO_OPT_HIDE)
        {
            continue;
        }

        // We display this seperately in _cargo_mutex_group_short_usage
        if (_cargo_mutex_group_should_be_grouped(ctx, opt, 1))
        {
            continue;
        }

        cargo_aappendf(&opt_str, " ");

        if ((ret = _cargo_get_short_option_usage(ctx, opt,
                                &opt_str, is_positional, 1)) < 0)
        {
            CARGODBG(1, "Failed to get option usage\n");
            goto fail;
        }

        if (ret == 1)
        {
            _cargo_xfree(&opt_s);
            continue;
        }

        // Does the option fit on this line?
        _cargo_fit_on_short_usage_line(ctx, str,
            indent, str->offset, opt_str.offset, &prev_offset);

        if (opt_s)
        {
            cargo_aappendf(str, "%s", opt_s);
            _cargo_xfree(&opt_s);
        }
    }

    return 0;
fail:
    _cargo_xfree(&opt_s);
    return -1;
}

static char **_cargo_split_and_verify_option_names(cargo_t ctx,
                                                    const char *optnames,
                                                    size_t *optcount)
{
    char **optname_list = NULL;
    char *tmp = NULL;
    assert(ctx);
    assert(optcount);

    if (!(tmp = _cargo_strdup(optnames)))
    {
        return NULL;
    }

    if (!(optname_list = _cargo_split(tmp, " ", optcount))
        || (*optcount <= 0))
    {
        CARGODBG(1, "Failed to split option name list: \"%s\"\n", optnames);
        goto fail;
    }

    #ifdef CARGO_DEBUG
    {
        size_t i;
        CARGODBG(3, "Got %lu option names:\n", *optcount);
        for (i = 0; i < *optcount; i++)
        {
            CARGODBG(3, " %s\n", optname_list[i]);
        }
    }
    #endif // CARGO_DEBUG

    if (!_cargo_find_option_name(ctx, optname_list[0], NULL, NULL))
    {
        CARGODBG(1, "%s already exists\n", optname_list[0]);
        goto fail;
    }

    // If this is a positional argument it has to start with
    // [a-zA-Z]
    if (!_cargo_starts_with_prefix(ctx, optname_list[0]))
    {
        if (strpbrk(optname_list[0],
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
            != optname_list[0])
        {
            CARGODBG(1, "A positional argument must start with [a-zA-Z]\n");
            goto fail;
        }
    }

    _cargo_free(tmp);
    return optname_list;

fail:
    _cargo_xfree(&tmp);
    _cargo_free_str_list(&optname_list, optcount);

    return NULL;
}

static cargo_opt_t *_cargo_option_init(cargo_t ctx,
                                        const char *name,
                                        const char *description)
{
    char *optname = NULL;
    cargo_opt_t *o = NULL;
    assert(ctx);

    if (_cargo_grow_options(&ctx->options, &ctx->opt_count, &ctx->max_opts))
    {
        return NULL;
    }

    o = &ctx->options[ctx->opt_count];
    memset(o, 0, sizeof(cargo_opt_t));
    ctx->opt_count++;

    if (o->name_count >= CARGO_NAME_COUNT)
    {
        CARGODBG(1, "Max %d names allowed\n", CARGO_NAME_COUNT);
        return NULL;
    }

    if (!(optname = _cargo_strdup(name)))
    {
        CARGODBG(1, "Out of memory\n");
        return NULL;
    }

    o->name[o->name_count] = optname;
    o->name_count++;

    if (description && !(o->description = _cargo_strdup(description)))
    {
        CARGODBG(1, "Out of memory\n");
        return NULL;
    }

    return o;
}

static void _cargo_option_destroy(cargo_opt_t *o)
{
    size_t j;

    if (!o)
    {
        return;
    }

    for (j = 0; j < o->name_count; j++)
    {
        CARGODBG(2, "###### FREE OPTION NAME: %s\n", o->name[j]);
        _cargo_xfree(&o->name[j]);
    }

    o->name_count = 0;

    _cargo_xfree(&o->description);
    _cargo_xfree(&o->metavar);
    _cargo_xfree(&o->bool_acc);
    o->bool_acc_count = 0;
    o->bool_acc_max_count = 0;

    // Special case for custom callback target, it is allocated
    // internally so we should always auto clean it.
    _cargo_free_str_list(&o->custom_target, &o->custom_target_count);
    _cargo_free_str_list(&o->mutex_group_names, &o->mutex_group_count);

    _cargo_option_destroy_validation(o);
}

typedef struct cargo_fmt_token_s
{
    int column;
    char token;
} cargo_fmt_token_t;

typedef struct cargo_fmt_scanner_s
{
    const char *fmt;
    const char *start;
    cargo_fmt_token_t prev_token;
    cargo_fmt_token_t token;
    cargo_fmt_token_t next_token;

    int column;
} cargo_fmt_scanner_t;

static char _cargo_fmt_token(cargo_fmt_scanner_t *s)
{
    return s->token.token;
}

static void _cargo_fmt_scanner_init(cargo_fmt_scanner_t *s,
                                    const char *fmt)
{
    assert(s);
    memset(s, 0, sizeof(cargo_fmt_scanner_t));

    CARGODBG(2, "FMT scanner init: \"%s\"\n", fmt);
    s->fmt = fmt;
    s->start = fmt;
    s->column = 0;

    s->token.token = *fmt;
}

static void _cargo_fmt_next_token(cargo_fmt_scanner_t *s)
{
    const char *fmt;

    CARGODBG(4, "\"%s\"\n", s->start);
    CARGODBG(4, " %*s\n", s->token.column, "^");

    s->prev_token = s->token;

    s->column++;

    fmt = s->fmt;
    while ((*fmt == ' ') || (*fmt == '\t'))
    {
        s->column++;
        fmt++;
    }

    s->token.token = *fmt;
    s->token.column = s->column;

    fmt++;
    s->fmt = fmt;
}

static void _cargo_fmt_prev_token(cargo_fmt_scanner_t *s)
{
    s->next_token = s->token;
    s->token = s->prev_token;
    s->fmt = &s->start[s->token.column];

    CARGODBG(4, "\"%s\"\n", s->start);
    CARGODBG(4, " %*s\n", s->token.column, "^");
}

static const char *_cargo_get_option_group_names(cargo_t ctx,
                                        const char *optnames,
                                        char **grpname,
                                        char **mutex_grpname)
{
    int len = 0;
    char *end = NULL;
    char *tmp = NULL;
    char **groups = NULL;
    const char *ret = NULL;
    char *s = NULL;
    size_t count;
    size_t i;
    assert(ctx);
    assert(grpname);
    assert(mutex_grpname);

    *grpname = NULL;

    // Skip any whitespace.
    optnames += strspn(optnames, " \t");

    if (*optnames != '<')
    {
        return optnames;
    }

    if (!(end = strchr(optnames, '>')))
    {
        CARGODBG(1, "Missing '>' to terminate group name\n");
        return NULL;
    }

    end--;

    len = (end - optnames);

    if (!(tmp = cargo_strndup(&optnames[1], len)))
    {
        CARGODBG(1, "Out of memory!\n");
        return NULL;
    }

    if (!(groups = _cargo_split(tmp, ",", &count)))
    {
        CARGODBG(1, "Failed to split group names\n");
        goto fail;
    }

    if (count > 2)
    {
        CARGODBG(1, "Invalid number of groups specified. "
                    "1 normal and 1 mutex group allowed\n");
        goto fail;
    }

    for (i = 0; i < count; i++)
    {
        s = groups[i];
        s += strspn(s, " \t");

        if (s[0] == '!')
        {
            if (!(*mutex_grpname = _cargo_strdup(&s[1])))
            {
                CARGODBG(1, "Out of memory!\n");
                goto fail;
            }
        }
        else
        {
            if (!(*grpname = _cargo_strdup(s)))
            {
                CARGODBG(1, "Out of memory!\n");
                goto fail;
            }
        }
    }

    optnames += len + 2;
    ret = optnames;

fail:
    _cargo_xfree(&tmp);

    if (!ret)
    {
        _cargo_xfree(grpname);
        _cargo_xfree(mutex_grpname);
    }

    _cargo_free_str_list(&groups, &count);

    return ret;
}

static void _cargo_group_destroy(cargo_group_t *g)
{
    if (!g) return;
    _cargo_xfree(&g->option_indices);
    _cargo_xfree(&g->name);
    _cargo_xfree(&g->title);
    _cargo_xfree(&g->description);
    _cargo_xfree(&g->metavar);
    g->opt_count = 0;
}

static void _cargo_groups_destroy(cargo_t ctx)
{
    size_t i;
    assert(ctx);

    if (ctx->groups)
    {
        for (i = 0; i < ctx->group_count; i++)
        {
            _cargo_group_destroy(&ctx->groups[i]);
        }

        _cargo_xfree(&ctx->groups);
    }

    if (ctx->mutex_groups)
    {
        for (i = 0; i < ctx->mutex_group_count; i++)
        {
            _cargo_group_destroy(&ctx->mutex_groups[i]);
        }

        _cargo_xfree(&ctx->mutex_groups);
    }
}

static cargo_group_t *_cargo_find_group(cargo_t ctx,
                    cargo_group_t *groups, size_t group_count,
                    const char *name, size_t *grp_i)
{
    size_t i;
    cargo_group_t *g;
    assert(ctx);
    assert(name);

    if (!groups)
        return NULL;

    for (i = 0; i < group_count; i++)
    {
        g = &groups[i];

        if (!strcmp(g->name, name))
        {
            if (grp_i) *grp_i = i;
            return g;
        }
    }

    return NULL;
}

static int _cargo_add_group(cargo_t ctx,
                    cargo_group_t **groups, size_t *group_count,
                    size_t *max_groups,
                    size_t flags, const char *name,
                    const char *title, const char *description)
{
    int ret = -1;
    cargo_group_t *grp = NULL;
    assert(ctx);
    assert(name);
    assert(groups);
    assert(max_groups);
    assert(group_count);

    CARGODBG(2, "Add group %s (%lu)\n", name, *group_count);

    // Initial allocation.
    if (!*groups)
    {
        (*max_groups) = CARGO_DEFAULT_MAX_GROUPS;

        if (!((*groups) = _cargo_calloc(*max_groups, sizeof(cargo_group_t))))
        {
            CARGODBG(1, "Out of memory!\n");
            return -1;
        }
    }

    if (_cargo_find_group(ctx, *groups, *group_count, name, NULL))
    {
        CARGODBG(1, "Group \"%s\" already exists\n", name);
        return -1;
    }

    // Realloc if needed.
    if (*group_count >= *max_groups)
    {
        (*max_groups) *= 2;

        if (!((*groups) = _cargo_realloc(*groups,
            sizeof(cargo_group_t) * (*max_groups))))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }
    }

    grp = &(*groups)[*group_count];
    memset(grp, 0, sizeof(cargo_group_t));

    if (!(grp->name = _cargo_strdup(name)))
    {
        CARGODBG(1, "Out of memory!\n");
        goto fail;
    }

    if (title)
    {
        if (!(grp->title = _cargo_strdup(title)))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }
    }
    else
    {
        if (!(grp->title = _cargo_strdup(name)))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }
    }

    if (description)
    {
        if (!(grp->description = _cargo_strdup(description)))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }
    }

    grp->flags = flags;
    grp->max_opt_count = CARGO_DEFAULT_MAX_GROUP_OPTS;
    grp->opt_count = 0;

    if (!(grp->option_indices = _cargo_calloc(grp->max_opt_count, sizeof(size_t))))
    {
        CARGODBG(1, "Out of memory!\n");
        goto fail;
    }

    (*group_count)++;
    CARGODBG(3, "  group_count after: %lu\n", *group_count);

    ret = 0;

fail:
    if (ret < 0)
    {
        _cargo_group_destroy(grp);
    }

    return ret;
}

static int _cargo_group_add_option_ex(cargo_t ctx,
                                        cargo_group_t *groups,
                                        size_t group_count,
                                        const char *group,
                                        const char *opt,
                                        int is_mutex)
{
    size_t opt_i;
    cargo_group_t *g = NULL;
    size_t grp_i;
    cargo_opt_t *o = NULL;
    assert(ctx);
    assert(groups);
    assert(group);
    assert(opt);

    CARGODBG(2, "+++++++ Add %s to group \"%s\" +++++++\n", opt, group);

    if (!(g = _cargo_find_group(ctx, groups, group_count, group, &grp_i)))
    {
        CARGODBG(1, "No such group \"%s\"\n", group);
        return -1;
    }

    if (_cargo_find_option_name(ctx, opt, &opt_i, NULL))
    {
        CARGODBG(1, "No such option \"%s\"\n", opt);
        return -1;
    }

    assert(opt_i < ctx->opt_count);
    o = &ctx->options[opt_i];

    if (!is_mutex && (o->group_index > 0))
    {
        CARGODBG(1, "\"%s\" is already in another group \"%s\"\n",
                o->name[0], ctx->groups[o->group_index].name);
        return -1;
    }

    if (g->opt_count >= g->max_opt_count)
    {
        CARGODBG(2, "Realloc group max option count from %lu to %lu\n",
                g->max_opt_count, 2 * g->max_opt_count);

        g->max_opt_count *= 2;

        if (!(g->option_indices = _cargo_realloc(g->option_indices,
                g->max_opt_count * sizeof(size_t))))
        {
            CARGODBG(1, "Out of memory!\n");
            return -1;
        }
    }

    g->option_indices[g->opt_count] = opt_i;

    // Save index in option
    // (since we might realloc the array of groups we must use the index)
    if (is_mutex)
    {
        if (o->mutex_group_count == CARGO_MAX_OPT_MUTEX_GROUP)
        {
            CARGODBG(1, "Option %s cannot belong to more mutex groups. "
                       "Count is CARGO_MAX_OPT_MUTEX_GROUP (%d).\n",
                       opt, CARGO_MAX_OPT_MUTEX_GROUP);
            return -1;
        }

        o->mutex_group_idxs[o->mutex_group_count++] = grp_i;
    }
    else
    {
        o->group_index = grp_i;
    }

    g->opt_count++;

    CARGODBG(2, "   Group \"%s\" option count: %lu\n", g->name, g->opt_count);

    return 0;
}

static void _cargo_print_mutex_group(cargo_t ctx,
                                     size_t start_index,
                                     cargo_astr_t *str,
                                     cargo_group_t *g)
{
    size_t i;
    cargo_opt_t *opt = NULL;
    assert(g);
    assert(str);

    if (start_index >= g->opt_count)
        return;

    for (i = start_index; i < g->opt_count; i++)
    {
        opt = &ctx->options[g->option_indices[i]];
        cargo_aappendf(str, "%s%s", opt->name[0],
                (i < (g->opt_count - 1)) ? ", " : "\n");
    }
}

static void _cargo_print_mutex_group_highlights(cargo_t ctx,
                                            cargo_astr_t *str,
                                            cargo_highlight_t *parse_highlights,
                                            size_t parsed_count)
{
    char *s;

    if (!(s = cargo_get_fprintl_args(ctx->argc, ctx->argv, ctx->start,
                _cargo_get_cflag(ctx), ctx->max_width,
                parsed_count, parse_highlights)))
    {
        CARGODBG(1, "Out of memory\n");
        return;
    }

    cargo_aappendf(str, "%s\n", s);
    _cargo_free(s);
}

static int _cargo_check_mutex_group(cargo_t ctx,
                                    cargo_astr_t *str,
                                    cargo_group_t *g)
{
    cargo_opt_t *opt = NULL;
    size_t j = 0;
    int ret = -1;
    cargo_highlight_t *parse_highlights = NULL;
    size_t parsed_count = 0;
    assert(ctx);
    assert(g);
    assert(str);

    parsed_count = 0;

    // We create a list of highlights, so if more than one option in the
    // mutex groups is parsed, we can highlight it.
    if (!(parse_highlights = _cargo_calloc(g->opt_count,
                                sizeof(cargo_highlight_t))))
    {
        CARGODBG(1, "Out of memory!\n");
        goto fail;
    }

    for (j = 0; j < g->opt_count; j++)
    {
        opt = &ctx->options[g->option_indices[j]];

        if (opt->parsed >= 0)
        {
            parse_highlights[parsed_count].i = opt->parsed;
            parse_highlights[parsed_count].c = "~"CARGO_COLOR_RED;
            parsed_count++;
        }
    }

    if (parsed_count > 1)
    {
        _cargo_print_mutex_group_highlights(ctx, str, parse_highlights, parsed_count);
        cargo_aappendf(str, "Only one of these variables is allowed at the same time:\n");
        _cargo_print_mutex_group(ctx, 0, str, g);
        goto fail;
    }
    else if ((parsed_count == 0)
            && (g->flags & CARGO_MUTEXGRP_ONE_REQUIRED))
    {
        cargo_aappendf(str, "One of these variables is required:\n");
        _cargo_print_mutex_group(ctx, 0, str, g);
        goto fail;
    }

    ret = 0;

fail:
    _cargo_xfree(&parse_highlights);
    return ret;
}

static int _cargo_check_order_mutex_group(cargo_t ctx,
                                          cargo_astr_t *str,
                                          cargo_group_t *g)
{
    int ret = -1;
    size_t i = 0;
    cargo_opt_t *opt = NULL;
    cargo_opt_t *first_opt = NULL;
    cargo_highlight_t *parse_highlights = NULL;
    int first_i = -1;
    int is_invalid = 0;
    size_t invalid_order_count = 0;
    assert(ctx);
    assert(str);
    assert(g);
    assert(g->flags & (CARGO_MUTEXGRP_ORDER_BEFORE | CARGO_MUTEXGRP_ORDER_AFTER));

    if (g->opt_count == 0)
    {
        CARGODBG(1, "Warning: No options in mutex order group \"%s\"\n", g->name);
        return 0;
    }

    // We create a list of highlights, so if more than one option in the
    // mutex groups is parsed, we can highlight it.
    if (!(parse_highlights = _cargo_calloc(g->opt_count,
                                sizeof(cargo_highlight_t))))
    {
        CARGODBG(1, "Out of memory!\n");
        goto fail;
    }

    // We compare all other options with the parse index of the first one.
    first_opt = &ctx->options[g->option_indices[0]];
    first_i = first_opt->parsed;

    parse_highlights[invalid_order_count].i = first_i;
    parse_highlights[invalid_order_count].c = "^"CARGO_COLOR_GREEN;

    invalid_order_count = 1;

    for (i = 1; i < g->opt_count; i++)
    {
        is_invalid = 0;
        opt = &ctx->options[g->option_indices[i]];

        // Skip unparsed.
        if (opt->parsed < 0)
        {
            CARGODBG(3, "  Not parsed skipping %s\n", opt->name[0]);
            continue;
        }

        CARGODBG(3, "  Check mutex order for %s\n", opt->name[0]);

        if (g->flags & CARGO_MUTEXGRP_ORDER_BEFORE)
        {
            if (opt->parsed > first_i)
            {
                is_invalid = 1;
            }
        }
        else
        {
            if (opt->parsed < first_i)
            {
                is_invalid = 1;
            }
        }

        if (is_invalid)
        {
            CARGODBG(3, "     Invalid order, highlight index %d\n", opt->parsed);
            parse_highlights[invalid_order_count].i = opt->parsed;
            parse_highlights[invalid_order_count].c = "~"CARGO_COLOR_RED;
            invalid_order_count++;
        }
    }

    if (invalid_order_count > 1)
    {
        CARGODBG(3, "  Invalid order highlight count: %lu\n", invalid_order_count);
        _cargo_print_mutex_group_highlights(ctx, str, parse_highlights, invalid_order_count);
        cargo_aappendf(str, "These options must all be specified %s \"%s\":\n",
            (g->flags & CARGO_MUTEXGRP_ORDER_BEFORE) ? "before" : "after",
            first_opt->name[0]);
        _cargo_print_mutex_group(ctx, 1, str, g);
        goto fail;
    }

    ret = 0;

fail:
    _cargo_xfree(&parse_highlights);
    return ret;
}

static cargo_parse_result_t _cargo_check_mutex_groups(cargo_t ctx)
{
    cargo_astr_t str;
    char *error = NULL;
    int ret = CARGO_PARSE_MUTEX_CONFLICT;
    size_t i = 0;
    cargo_group_t *g = NULL;
    assert(ctx);
    memset(&str, 0, sizeof(cargo_astr_t));
    str.s = &error;

    CARGODBG(2, "Check mutex %lu groups\n", ctx->mutex_group_count);

    for (i = 0; i < ctx->mutex_group_count; i++)
    {
        g = &ctx->mutex_groups[i];

        if (g->flags & (CARGO_MUTEXGRP_ORDER_BEFORE | CARGO_MUTEXGRP_ORDER_AFTER))
        {
            CARGODBG(2, "Order mutex group %s:\n", g->name);

            // Check that all options in the group is either before or after
            // the first option of the group.
            if (_cargo_check_order_mutex_group(ctx, &str, g))
            {
                ret = CARGO_PARSE_MUTEX_CONFLICT_ORDER; goto fail;
            }
        }
        else
        {
            CARGODBG(2, "Normal mutex group %s:\n", g->name);

            // Check that only one of the group is selected.
            if (_cargo_check_mutex_group(ctx, &str, g))
            {
                ret = CARGO_PARSE_MUTEX_CONFLICT; goto fail;
            }
        }
    }

    ret = CARGO_PARSE_OK;
fail:
    if (ret)
    {
        _cargo_set_error(ctx, error);
    }

    return ret;
}

static int _cargo_add_orphans_to_default_group(cargo_t ctx)
{
    // Instead of being able to delete options from groups
    // we add any orphans to the default group at first use.
    size_t i;
    cargo_opt_t *opt = NULL;
    assert(ctx);

    for (i = 0; i < ctx->opt_count; i++)
    {
        opt = &ctx->options[i];

        // Default group.
        if (opt->group_index < 0)
        {
            if (cargo_group_add_option(ctx, "", opt->name[0]))
            {
                CARGODBG(1, "Failed to add \"%s\" to default group\n", opt->name[0]);
                return -1;
            }
        }
    }

    return 0;
}

static int _cargo_is_arg_negative_integer(const char *arg)
{
    char *end = NULL;
    long long int i = strtoll(arg, &end, 10);
    return (i < 0);
}

static void _cargo_check_unknown_options_gather(cargo_t ctx,
                                                int start, int end)
{
    cargo_opt_t *opt = NULL;
    char *arg = NULL;
    const char *name = NULL;
    assert(ctx);

    // TODO: Add support for options with negative numbers.
    for (ctx->i = start; ctx->i < end; )
    {
        arg = ctx->argv[ctx->i];

        if (_cargo_starts_with_prefix(ctx, arg)
        && !_cargo_is_arg_negative_integer(arg))
        {
            if (!(name = _cargo_check_options(ctx, &opt, arg)))
            {
                CARGODBG(2, "    Unknown option: %s\n", arg);
                ctx->unknown_opts[ctx->unknown_opts_count] = arg;
                ctx->unknown_opts_idxs[ctx->unknown_opts_count] = ctx->i;
                ctx->unknown_opts_count++;
            }
        }

        ctx->i++;
    }
}

static cargo_parse_result_t _cargo_check_unknown_options(cargo_t ctx)
{
    cargo_parse_result_t ret = CARGO_PARSE_UNKNOWN_OPTS;
    size_t i;
    cargo_highlight_t *highlights = NULL;
    cargo_astr_t str;
    char *error = NULL;
    memset(&str, 0, sizeof(str));
    str.s = &error;
    assert(ctx);

    // We could do a first pass for unknown options the first thing we do.
    // Default is to wait until after parsing.
    if (ctx->flags & CARGO_UNKNOWN_EARLY)
    {
        CARGODBG(2, "Check for unknown options before parsing.\n"
                    "   CARGO_UNKNOWN_EARLY is set\n"
                    "    Check between %d and %d\n", ctx->start, ctx->stopped);
        _cargo_check_unknown_options_gather(ctx, ctx->start, ctx->argc);
    }
    else
    {
        CARGODBG(2, "Check for unknown options after parsing.\n"
                    "    CARGO_UNKNOWN_EARLY is NOT set\n"
                    "    Check between %d and %d\n", ctx->start, ctx->stopped);
        _cargo_check_unknown_options_gather(ctx, ctx->start, ctx->stopped);
    }

    if (ctx->unknown_opts_count > 0)
    {
        const char *suggestion = NULL;
        char *s = NULL;

        if (ctx->error)
        {
            cargo_aappendf(&str, "%s\n", ctx->error);
        }

        CARGODBG(2, "Unknown options count: %lu\n", ctx->unknown_opts_count);
        cargo_aappendf(&str, "Unknown options:\n");

        if (!(highlights = _cargo_calloc(ctx->unknown_opts_count,
                                sizeof(cargo_highlight_t))))
        {
            ret = CARGO_PARSE_NOMEM; goto fail;
        }

        for (i = 0; i < ctx->unknown_opts_count; i++)
        {
            highlights[i].i = ctx->unknown_opts_idxs[i];
            highlights[i].c = "~"CARGO_COLOR_RED;
        }

        if (!(s = cargo_get_fprintl_args(ctx->argc, ctx->argv, ctx->start,
            _cargo_get_cflag(ctx), ctx->max_width,
            ctx->unknown_opts_count, highlights)))
        {
            CARGODBG(1, "Out of memory\n");
            ret = CARGO_PARSE_NOMEM; goto fail;
        }

        cargo_aappendf(&str, "%s\n", s);

        for (i = 0; i < ctx->unknown_opts_count; i++)
        {
            suggestion = _cargo_find_closest_opt(ctx, ctx->unknown_opts[i]);
            if (!suggestion) continue;

            cargo_aappendf(&str, "%s ", ctx->unknown_opts[i]);
            cargo_aappendf(&str, " (Did you mean %s)?", suggestion);
            cargo_aappendf(&str, "\n");
        }

        _cargo_xfree(&highlights);
        _cargo_xfree(&s);

        if (!(ctx->flags & CARGO_NO_FAIL_UNKNOWN))
        {
            _cargo_set_error(ctx, error);
            return ret;
        }
        else
        {
            CARGODBG(2, "No error on unknown options, CARGO_NO_FAIL_UNKNOWN set\n");
        }
    }

    ret = CARGO_PARSE_OK;

fail:
    // We failed to set the error...
    _cargo_xfree(&error);
    _cargo_xfree(&highlights);
    return ret;
}

static cargo_parse_result_t _cargo_check_unknown_options_after(cargo_t ctx)
{
    assert(ctx);

    // This check has already been done.
    if (ctx->flags & CARGO_UNKNOWN_EARLY)
    {
        return CARGO_PARSE_OK;
    }

    // We must have a stop index so we know for what span to check for unknown
    // options. Any options remaining after the stop index will be excluded in
    // this check.
    if (!ctx->stopped)
    {
        CARGODBG(2, "Stop at end of argv after parse: %d\n", ctx->argc);
        ctx->stopped = ctx->argc;
    }
    else
    {
        CARGODBG(2, "Stopped by option at index: %d\n", ctx->stopped);
    }

    // TODO: Only append errors.
    return _cargo_check_unknown_options(ctx);
}

static void _cargo_parse_show_error(cargo_t ctx)
{
    FILE *fd = (ctx->flags & CARGO_STDOUT_ERR) ? stdout : stderr;
    assert(ctx);

    if (!ctx->error)
        return;

    if (!(ctx->flags & CARGO_NOERR_USAGE))
    {
        cargo_fprint_usage(ctx, fd, ctx->usage_flags);
    }

    // Show errors automatically?
    if (!(ctx->flags & CARGO_NOERR_OUTPUT))
    {
        fprintf(fd, "%s\n", ctx->error);
    }
}

static int _cargo_get_max_name_length(cargo_t ctx,
            size_t *positional_count, size_t *option_count)
{
    #define MAX_OPT_NAME_LEN 40
    size_t i = 0;
    int namelen = 0;
    int max_name_len = 0;
    char *name = NULL;
    cargo_opt_t *opt = NULL;
    assert(ctx);
    assert(positional_count);
    assert(option_count);

    (*positional_count) = 0;
    (*option_count) = 0;

    // TODO: Replace with cargo_astr_t so we don't have to prealloc max_width
    if (!(name = _cargo_malloc(ctx->max_width)))
    {
        CARGODBG(1, "Out of memory!\n");
        return -1;
    }

    for (i = 0; i < ctx->opt_count; i++)
    {
        opt = &ctx->options[i];

        if (opt->flags & CARGO_OPT_HIDE)
        {
            continue;
        }

        if (opt->positional)
        {
            (*positional_count)++;
        }
        else
        {
            (*option_count)++;
        }

        namelen = _cargo_get_option_name_str(ctx, opt,
                                            name, ctx->max_width);

        if (namelen < 0)
        {
            max_name_len = -1;
            goto fail;
        }

        // Get the longest option name.
        // (However, if it's too long don't count it, then we'll just
        // do a line break before printing the description).
        if ((namelen > max_name_len) && (namelen <= MAX_OPT_NAME_LEN))
        {
            max_name_len = namelen;
        }
    }

fail:
    _cargo_xfree(&name);

    return max_name_len;
}

static int _cargo_get_group_description(cargo_t ctx, cargo_astr_t *str,
                                        cargo_group_t *grp, int indent)
{
    int ret = -1;
    size_t j = 0;
    char *lb_desc = NULL;
    char **desc_lines = NULL;
    size_t line_count = 0;
    assert(ctx);
    assert(str);
    assert(grp);

    if (!grp->description || strlen(grp->description) == 0)
    {
        return 0;
    }

    if (grp->flags & CARGO_GROUP_RAW_DESCRIPTION)
    {
        cargo_aappendf(str, "%*s%s\n", indent, " ", grp->description);
    }
    else
    {
        if (!(lb_desc = _cargo_linebreak(ctx, grp->description, ctx->max_width)))
        {
            goto fail;
        }

        if (!(desc_lines = _cargo_split(lb_desc, "\n", &line_count)))
        {
            CARGODBG(1, "Failed to split group description\n");
            goto fail;
        }

        for (j = 0; j < line_count; j++)
        {
            cargo_aappendf(str, "%*s%s\n", indent, " ", desc_lines[j]);
        }
    }

    ret = 0;
fail:
    _cargo_xfree(&lb_desc);
    _cargo_free_str_list(&desc_lines, &line_count);

    return ret;
}

void _cargo_invalid_format_char(cargo_t ctx,
                                    const char *optname, const char *fmt,
                                    cargo_fmt_scanner_t *s)
{
    assert(ctx);
    assert(optname);
    assert(s);

    CARGODBG(1, "  %s: Unknown format character '%c' at index %d\n",
            optname, _cargo_fmt_token(s), s->column);
    CARGODBG(1, "      \"%s\"\n", fmt);
    CARGODBG(1, "       %*s\n", s->column, "^");
}

static int _cargo_check_required_options(cargo_t ctx)
{
    cargo_astr_t errstr;
    char *error = NULL;
    size_t i;
    cargo_opt_t *opt = NULL;
    memset(&errstr, 0, sizeof(cargo_astr_t));
    errstr.s = &error;

    for (i = 0; i < ctx->opt_count; i++)
    {
        opt = &ctx->options[i];

        if ((opt->flags & CARGO_OPT_REQUIRED) && (opt->parsed < 0))
        {
            CARGODBG(1, "Missing required argument \"%s\"\n", opt->name[0]);
            cargo_aappendf(&errstr, "Missing required argument \"%s\"\n", opt->name[0]);

            _cargo_set_error(ctx, error);
            return -1;
        }

        if (opt->parsed >= 0)
        {
            if (((opt->nargs == CARGO_NARGS_ONE_OR_MORE) && (opt->num_eaten == 0))
             || ((opt->nargs >= 0) && (opt->num_eaten != opt->nargs)))
            {
                CARGODBG(1, "Not enough arguments. Expected %s, got %d\n",
                        _cargo_nargs_str(opt->nargs), opt->num_eaten);

                // TODO: Highlight option (ctx->parsed is the index into argv)

                if (opt->num_eaten == 0)
                {
                    cargo_aappendf(&errstr,
                        "Not enough arguments for \"%s\" expected %s "
                        "but got none\n", opt->name[0],
                        _cargo_nargs_str(opt->nargs));
                }
                else
                {
                    cargo_aappendf(&errstr,
                        "Not enough arguments for \"%s\" expected %s "
                        "but got only %d\n", opt->name[0],
                        _cargo_nargs_str(opt->nargs), opt->num_eaten);
                }

                _cargo_set_error(ctx, error);
                return -1;
            }
        }
    }

    return 0;
}

static size_t _cargo_process_max_width(size_t max_width)
{
    int console_width;
    size_t maxw = CARGO_DEFAULT_MAX_WIDTH;

    if (max_width == CARGO_AUTO_MAX_WIDTH)
    {
        CARGODBG(2, "User specified CARGO_AUTO_MAX_WIDTH\n");

        if ((console_width = _cargo_get_console_width()) > 0)
        {
            CARGODBG(2, "Max width based on console width: %d\n", console_width);
            maxw = console_width;
        }
        else
        {
            CARGODBG(2, "Max width set to CARGO_DEFAULT_MAX_WIDTH = %d\n",
                    CARGO_DEFAULT_MAX_WIDTH);
        }
    }
    else
    {
        CARGODBG(2, "User specified max width: %lu\n", max_width);
        maxw = max_width;
    }

    // Since we allocate memory based on this later on, make sure this
    // is something sane. Anything above 1024 characters is more than enough.
    // Also some systems might have a really big console width for instance.
    // (This happened on drone.io continous integration service).
    if (maxw > CARGO_MAX_MAX_WIDTH)
    {
        CARGODBG(2, "Max width too large, capping at: %d\n", CARGO_MAX_MAX_WIDTH);
        maxw = CARGO_MAX_MAX_WIDTH;
    }

    return maxw;
}

static int _cargo_set_group_context(cargo_t ctx,
        const char *group, cargo_group_t *groups, size_t group_count, void *user)
{
    cargo_group_t *grp = NULL;
    assert(ctx);

    // If a NULL group is given, we interpret that as the default "" group.
    if (!group)
    {
        group = "";
    }

    grp = _cargo_find_group(ctx, groups, group_count, group, NULL);

    if (!grp)
    {
        CARGODBG(1, "No such group \"%s\"\n", group);
        return -1;
    }

    grp->user = user;

    return 0;
}

static void * _cargo_get_group_context(cargo_t ctx, const char *group,
                cargo_group_t *groups, size_t group_count)
{
    cargo_group_t *grp = NULL;
    assert(ctx);

    grp = _cargo_find_group(ctx, groups, group_count, group, NULL);

    if (!grp)
    {
        CARGODBG(1, "No such group \"%s\"\n", group);
        return NULL;
    }

    return grp->user;
}

static const char *_cargo_option_get_group(cargo_t ctx, const char *opt,
                        cargo_group_t *groups, size_t group_count)
{
    cargo_opt_t *o = NULL;
    size_t opt_i;
    assert(ctx);
    assert(opt);

    if (_cargo_find_option_name(ctx, opt, &opt_i, NULL) < 0)
    {
        CARGODBG(1, "No such option \"%s\"\n", opt);
        return NULL;
    }

    o = &ctx->options[opt_i];

    if (o->group_index < 0)
    {
        CARGODBG(1, "%s: Group is not set\n", opt);
        return NULL;
    }

    CARGODBG(3, "Opt idx: %lu, Group idx: %d, Group name: \"%s\"\n",
            opt_i, o->group_index, groups[o->group_index].title);

    return groups[o->group_index].name;
}

static void _cargo_mutex_group_short_usage(cargo_t ctx,
                        cargo_astr_t *str, size_t indent)
{
    int ret = 0;
    size_t i;
    size_t j;
    size_t prev_offset = 0;
    cargo_group_t *mgrp = NULL;
    cargo_opt_t *opt = NULL;
    cargo_astr_t opt_str;
    char *opt_s = NULL;
    assert(ctx);
    assert(str);

    for (i = 0; i < ctx->mutex_group_count; i++)
    {
        mgrp = &ctx->mutex_groups[i];

        if (mgrp->flags & (CARGO_MUTEXGRP_ORDER_BEFORE | CARGO_MUTEXGRP_ORDER_AFTER))
        {
            continue;
        }

        // Given by the user to override.
        if (mgrp->metavar)
        {
            memset(&opt_str, 0, sizeof(opt_str));
            opt_str.s = &opt_s;
            cargo_aappendf(&opt_str, " %s", mgrp->metavar);

            _cargo_fit_on_short_usage_line(ctx, str,
                indent, str->offset, opt_str.offset, &prev_offset);

            if (opt_s)
            {
                cargo_aappendf(str, "%s", opt_s);
                _cargo_xfree(&opt_s);
            }

            continue;
        }

        // If we should group the options in the mutex group like this
        // {opt1, opt2, opt3} or [opt1, opt2, opt3].
        if ((mgrp->flags & CARGO_MUTEXGRP_NO_GROUP_SHORT_USAGE))
        {
            continue;
        }

        // Instead of printing the {} before and after loop we include it
        // in the string inside the loop together with the option name
        // so that we always get a nice linebreak like this:
        //   {opt1, opt2}
        // compared to:
        //   {
        //   opt1, opt2}

        for (j = 0; j < mgrp->opt_count; j++)
        {
            int is_first = (j == 0);
            int is_last = (j == (mgrp->opt_count - 1));
            char is_req = (mgrp->flags & CARGO_MUTEXGRP_ONE_REQUIRED);

            memset(&opt_str, 0, sizeof(opt_str));
            opt_str.s = &opt_s;

            opt = &ctx->options[mgrp->option_indices[j]];

            if (is_first) cargo_aappendf(&opt_str, "%s", (is_req ? " {" : " ["));

            if ((ret = _cargo_get_short_option_usage(ctx, opt,
                            &opt_str, opt->positional, 0)) < 0)
            {
                CARGODBG(1, "Failed to get option usage\n");
                return;
            }

            if (!is_last) cargo_aappendf(&opt_str, ", ");
            if (is_last) cargo_aappendf(&opt_str, "%s", (is_req ? "}" : "]"));

            _cargo_fit_on_short_usage_line(ctx, str,
                indent, str->offset, opt_str.offset, &prev_offset);

            if (opt_s)
            {
                cargo_aappendf(str, "%s", opt_s);
                _cargo_xfree(&opt_s);
            }
        }
    }
}

static const char *_cargo_strip_path_from_progname(const char *org_progname)
{
    const char *progname;

    // Strip any directory path from the program name.
    if (!(progname = strrchr(org_progname, '/')))
    {
        if (!(progname = strrchr(org_progname, '\\')))
        {
            progname = org_progname;
        }
    }

    progname += strspn(progname, "/\\");

    return progname;
}

static const char *_cargo_get_short_usage(cargo_t ctx, cargo_usage_t flags)
{
    char *b = NULL;
    size_t indent = 0;
    cargo_astr_t str;
    const char *progname;
    assert(ctx);

    _cargo_add_help_if_missing(ctx);
    _cargo_add_orphans_to_default_group(ctx);

    memset(&str, 0, sizeof(str));
    str.s = &b;

    if ((flags & CARGO_USAGE_NO_STRIP_PROGNAME))
    {
        progname = ctx->progname;
    }
    else
    {
        progname = _cargo_strip_path_from_progname(ctx->progname);
    }

    cargo_aappendf(&str, "Usage: %s", progname);

    if (!(flags & CARGO_USAGE_OVERRIDE_SHORT))
    {
        indent = str.offset;

        // Options.
        _cargo_get_short_option_usages(ctx, &str, indent, 0);

        _cargo_mutex_group_short_usage(ctx, &str, indent);

        // Positional arguments at the end.
        _cargo_get_short_option_usages(ctx, &str, indent, 1);
    }

    // Reallocate the memory used for the string so it's too big.
    if (!(b = _cargo_realloc(b, str.offset + 1)))
    {
        CARGODBG(1, "Out of memory!\n");
        return NULL;
    }

    // We are always responsible to free this.
    _cargo_xfree(&ctx->short_usage);

    ctx->short_usage = b;

    return b;
}

char **_cargo_copy_string_list(char **strs, size_t count, size_t *target_count)
{
    char **ret = NULL;
    size_t i;

    if (target_count) *target_count = 0;

    if (!strs)
    {
        return NULL;
    }

    if (!(ret = _cargo_calloc(count, sizeof(char *))))
    {
        CARGODBG(1, "Out of memory!\n");
        return NULL;
    }

    if (target_count) *target_count = count;

    for (i = 0; i < count; i++)
    {
        if (!(ret[i] = _cargo_strdup(strs[i])))
        {
            CARGODBG(1, "Out of memory\n");
            goto fail;
        }
    }

    return ret;
fail:
    if (ret)
    {
        count = i;
        for (i = 0; i < count; i++)
        {
            _cargo_free(ret[i]);
        }

        _cargo_free(ret);
    }

    if (target_count) *target_count = 0;

    return NULL;
}

// -----------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------

void cargo_set_internal_usage_flags(cargo_t ctx, cargo_usage_t flags)
{
    assert(ctx);
    ctx->usage_flags = flags;
}

int cargo_get_width(cargo_t ctx, cargo_width_flags_t flags)
{
    int ret = 0;
    assert(ctx);

    ret = (int)ctx->max_width;

    if (flags & CARGO_WIDTH_RAW)
    {
        ret = _cargo_get_console_width();
    }

    return ret;
}

void cargo_set_max_width(cargo_t ctx, size_t max_width)
{
    assert(ctx);
    ctx->max_width = _cargo_process_max_width(max_width);
    CARGODBG(2, "Usage max width: %lu\n", ctx->max_width);
}

int cargo_init(cargo_t *ctx, cargo_flags_t flags, const char *progname_fmt, ...)
{
    va_list ap;
    cargo_s *c;
    assert(ctx);

    *ctx = (cargo_s *)_cargo_calloc(1, sizeof(cargo_s));
    c = *ctx;

    if (!c)
        return -1;

    c->max_opts = CARGO_DEFAULT_MAX_OPTS;
    c->flags = flags;
    c->prefix = CARGO_DEFAULT_PREFIX;
    cargo_set_max_width(c, CARGO_AUTO_MAX_WIDTH);

    va_start(ap, progname_fmt);
    cargo_vasprintf(&c->progname, progname_fmt, ap);
    va_end(ap);

    // By default we show only short usage on errors.
    c->usage_flags = CARGO_USAGE_SHORT;

    // Add the default group.
    cargo_add_group(c, 0, "", "", "");

    _cargo_add_help_if_missing(c);

    return 0;
}

void cargo_destroy(cargo_t *ctx)
{
    size_t i;

    CARGODBG(2, "cargo_destroy: DESTROY!\n");

    if (ctx)
    {
        cargo_opt_t *opt;
        cargo_t c = *ctx;

        if (c->flags & CARGO_AUTOCLEAN)
        {
            CARGODBG(2, "Auto clean target values\n");
            _cargo_cleanup_option_values(c, 1);
        }

        if (c->options)
        {
            CARGODBG(2, "DESTROY %lu options!\n", c->opt_count);

            for (i = 0; i < c->opt_count; i++)
            {
                opt = &c->options[i];
                CARGODBG(2, "Free opt: %s\n", opt->name[0]);
                _cargo_option_destroy(opt);
            }

            _cargo_xfree(&c->options);
        }

        _cargo_groups_destroy(c);

        _cargo_free_str_list(&c->args, NULL);
        _cargo_free_str_list(&c->unknown_opts, NULL);

        _cargo_xfree(&c->unknown_opts_idxs);
        _cargo_xfree(&c->error);
        _cargo_xfree(&c->short_usage);
        _cargo_xfree(&c->usage);
        _cargo_xfree(&c->description);
        _cargo_xfree(&c->epilog);
        _cargo_xfree(&c->progname);

        _cargo_free(*ctx);
        ctx = NULL;
    }
}

void cargo_set_flags(cargo_t ctx, cargo_flags_t flags)
{
    assert(ctx);
    ctx->flags = flags;
}

cargo_flags_t cargo_get_flags(cargo_t ctx)
{
    assert(ctx);
    return ctx->flags;
}

void cargo_set_prefix(cargo_t ctx, const char *prefix_chars)
{
    assert(ctx);
    ctx->prefix = prefix_chars;
}

void cargo_set_prognamev(cargo_t ctx, const char *fmt, va_list ap)
{
    assert(ctx);
    _cargo_xfree(&ctx->progname);
    cargo_vasprintf(&ctx->progname, fmt, ap);
}

void cargo_set_progname(cargo_t ctx, const char *fmt, ...)
{
    va_list ap;
    assert(ctx);
    va_start(ap, fmt);
    cargo_set_prognamev(ctx, fmt, ap);
    va_end(ap);
}

void cargo_set_descriptionv(cargo_t ctx, const char *fmt, va_list ap)
{
    assert(ctx);
    _cargo_xfree(&ctx->description);
    cargo_vasprintf(&ctx->description, fmt, ap);
}

void cargo_set_description(cargo_t ctx, const char *fmt, ...)
{
    va_list ap;
    assert(ctx);
    va_start(ap, fmt);
    cargo_set_descriptionv(ctx, fmt, ap);
    va_end(ap);
}

void cargo_set_epilogv(cargo_t ctx, const char *fmt, va_list ap)
{
    assert(ctx);
    _cargo_xfree(&ctx->epilog);
    cargo_vasprintf(&ctx->epilog, fmt, ap);
}

void cargo_set_epilog(cargo_t ctx, const char *fmt, ...)
{
    va_list ap;
    assert(ctx);
    va_start(ap, fmt);
    cargo_set_epilogv(ctx, fmt, ap);
    va_end(ap);
}

typedef struct cargo_phighlight_s
{
    int i;              // Index of highlight in argv.
    char *c;            // Highlight character (followed by color).
    int indent;         // Indent position for highlight in relation
                        // to previous highlight.
    int total_indent;   // Total indentation since start of string.
    int highlight_len;  // Length of the highlight.
    int show;
} cargo_phighlight_t;

static int _cargo_compare_highlights(const void *a, const void *b)
{
    cargo_phighlight_t *ha = (cargo_phighlight_t *)a;
    cargo_phighlight_t *hb = (cargo_phighlight_t *)b;
    return (ha->i - hb->i);
}

char *cargo_get_fprintl_args(int argc, char **argv, int start,
                            cargo_fprint_flags_t flags,
                            size_t max_width,
                            size_t highlight_count,
                            const cargo_highlight_t *highlights_in)
{
    char *ret = NULL;
    int i;
    int j;
    int global_indent = 0;
    size_t arglen = 0;
    cargo_str_t str;
    char *out = NULL;
    size_t out_size = 0;
    cargo_phighlight_t *highlights = NULL;
    assert(highlights_in);

    max_width = _cargo_process_max_width(max_width);

    if (!(highlights = _cargo_calloc(highlight_count, sizeof(cargo_phighlight_t))))
    {
        CARGODBG(1, "Out of memory!\n");
        return NULL;
    }

    for (i = 0; i < (int)highlight_count; i++)
    {
        highlights[i].i = highlights_in[i].i;
        highlights[i].c = highlights_in[i].c;
        CARGODBG(6, "  Highlight %d: %d\n", i, highlights[i].i);
    }

    CARGODBG(6, "  Sort highlights:\n");
    qsort(highlights, highlight_count,
        sizeof(cargo_phighlight_t), _cargo_compare_highlights);

    #if (CARGO_DEBUG >= 6)
    for (i = 0; i < (int)highlight_count; i++)
    {
        CARGODBG(6, "  Highlight %d: %d\n", i, highlights[i].i);
    }
    #endif

    // Get buffer size and highlight data.
    for (i = 0, j = 0; i < argc; i++)
    {
        arglen = (i >= start) ? strlen(argv[i]) : 0;

        if (arglen + global_indent >= max_width)
        {
            break;
        }

        if (j < (int)highlight_count)
        {
            cargo_phighlight_t *h = &highlights[j];
            if (h->i == i)
            {
                // We only keep this flag so that we can make sure we don't
                // output the color codes when an index is out of range.
                if (i >= start)
                {
                    h->show = 1;
                }

                h->highlight_len = (int)arglen;
                h->total_indent = global_indent;

                // We want to indent in relation to the previous indentation.
                if (j > 0)
                {
                    cargo_phighlight_t *hprev = &highlights[j - 1];
                    h->indent = h->total_indent
                              - (hprev->total_indent + hprev->highlight_len);
                }
                else
                {
                    h->indent = h->total_indent;
                }

                // If we use color, we must include the ANSI color code length
                // in the buffer length as well.
                out_size += strlen(h->c);

                j++;
            }
        }

        global_indent += arglen;
        if (i >= start) global_indent++; // + 1 for space.
        out_size += global_indent;
    }

    // Allocate and fill buffer.
    out_size += 2; // New lines.
    out_size *= 2; // Two rows, one for args and one for highlighting.

    if (!(out = _cargo_malloc(out_size)))
    {
        CARGODBG(1, "Out of memory!\n");
        goto fail;
    }

    str.s = out;
    str.l = out_size;
    str.offset = 0;

    // TODO: Try adding an "..." and try to fit all highlights on screen if possible.
    if (!(flags & CARGO_FPRINT_NOARGS))
    {
        for (i = start; i < argc; i++)
        {
            // The highlight will be incorrect if we allow a line break.
            if (str.offset + strlen(argv[i]) >= max_width)
            {
                break;
            }

            cargo_appendf(&str, "%s ", argv[i]);
        }

        cargo_appendf(&str, "\n");
    }

    if (!(flags & CARGO_FPRINT_NOHIGHLIGHT))
    {
        for (i = 0; i < (int)highlight_count; i++)
        {
            cargo_phighlight_t *h = &highlights[i];
            char *highlvec;
            int has_color = strlen(h->c) > 1;

            if (h->highlight_len == 0)
                continue;

            if (!(highlvec = _cargo_malloc(h->highlight_len)))
            {
                CARGODBG(1, "Out of memory!\n");
                goto fail;
            }

            // Use the first character as the highlight character.
            //                                ~~~~~~~~~
            memset(highlvec, *h->c, h->highlight_len);

            // If we have more characters, we append that as a string.
            // (This can be used for color ansi color codes).
            if (!(flags & CARGO_FPRINT_NOCOLOR) && has_color && h->show)
            {
                cargo_appendf(&str, "%s", &h->c[1]);
            }

            cargo_appendf(&str, "%*s%*.*s",
                h->indent, "",
                h->highlight_len,
                h->highlight_len,
                highlvec);

            if (!(flags & CARGO_FPRINT_NOCOLOR) && has_color && h->show)
            {
                cargo_appendf(&str, "%s", CARGO_COLOR_RESET);
            }

            _cargo_free(highlvec);
        }
    }

    ret = out;

fail:
    if (!ret) _cargo_free(out);
    _cargo_free(highlights);

    return ret;
}

char *cargo_get_vfprint_args(int argc, char **argv, int start,
                            cargo_fprint_flags_t flags,
                            size_t max_width,
                            size_t highlight_count, va_list ap)
{
    char *ret = NULL;
    int i;
    cargo_highlight_t *highlights = NULL;

    // Create a list of indices to highlight from the va_args.
    if (!(highlights = _cargo_calloc(highlight_count, sizeof(cargo_highlight_t))))
    {
        CARGODBG(1, "Out of memory trying to allocate %lu highlights!\n",
                highlight_count);
        goto fail;
    }

    for (i = 0; i < (int)highlight_count; i++)
    {
        highlights[i].i = va_arg(ap, int);
        highlights[i].c = va_arg(ap, char *);
    }

    ret = cargo_get_fprintl_args(argc, argv, start, flags, max_width,
                                highlight_count, highlights);

fail:
    _cargo_xfree(&highlights);

    return ret;
}

char *cargo_get_fprint_args(int argc, char **argv, int start,
                            cargo_fprint_flags_t flags,
                            size_t max_width,
                            size_t highlight_count, ...)
{
    char *ret;
    va_list ap;
    va_start(ap, highlight_count);
    ret = cargo_get_vfprint_args(argc, argv, start, flags,
                                max_width, highlight_count, ap);
    va_end(ap);
    return ret;
}

int cargo_fprint_args(FILE *f, int argc, char **argv, int start,
                      cargo_fprint_flags_t flags, size_t max_width,
                      size_t highlight_count, ...)
{
    char *ret;
    va_list ap;
    va_start(ap, highlight_count);
    ret = cargo_get_vfprint_args(argc, argv, start, flags,
                                max_width, highlight_count, ap);
    va_end(ap);

    if (!ret)
    {
        return -1;
    }

    fprintf(f, "%s\n", ret);
    _cargo_free(ret);
    return 0;
}

int cargo_fprintl_args(FILE *f, int argc, char **argv, int start,
                       cargo_fprint_flags_t flags,
                       size_t max_width, size_t highlight_count,
                       const cargo_highlight_t *highlights)
{
    char *ret;
    if (!(ret = cargo_get_fprintl_args(argc, argv, start, flags,
                                max_width, highlight_count, highlights)))
    {
        return -1;
    }

    fprintf(f, "%s\n", ret);
    _cargo_free(ret);
    return 0;
}

int cargo_parse(cargo_t ctx, cargo_flags_t flags, int start_index, int argc, char **argv)
{
    int ret = CARGO_PARSE_OK;
    int start = 0;
    int opt_arg_count = 0;
    char *arg = NULL;
    const char *name = NULL;
    cargo_opt_t *opt = NULL;
    cargo_flags_t global_flags = ctx->flags;

    // Override if any flags are set.
    if (flags)
    {
        ctx->flags = flags;
    }

    CARGODBG(2, "============ Cargo Parse =============\n");

    ctx->argc = argc;
    ctx->argv = argv;
    ctx->start = start_index;
    ctx->stopped = 0;
    ctx->stopped_hard = 0;

    _cargo_set_error(ctx, NULL);

    _cargo_add_help_if_missing(ctx);
    _cargo_add_orphans_to_default_group(ctx);

    _cargo_free_str_list(&ctx->args, NULL);
    ctx->arg_count = 0;

    _cargo_free_str_list(&ctx->unknown_opts, NULL);
    _cargo_xfree(&ctx->unknown_opts_idxs);
    ctx->unknown_opts_count = 0;

    // Make sure we start over, if this function is
    // called more than once.
    // (But we don't free the values since we don't want to
    //  overwrite default or already parsed values)
    _cargo_cleanup_option_values(ctx, 0);

    if (!(ctx->args = (char **)_cargo_calloc(argc, sizeof(char *))))
    {
        CARGODBG(1, "Out of memory!\n");
        ret = CARGO_PARSE_NOMEM; goto fail;
    }

    if (!(ctx->unknown_opts = (char **)_cargo_calloc(argc, sizeof(char *))))
    {
        CARGODBG(1, "Out of memory!\n");
        ret = CARGO_PARSE_NOMEM; goto fail;
    }

    if (!(ctx->unknown_opts_idxs = _cargo_calloc(argc, sizeof(int))))
    {
        CARGODBG(1, "Out of memory");
        ret = CARGO_PARSE_NOMEM; goto fail;
    }

    CARGODBG(2, "Parse arg list of count %d start at index %d\n", argc, start_index);

    // Check for unknown options early.
    if ((ctx->flags & CARGO_UNKNOWN_EARLY)
        && _cargo_check_unknown_options(ctx))
    {
        ret = CARGO_PARSE_UNKNOWN_OPTS; goto fail;
    }

    for (ctx->i = ctx->start; ctx->i < ctx->argc; )
    {
        arg = argv[ctx->i];
        start = ctx->i;
        opt_arg_count = 0;

        CARGODBG(3, "\n");
        CARGODBG(3, "argv[%d] = %s\n", ctx->i, arg);
        CARGODBG(3, "  Look for opt matching %s:\n", arg);

        // TODO: Add support for a "--" option which forces all
        // options specified after it to be parsed as positional arguments
        // and not options. Say there's a file named "-thefile".

        // TODO: Add support for abbreviated prefix matching so that
        // --ar will match --arne unless it's ambigous with some other option.
        if (!ctx->stopped && (name = _cargo_check_options(ctx, &opt, arg)))
        {
            // We found an option, parse any arguments it might have.
            if ((opt_arg_count = _cargo_parse_option(ctx, opt, name,
                                                    argc, argv)) < 0)
            {
                CARGODBG(1, "Failed to parse %s option: %s\n",
                        _cargo_type_to_str(opt->type), name);
                ret = opt_arg_count; goto fail;
            }
        }
        else
        {
            size_t opt_i = 0;
            CARGODBG(2, "    Positional argument: %s\n", argv[ctx->i]);

            // Positional argument.
            if (!ctx->stopped && (_cargo_get_positional(ctx, &opt_i) == 0))
            {
                opt = &ctx->options[opt_i];
                if ((opt_arg_count = _cargo_parse_option(ctx, opt,
                                                    opt->name[0],
                                                    argc, argv)) < 0)
                {
                    CARGODBG(1, "    Failed to parse %s option: %s\n",
                            _cargo_type_to_str(opt->type), name);
                    ret = opt_arg_count; goto fail;
                }
            }
            else
            {
                CARGODBG(2, "    Extra argument: %s\n", argv[ctx->i]);
                ctx->args[ctx->arg_count] = argv[ctx->i];
                ctx->arg_count++;
                opt_arg_count = 1;
            }
        }

        ctx->i += opt_arg_count;

        #if CARGO_DEBUG
        {
            int k = 0;
            int ate = opt_arg_count;

            CARGODBG(2, "    Ate %d args: ", opt_arg_count);

            for (k = start; k < (start + opt_arg_count); k++)
            {
                CARGODBGI(2, "\"%s\" ", argv[k]);
            }

            CARGODBGI(2, "%s", "\n");
        }
        #endif // CARGO_DEBUG
    }

    // Print automatic help.
    if (ctx->help)
    {
        cargo_print_usage(ctx, 0);
        ctx->flags = global_flags;
        return CARGO_PARSE_SHOW_HELP;
    }

    // An option can cause a "hard stop", meaning that there
    // won't be any errors for mutex groups and so on. This is
    // useful for cases where you have --advanced_help or similar
    // where you just want to show some extended help without also
    // having to specify required arguments.
    if (ctx->stopped_hard)
    {
        goto skip_checks;
    }

    if (_cargo_check_required_options(ctx))
    {
        ret = CARGO_PARSE_MISS_REQUIRED; goto fail;
    }

    if ((ret = _cargo_check_mutex_groups(ctx)))
    {
        goto fail;
    }

    if ((ret = _cargo_check_unknown_options_after(ctx)))
    {
        goto fail;
    }

    // Shows warnings.
    if (!(ctx->flags & CARGO_NOWARN))
    {
        _cargo_parse_show_error(ctx);
    }

skip_checks:
    ctx->flags = global_flags;
    return CARGO_PARSE_OK;

fail:
    // Let unknown options override other errors.
    // But don't check for them more than once.
    if (ctx->unknown_opts_count == 0)
    {
        int unknown_ret = 0;
        if ((unknown_ret = _cargo_check_unknown_options_after(ctx)))
        {
            CARGODBG(1, "Unknown option overrides previous error\n");
            ret = unknown_ret;
        }
    }

    _cargo_parse_show_error(ctx);
    _cargo_cleanup_option_values(ctx, 1);
    ctx->flags = global_flags;
    return ret;
}

void cargo_set_errorv(cargo_t ctx, cargo_err_flags_t flags,
                    const char *fmt, va_list ap)
{
    int ret = 0;
    char *error = NULL;
    char *error2 = NULL;
    assert(ctx);

    ret = cargo_vasprintf(&error, fmt, ap);

    if (ret >= 0)
    {
        // Maybe this can be done a bit nicer...
        if (ctx->error && (flags & CARGO_ERR_APPEND))
        {
            ret = cargo_asprintf(&error2, "%s%s", ctx->error, error);

            if (ret >= 0)
            {
                _cargo_free(error);
                error = error2;
            }
        }

        _cargo_xfree(&ctx->error);
        ctx->error = error;
    }
}

void cargo_set_error(cargo_t ctx,
                    cargo_err_flags_t flags, const char *fmt, ...)
{
    va_list ap;
    assert(ctx);
    va_start(ap, fmt);
    cargo_set_errorv(ctx, flags, fmt, ap);
    va_end(ap);
}

const char *cargo_get_error(cargo_t ctx)
{
    assert(ctx);
    return ctx->error;
}

const char **cargo_get_unknown(cargo_t ctx, size_t *unknown_count)
{
    assert(ctx);

    if (unknown_count)
    {
        *unknown_count = ctx->unknown_opts_count;
    }

    return (const char **)ctx->unknown_opts;
}

char **cargo_get_unknown_copy(cargo_t ctx, size_t *unknown_count)
{
    assert(ctx);
    return _cargo_copy_string_list(ctx->unknown_opts,
            ctx->unknown_opts_count, unknown_count);
}

const char **cargo_get_args(cargo_t ctx, size_t *argc)
{
    assert(ctx);

    if (argc)
    {
        *argc = ctx->arg_count;
    }

    return (const char **)ctx->args;
}

char **cargo_get_args_copy(cargo_t ctx, size_t *argc)
{
    assert(ctx);
    return _cargo_copy_string_list(ctx->args, ctx->arg_count, argc);
}

int cargo_add_alias(cargo_t ctx, const char *optname, const char *alias)
{
    size_t opt_i;
    size_t name_i;
    cargo_opt_t *opt;
    assert(ctx);

    if (!_cargo_find_option_name(ctx, alias, &opt_i, &name_i))
    {
        CARGODBG(1, "Alias %s already used by option %s. Cannot add to %s.\n",
                alias, ctx->options[opt_i].name[0], optname);
        return -1;
    }

    if (_cargo_find_option_name(ctx, optname, &opt_i, &name_i))
    {
        CARGODBG(1, "Failed to add alias %s to %s, option not found.\n",
                alias, optname);
        return -1;
    }

    CARGODBG(2, "Found option \"%s\"\n", optname);

    opt = &ctx->options[opt_i];

    if (opt->positional)
    {
        CARGODBG(1, "Cannot add alias for positional argument\n");
        return -1;
    }
    else
    {
        if (!_cargo_starts_with_prefix(ctx, alias))
        {
            CARGODBG(1, "Cannot add a positional as an alias to an option. "
                        "The alias must be prefixed with %s: \"%s\"\n",
                        (strlen(ctx->prefix) > 1)
                        ? "one of these characters" : "this character",
                        ctx->prefix);
            return -1;
        }
    }

    if (opt->name_count >= CARGO_NAME_COUNT)
    {
        CARGODBG(1, "Too many aliases for option: %s\n", opt->name[0]);
        return -1;
    }

    if (!(opt->name[opt->name_count] = _cargo_strdup(alias)))
    {
        CARGODBG(1, "Out of memory\n");
        return -1;
    }

    opt->name_count++;

    CARGODBG(2, "  Added alias \"%s\"\n", alias);

    return 0;
}

int cargo_set_option_descriptionv(cargo_t ctx,
                                  const char *optname,
                                  const char *fmt, va_list ap)
{
    int ret = 0;
    size_t opt_i = 0;
    size_t name_i = 0;
    cargo_opt_t *opt = NULL;
    assert(ctx);

    if (_cargo_find_option_name(ctx, optname, &opt_i, &name_i))
    {
        CARGODBG(1, "Failed to find option \"%s\"\n", optname);
        return -1;
    }

    opt = &ctx->options[opt_i];

    _cargo_xfree(&opt->description);

    ret = cargo_vasprintf(&opt->description, fmt, ap);
    return (ret >= 0) ? 0 : -1;
}

int cargo_set_option_description(cargo_t ctx,
                                 const char *optname,
                                 const char *fmt, ...)
{
    int ret = 0;
    va_list ap;
    assert(ctx);
    va_start(ap, fmt);
    ret = cargo_set_option_descriptionv(ctx, optname, fmt, ap);
    va_end(ap);
    return ret;
}

int cargo_set_metavarv(cargo_t ctx,
                    const char *optname,
                    const char *fmt, va_list ap)
{
    int ret = 0;
    size_t opt_i;
    size_t name_i;
    cargo_opt_t *opt;
    assert(ctx);

    if (_cargo_find_option_name(ctx, optname, &opt_i, &name_i))
    {
        CARGODBG(1, "Failed to find option \"%s\"\n", optname);
        return -1;
    }

    opt = &ctx->options[opt_i];

    _cargo_xfree(&opt->metavar);

    ret = cargo_vasprintf(&opt->metavar, fmt, ap);
    return (ret >= 0) ? 0 : -1;
}

int cargo_set_metavar(cargo_t ctx,
                    const char *optname,
                    const char *fmt, ...)
{
    int ret = 0;
    va_list ap;
    assert(ctx);
    va_start(ap, fmt);
    ret = cargo_set_metavarv(ctx, optname, fmt, ap);
    va_end(ap);
    return ret;
}

int cargo_mutex_group_set_metavarv(cargo_t ctx,
                                   const char *mutex_group,
                                   const char *fmt, va_list ap)
{
    int ret = 0;
    cargo_group_t *g = NULL;
    assert(ctx);
    assert(mutex_group);

    if (!(g =_cargo_find_group(ctx,
        ctx->mutex_groups, ctx->mutex_group_count, mutex_group, NULL)))
    {
        CARGODBG(1, "No such mutex group \"%s\"\n", mutex_group);
        return -1;
    }

    _cargo_xfree(&g->metavar);

    ret = cargo_vasprintf(&g->metavar, fmt, ap);

    return (ret >= 0) ? 0 : -1;
}

int cargo_mutex_group_set_metavar(cargo_t ctx,
                                  const char *mutex_group,
                                  const char *fmt, ...)
{
    int ret = 0;
    va_list ap;
    assert(ctx);
    va_start(ap, fmt);
    ret = cargo_mutex_group_set_metavarv(ctx, mutex_group, fmt, ap);
    va_end(ap);
    return ret;
}

const char *cargo_get_usage(cargo_t ctx, cargo_usage_t flags)
{
    char *ret = NULL;
    size_t i;
    char *b = NULL;
    int max_name_len = 0;
    size_t positional_count = 0;
    size_t option_count = 0;
    const char *short_usage = NULL;
    cargo_group_t *grp = NULL;
    cargo_astr_t str;
    int is_default_group = 1;
    assert(ctx);

    if (!(flags & CARGO_USAGE_HIDE_SHORT))
    {
        if (!(short_usage = _cargo_get_short_usage(ctx, flags)))
        {
            CARGODBG(1, "Failed to get short usage\n");
            return NULL;
        }
    }

    // Only show short usage.
    if (flags & CARGO_USAGE_SHORT)
    {
        return short_usage;
    }

    // TODO: Instead of looping over all options at this stage, save the length
    // of all options on add_option, including max_length, positional_count and
    // so on.

    // First get option names and their length.
    // We get the widest one so we know the column width to use
    // for the final result.
    //   --option_a         Some description.
    //   --longer_option_b  Another description...
    // ^-------------------^
    // What should the above width be.
    if ((max_name_len = _cargo_get_max_name_length(ctx,
                        &positional_count, &option_count)) < 0)
    {
        CARGODBG(1, "Failed to get option name max length\n");
        goto fail;
    }

    str.s = &b;
    str.l = 1024;
    str.offset = 0;

    // TODO: Break all this into separate functions.

    if (short_usage && !(flags & CARGO_USAGE_HIDE_SHORT))
    {
        cargo_aappendf(&str, "%s\n", short_usage);
    }

    if(ctx->description && strlen(ctx->description)
       && !(flags & CARGO_USAGE_HIDE_DESCRIPTION))
    {
        if (flags & CARGO_USAGE_RAW_DESCRIPTION)
        {
            if (cargo_aappendf(&str, "\n%s\n", ctx->description) < 0) goto fail;
        }
        else
        {
            char *lb_desc;
            if (!(lb_desc = _cargo_linebreak(ctx, ctx->description, ctx->max_width)))
            {
                goto fail;
            }
            cargo_aappendf(&str, "\n%s\n", lb_desc);
            _cargo_free(lb_desc);
        }
    }

    CARGODBG(2, "max_name_len = %d, ctx->max_width = %lu\n",
            max_name_len, ctx->max_width);

    #ifdef CARGO_DEBUG
    for (i = 0; i < ctx->group_count; i++)
    {
        CARGODBG(2, "%s: %lu\n",
            ctx->groups[i].name, ctx->groups[i].opt_count);
    }
    #endif

    // Start by printing mutually exclusive groups
    for (i = 0; i < ctx->mutex_group_count; i++)
    {
        int indent = 2;
        const char *description = NULL;
        grp = &ctx->mutex_groups[i];

        if (!(grp->flags & CARGO_MUTEXGRP_GROUP_USAGE))
        {
            continue;
        }

        if (grp->title)
        {
            cargo_aappendf(&str, "\n%s:\n", grp->title);
        }

        description = grp->description;

        if (!description)
        {
            description = "Specify one of the following.";
        }

        if (_cargo_get_group_description(ctx, &str, grp, indent))
        {
            goto fail;
        }

        // Positional.
        if (_cargo_print_options(ctx, grp->option_indices, grp->opt_count,
                                1, &str, max_name_len, indent, 1, flags))
        {
            goto fail;
        }

        // Options.
        if (_cargo_print_options(ctx, grp->option_indices, grp->opt_count,
                                0, &str, max_name_len, indent, 1, flags))
        {
            goto fail;
        }
    }

    for (i = 0; i < ctx->group_count; i++)
    {
        int indent = 2;
        grp = &ctx->groups[i];

        if ((grp->flags & CARGO_GROUP_HIDE) || (grp->opt_count == 0))
        {
            continue;
        }

        // The group name is always set for normal groups.
        // The default group is simply "".
        is_default_group = (strlen(grp->name) == 0);

        if (!is_default_group)
        {
            indent = 2;
        }

        if (!is_default_group) cargo_aappendf(&str, "\n%s:", grp->title);
        if (grp->description) cargo_aappendf(&str, "\n");

        if (_cargo_get_group_description(ctx, &str, grp, indent))
        {
            goto fail;
        }

        // Note, we only show the "Positional arguments" and "Options"
        // titles for the default group. It becomes quite spammy otherwise.
        if (positional_count > 0)
        {
            if (is_default_group)
                if (cargo_aappendf(&str, "Positional arguments:\n") < 0) goto fail;

            if (_cargo_print_options(ctx, grp->option_indices, grp->opt_count,
                                    1, &str, max_name_len, indent, 0, flags))
            {
                goto fail;
            }
        }

        if (cargo_aappendf(&str, "\n") < 0) goto fail;

        if (option_count > 0)
        {
            if (is_default_group)
                if (cargo_aappendf(&str, "Options:\n") < 0) goto fail;

            if (_cargo_print_options(ctx, grp->option_indices, grp->opt_count,
                                    0, &str, max_name_len, indent, 0, flags))
            {
                goto fail;
            }
        }
    }

    if(ctx->epilog && strlen(ctx->epilog)
       && !(flags & CARGO_USAGE_HIDE_EPILOG))
    {
        if (flags & CARGO_USAGE_RAW_EPILOG)
        {
            if (cargo_aappendf(&str, "\n%s\n", ctx->epilog) < 0) goto fail;
        }
        else
        {
            char *lb_epilog;
            if (!(lb_epilog = _cargo_linebreak(ctx, ctx->epilog, ctx->max_width)))
            {
                goto fail;
            }
            cargo_aappendf(&str, "\n%s\n", lb_epilog);
            _cargo_free(lb_epilog);
        }
    }

    // TODO: Realloc the buffer to minimize size.

    ret = b;

fail:
    // A real failure!
    if (!ret)
    {
        _cargo_xfree(&b);
    }

    // Save the usage and destroy it on exit,
    // we want the user to be able to do things like this:
    // printf("%s\nYou're bad at typing!\n", cargo_get_usage(cargo, 0));
    // without leaking memory.
    _cargo_xfree(&ctx->usage);

    ctx->usage = ret;

    return ret;
}

int cargo_get_stop_index(cargo_t ctx)
{
    assert(ctx);
    return ctx->stopped;
}

int cargo_fprint_usage(cargo_t ctx, FILE *f, cargo_usage_t flags)
{
    const char *s;
    assert(ctx);

    if (!(s = cargo_get_usage(ctx, flags)))
    {
        return -1;
    }

    fprintf(f, "%s\n", s);

    return 0;
}

int cargo_print_usage(cargo_t ctx, cargo_usage_t flags)
{
    return cargo_fprint_usage(ctx, stdout, flags);
}

int cargo_add_group(cargo_t ctx, cargo_group_flags_t flags, const char *name,
                    const char *title, const char *description, ...)
{
    char *d = NULL;
    int ret = 0;
    va_list ap;
    assert(ctx);

    if (description)
    {
        va_start(ap, description);
        cargo_vasprintf(&d, description, ap);
        va_end(ap);
    }

    ret = _cargo_add_group(ctx, &ctx->groups, &ctx->group_count,
                            &ctx->max_groups,
                            (size_t)flags, name, title, d);
    _cargo_xfree(&d);
    return ret;
}

int cargo_group_add_option(cargo_t ctx, const char *group, const char *opt)
{
    return _cargo_group_add_option_ex(ctx,
                ctx->groups, ctx->group_count, group, opt, 0);
}

int cargo_group_set_flags(cargo_t ctx, const char *group,
                           cargo_group_flags_t flags)
{
    cargo_group_t *g = NULL;
    size_t grp_i;
    assert(ctx);
    assert(group);

    if (!(g = _cargo_find_group(ctx, ctx->groups, ctx->group_count, group, &grp_i)))
    {
        CARGODBG(1, "No such group \"%s\"\n", group);
        return -1;
    }

    g->flags = flags;

    return 0;
}

int cargo_add_mutex_group(cargo_t ctx,
                        cargo_mutex_group_flags_t flags,
                        const char *name,
                        const char *title,
                        const char *description, ...)
{
    char *d = NULL;
    int ret = 0;
    va_list ap;
    assert(ctx);

    if (description)
    {
        va_start(ap, description);
        cargo_vasprintf(&d, description, ap);
        va_end(ap);
    }

    ret = _cargo_add_group(ctx, &ctx->mutex_groups, &ctx->mutex_group_count,
                            &ctx->mutex_max_groups,
                            (size_t)flags, name, title, d);
    _cargo_xfree(&d);
    return ret;
}

int cargo_mutex_group_add_option(cargo_t ctx, const char *group, const char *opt)
{
    return _cargo_group_add_option_ex(ctx,
                ctx->mutex_groups, ctx->mutex_group_count, group, opt, 1);
}

int cargo_add_optionv(cargo_t ctx, cargo_option_flags_t flags,
                         const char *optnames, const char *description,
                         const char *fmt, va_list ap)
{
    size_t optcount = 0;
    char **optname_list = NULL;
    int ret = -1;
    size_t i = 0;
    cargo_fmt_scanner_t s;
    cargo_opt_t *o = NULL;
    char *grpname = NULL;
    char *mutex_grpname = NULL;
    int nargs_is_set = 0;
    assert(ctx);

    CARGODBG(2, "-------- Add option \"%s\", \"%s\" --------\n", optnames, fmt);

    if ((
            (flags & CARGO_OPT_DEFAULT_LITERAL) ||
            (ctx->flags & CARGO_DEFAULT_LITERALS)
        )
        && !(ctx->flags & CARGO_AUTOCLEAN))
    {
        CARGODBG(1, "Option flag CARGO_OPT_DEFAULT_LITERAL or global flag "
                    "CARGO_DEFAULT_LITERALS must be "
                    "combined with the global flag CARGO_AUTOCLEAN "
                    "to avoid memory leak / crash.");
        return -1;
    }

    // TODO: Allow multiple mutex groups...
    if (!(optnames = _cargo_get_option_group_names(ctx, optnames,
                        &grpname, &mutex_grpname)))
    {
        CARGODBG(1, "Failed to parse group name\n");
        return -1;
    }

    CARGODBG(2, " Group: \"%s\", Optnames: \"%s\"\n", grpname, optnames);

    if (!(optname_list = _cargo_split_and_verify_option_names(ctx, optnames, &optcount)))
    {
        CARGODBG(1, "Failed to split option names \"%s\"\n", optnames);
        goto fail;
    }

    if (!(o = _cargo_option_init(ctx, optname_list[0], description)))
    {
        CARGODBG(1, "Failed to init option\n");
        goto fail;
    }

    if (grpname)
    {
        CARGODBG(2, "Add \"%s\" to group \"%s\"\n", o->name[0], grpname);

        if (cargo_group_add_option(ctx, grpname, o->name[0]))
        {
            CARGODBG(1, "Failed to add option \"%s\" to group \"%s\"\n",
                    o->name[0], grpname);
            goto fail;
        }
    }
    else
    {
        CARGODBG(2, "Add \"%s\" to default group\n", o->name[0]);
        o->group_index = -1;
    }

    if (mutex_grpname)
    {
        CARGODBG(2, "Add \"%s\" to mutex group \"%s\"\n", o->name[0], mutex_grpname);

        if (cargo_mutex_group_add_option(ctx, mutex_grpname, o->name[0]))
        {
            CARGODBG(1, "Failed to add option \"%s\" to mutex group \"%s\"",
                    o->name[0], mutex_grpname);
            goto fail;
        }
    }

    // Start parsing the format string.
    _cargo_fmt_scanner_init(&s, fmt);
    _cargo_fmt_next_token(&s);

    // Get the first token.
    if (_cargo_fmt_token(&s) == '.')
    {
        CARGODBG(2, "Static\n");
        o->alloc = 0;
        _cargo_fmt_next_token(&s);
    }
    else
    {
        o->alloc = 1;
    }

    if (_cargo_fmt_token(&s) == '[')
    {
        CARGODBG(4, "   [\n");
        o->array = 1;
        _cargo_fmt_next_token(&s);
    }

    switch (_cargo_fmt_token(&s))
    {
        //
        // !!!WARNING!!!
        // Do not attempt to refactor and put these longer cases in
        // separate functions. Passing a va_list around is dangerous.
        // This works fine on Unix, but fails randomly on Windows!
        //

        // Same as string, but the target is internal
        // and will be passed to the user specified callback.
        case 'c':
        {
            CARGODBG(4, "Custom callback\n");

            if (!o->alloc)
            {
                CARGODBG(1, "WARNING! Static '.' is ignored for a custom "
                            "callback the memory for the arguments is "
                            "allocated internally.");
                o->alloc = 1;
            }

            o->type = CARGO_STRING;
            o->custom = va_arg(ap, cargo_custom_f);
            o->custom_user = va_arg(ap, void *);

            // Internal target.
            o->target = (void **)&o->custom_target;
            o->target_count = &o->custom_target_count;

            o->lenstr = 0;

            if (!o->custom)
            {
                CARGODBG(2, "Warning: Got NULL custom callback pointer\n");
            }

            _cargo_fmt_next_token(&s);

            switch (_cargo_fmt_token(&s))
            {
                // A shortcut to "[c]#", cbfunc, NULL, NULL, 0.
                // which makes a custom callback a bool flag basically.
                case '0': o->nargs = 0; break;
                default:
                {
                    _cargo_fmt_prev_token(&s);
                    o->nargs = 1;
                    break;
                }
            }

            nargs_is_set = 1;

            break;
        }
        case 'D': // D as in Dummy.
        {
            // Shortcut for: "c0", NULL, NULL;
            // That is a dummy callback. This is mostly useful
            // for using in a mutex group.
            o->array = 0;
            o->alloc = 1;
            o->type = CARGO_STRING;
            o->custom = NULL;
            o->custom_user = NULL;
            o->lenstr = 0;
            o->nargs = 0;
            o->target = (void **)&o->custom_target;
            o->target_count = &o->custom_target_count;
            nargs_is_set = 1;
            break;
        }
        case 's':
        {
            o->type = CARGO_STRING;
            o->target = (void *)va_arg(ap, char *);

            CARGODBG(4, "Read string\n");
            _cargo_fmt_next_token(&s);

            if (_cargo_fmt_token(&s) == '#')
            {
                o->lenstr = (size_t)va_arg(ap, int);
                CARGODBG(4, "String length: %lu\n", o->lenstr);

                if (o->alloc)
                {
                    CARGODBG(1, "%s: WARNING! Usually restricting the size of a "
                        "string using # is only done on static strings.\n"
                        "    Are you sure you want this?\n",
                        o->name[0]);
                    CARGODBG(1, "      \"%s\"\n", s.start);
                    CARGODBG(1, "       %*s\n", s.column, "^");
                }
            }
            else
            {
                // String size not fixed.
                o->lenstr = 0;
                _cargo_fmt_prev_token(&s);
            }

            break;
        }
        case 'b':
        {
            o->type = CARGO_BOOL;
            o->target = va_arg(ap, void *);

            // Look for any modifier tokens.
            _cargo_fmt_next_token(&s);

            switch (_cargo_fmt_token(&s))
            {
                // Read an int that will be stored in the bool value (Default 1)
                case '=': o->bool_store = va_arg(ap, int); break;
                // Count flag occurances.
                case '!': o->bool_count = 1; break;
                case '|':
                case '+':
                case '&':
                case '_':
                {
                    switch (_cargo_fmt_token(&s))
                    {
                        default:
                        case '|': o->bool_acc_op = CARGO_BOOL_OP_OR; break;
                        case '+': o->bool_acc_op = CARGO_BOOL_OP_PLUS; break;
                        case '&': o->bool_acc_op = CARGO_BOOL_OP_AND; break;
                        case '_': o->bool_acc_op = CARGO_BOOL_OP_STORE; break;
                    }

                    o->bool_acc_count = 0;
                    o->bool_acc_max_count = (size_t)va_arg(ap, unsigned int);
                    CARGODBG(3, "Bool acc max count %lu\n", o->bool_acc_max_count);

                    if (!(o->bool_acc = _cargo_calloc(o->bool_acc_max_count, sizeof(int))))
                    {
                        CARGODBG(1, "Out of memory\n");
                        goto fail;
                    }

                    for (i = 0; i < o->bool_acc_max_count; i++)
                    {
                        o->bool_acc[i] = va_arg(ap, int);
                        CARGODBG(3, "  bool acc value %lu: 0x%x\n", i, o->bool_acc[i]);
                    }
                    break;
                }
                default:
                {
                    // Got no flag modifier token.
                    o->bool_store = 1;
                    o->bool_count = 0;
                    o->bool_acc = NULL;
                    _cargo_fmt_prev_token(&s);
                }
            }

            break;
        }
        // TODO: Maybe replace target here with a union?
        case 'i': o->type = CARGO_INT;    o->target = va_arg(ap, void *); break;
        case 'd': o->type = CARGO_DOUBLE; o->target = va_arg(ap, void *); break;
        case 'u': o->type = CARGO_UINT;   o->target = va_arg(ap, void *); break;
        case 'f': o->type = CARGO_FLOAT;  o->target = va_arg(ap, void *); break;
        case 'L': o->type = CARGO_LONGLONG;   o->target = va_arg(ap, void *); break;
        case 'U': o->type = CARGO_ULONGLONG;  o->target = va_arg(ap, void *); break;
        default: _cargo_invalid_format_char(ctx, o->name[0], fmt, &s);goto fail;
    }

    if (o->array)
    {
        // Custom callbacks uses an internal target and count.
        // However we still return the count in a separate
        // user specified value for arrays.
        if (o->custom)
        {
            o->custom_user_count = va_arg(ap, size_t *);
        }
        else
        {
            // This is where we return the number of arguments
            // we have parsed for this option.
            o->target_count = va_arg(ap, size_t *);
        }

        _cargo_fmt_next_token(&s);

        if (_cargo_fmt_token(&s) != ']')
        {
            CARGODBG(1, "%s: Expected ']'\n", o->name[0]);
            CARGODBG(1, "      \"%s\"\n", fmt);
            CARGODBG(1, "        %*s\n", s.column, "^");
            goto fail;
        }

        _cargo_fmt_next_token(&s);

        if (o->alloc)
        {
            switch (_cargo_fmt_token(&s))
            {
                case '*': o->nargs = CARGO_NARGS_ZERO_OR_MORE; break;
                case '+': o->nargs = CARGO_NARGS_ONE_OR_MORE;  break;
                case 'N': // Fall through. Python uses N so lets allow that...
                case '#': o->nargs = va_arg(ap, int); break;
                default: _cargo_invalid_format_char(ctx, o->name[0], fmt, &s);
                        goto fail;
            }

            // By default "nargs" is the max number of arguments the option should parse.
            o->max_target_count = (o->nargs >= 0) ? o->nargs : (size_t)(-1);
        }
        else
        {
            assert(*fmt == '.');
            // If we have a static array. For example:
            // "int val[4];"
            // The max target count must still always be specified
            // if we use "+" or "*" when in static mode.
            o->max_target_count = va_arg(ap, int);

            switch (_cargo_fmt_token(&s))
            {
                case '*': o->nargs = CARGO_NARGS_ZERO_OR_MORE; break;
                case '+': o->nargs = CARGO_NARGS_ONE_OR_MORE;  break;
                case 'N': // Fall through. Python uses N so lets allow that...
                case '#': o->nargs = o->max_target_count; break;
                default: _cargo_invalid_format_char(ctx, o->name[0], fmt, &s);
                        goto fail;
            }
        }
    }
    else
    {
        // Non-array.

        if (o->type == CARGO_BOOL)
        {
            // BOOLs never have arguments.
            o->nargs = 0;
        }
        else
        {
            _cargo_fmt_next_token(&s);

            if (_cargo_fmt_token(&s) == '?')
            {
                o->nargs = CARGO_NARGS_ZERO_OR_ONE;
                o->zero_or_one_default = va_arg(ap, char *);
            }
            else
            {
                _cargo_fmt_prev_token(&s);

                if (!nargs_is_set)
                {
                    o->nargs = 1;
                }
            }
        }

        // Never allocate single values (unless it's a string).
        o->alloc = (o->type != CARGO_STRING) ? 0 : o->alloc;
        o->max_target_count = 1;
    }

    _cargo_fmt_next_token(&s);

    if (_cargo_fmt_token(&s) != '\0')
    {
        _cargo_invalid_format_char(ctx, o->name[0], fmt, &s);
        CARGODBG(1, "Got garbage at end of format string\n");
        goto fail;
    }

    o->flags = flags;
    o->first_parse = 1;

    // Check if the option has a prefix
    // (if not it's positional).
    o->positional = !_cargo_is_prefix(ctx, o->name[0][0]);

    if (o->positional
        && !(o->flags & CARGO_OPT_NOT_REQUIRED)
        && (o->nargs != CARGO_NARGS_ZERO_OR_MORE)
        && (o->nargs != CARGO_NARGS_ZERO_OR_ONE))
    {
        CARGODBG(2, "Positional argument %s required by default\n", o->name[0]);
        o->flags |= CARGO_OPT_REQUIRED;
    }

    if (_cargo_validate_option_args(ctx, o))
    {
        goto fail;
    }

    // .[s]#  .[s]+  .[s]*
    if ((o->type == CARGO_STRING)
         && (o->nargs != 1)
         && (o->lenstr == 0)
         && !o->alloc)
    {
        // A list of strings with a static size is a special case:
        //   char *strs[5];
        // Since we only want to allocate memory for the individual
        // strings we parse, but not the entire list (as with):
        //   char **strs;
        // The format string for this would be ".[s]#"
        //
        // nargs != 1 && nargs != -1:
        //   So we want nargs to be set, but not to infinite (-1),
        //   this means # was used.
        // lenstr == 0:
        //   .[s#]# would mean we have something like char strs[5][15];
        // !alloc:
        //   The list is not to be allocated.

        // So in this case we are not allocating the list itself
        // since that is of a fixed size. But we want to allocate
        // each individual item string.
        o->str_alloc_items = 1;
    }

    for (i = 1; i < optcount; i++)
    {
        if (cargo_add_alias(ctx, optname_list[0], optname_list[i]))
        {
            goto fail;
        }
    }

    CARGODBG(2, " Option %s:\n", o->name[0]);
    CARGODBG(2, "   max_target_count = %lu\n", o->max_target_count);
    CARGODBG(2, "   alloc = %d\n", o->alloc);
    CARGODBG(2, "   lenstr = %lu\n", o->lenstr);
    CARGODBG(2, "   nargs = %d\n", o->nargs);
    CARGODBG(2, "   positional = %d\n", o->positional);
    CARGODBG(2, "   array = %d\n", o->array);
    CARGODBG(2, "   \n");

    ret = 0;

fail:
    if (ret < 0)
    {
        if (o)
        {
            _cargo_option_destroy(o);
            ctx->opt_count--;
        }
    }

    _cargo_xfree(&grpname);
    _cargo_xfree(&mutex_grpname);
    _cargo_free_str_list(&optname_list, &optcount);

    return ret;
}

int cargo_add_option(cargo_t ctx, cargo_option_flags_t flags,
                        const char *optnames, const char *description,
                        const char *fmt, ...)
{
    int ret;
    va_list ap;
    assert(ctx);

    va_start(ap, fmt);
    ret = cargo_add_optionv(ctx, flags, optnames, description, fmt, ap);
    va_end(ap);

    return ret;
}

cargo_validation_t *cargo_create_validator(const char *name,
                                           cargo_validation_f validator,
                                           cargo_validation_destroy_f destroy,
                                           cargo_type_t types,
                                           void *user)
{
    cargo_validation_t *v = NULL;

    if (!(v = _cargo_calloc(1, sizeof(cargo_validation_t))))
    {
        return NULL;
    }

    v->name = name;
    v->validator = validator;
    v->destroy = destroy;
    v->types = types;
    v->user = user;

    return v;
}

void *cargo_validator_get_context(cargo_validation_t *validator)
{
    assert(validator);
    return validator->user;
}


typedef union cargo_vals_s
{
    int i;
    unsigned int u;
    float f;
    double d;
    long long int ll;
    unsigned long long ull;
} cargo_vals_t;

typedef struct cargo_range_validation_s
{
    cargo_vals_t min;
    cargo_vals_t max;
    cargo_vals_t epsilon;
} cargo_range_validation_t;

static int _cargo_nearly_equal(double a, double b, double epsilon)
{
    double largest;
    double diff = cargo_fabs(a - b);
    a = (double)cargo_fabs(a);
    b = (double)cargo_fabs(b);
    largest = (b > a) ? b : a;

    if (diff <= (largest * epsilon))
        return 1;
    return 0;
}

static int _cargo_validate_range_cb(cargo_t ctx, cargo_validation_flags_t flags,
                        const char *opt, cargo_validation_t *vd, void *value)
{
    #define _CARGO_COMPARE_RANGE(_type, valfmt, _member)                    \
    do                                                                      \
    {                                                                       \
        _type i = *((_type *)value);                                        \
        if ((i < vr->min._member) || (i > vr->max._member))                 \
        {                                                                   \
            cargo_set_error(ctx, 0,                                         \
                "Value must be in the range between "                       \
                 valfmt" and "valfmt" for %s but got "valfmt"\n",           \
                vr->min._member, vr->max._member, opt, i);                  \
            return -1;                                                      \
        }                                                                   \
    } while (0)

    #define _CARGO_COMPARE_FLOAT_RANGE(_type, valfmt, _member)              \
    do                                                                      \
    {                                                                       \
        _type f = *((_type *)value);                                        \
        _type min = vr->min._member;                                        \
        _type max = vr->max._member;                                        \
        _type epsilon = vr->epsilon._member;                                \
        if (((f < min) && !_cargo_nearly_equal(f, min, epsilon))            \
         || ((f > max) && !_cargo_nearly_equal(f, max, epsilon)))           \
        {                                                                   \
            cargo_set_error(ctx, 0,                                         \
                "Value must be in the range between "                       \
                 valfmt" and "valfmt" for %s but got "valfmt"\n",       \
                vr->min._member, vr->max._member, opt, f);                  \
            return -1;                                                      \
        }                                                                   \
    } while (0)

    cargo_type_t type = cargo_get_option_type(ctx, opt);
    cargo_range_validation_t *vr = NULL;
    assert(ctx);
    assert(vd);
    assert(opt);

    vr = (cargo_range_validation_t *)cargo_validator_get_context(vd);

    switch (type)
    {
        case CARGO_INT:
            _CARGO_COMPARE_RANGE(int, "%d", i); break;
        case CARGO_UINT:
            _CARGO_COMPARE_RANGE(unsigned int, "%u", u); break;
        case CARGO_FLOAT:
            _CARGO_COMPARE_FLOAT_RANGE(float, "%0.2f", f); break;
        case CARGO_DOUBLE:
            _CARGO_COMPARE_FLOAT_RANGE(double, "%0.2f", d); break;
        case CARGO_LONGLONG:
            _CARGO_COMPARE_RANGE(long long int, "%"CARGO_LONGLONG_FMT, ll); break;
        case CARGO_ULONGLONG:
            _CARGO_COMPARE_RANGE(unsigned long long int, "%"CARGO_ULONGLONG_FMT, ull); break;
        case CARGO_BOOL:
        case CARGO_STRING: return -1;
    }

    return 0;
    #undef _CARGO_COMPARE_RANGE
    #undef _CARGO_COMPARE_FLOAT_RANGE
}

static cargo_validation_t *_cargo_create_range(const char *name,
                                               cargo_type_t type)
{
    cargo_range_validation_t *vr = NULL;
    cargo_validation_t *v = NULL;

    if (!(vr = _cargo_calloc(1, sizeof(cargo_range_validation_t))))
    {
        return NULL;
    }

    if (!(v = cargo_create_validator(name,
                                    _cargo_validate_range_cb,
                                    NULL, type, vr)))
    {
        _cargo_free(vr);
        return NULL;
    }

    return v;
}

#define _CARGO_CREATE_VALIDATE_RANGE_FUNC_BODY(type, _cargo_type, _member)          \
    cargo_range_validation_t *vr = NULL;                                            \
    cargo_validation_t *v = NULL;                                                   \
    if (!(v = _cargo_create_range(#type" range", _cargo_type)))                 \
        return NULL;                                                                \
    vr = cargo_validator_get_context(v);                                            \
    vr->min._member = min;                                                          \
    vr->max._member = max;                                                          \
    vr->epsilon._member = epsilon;                                                  \
    return v;                                                                       \

#define _CARGO_CREATE_VALIDATE_RANGE_FUNC(name, type, _cargo_type, _member)         \
    cargo_validation_t *cargo_validate_##name(type min, type max)                   \
    {                                                                               \
        type epsilon = 0;                                                           \
        _CARGO_CREATE_VALIDATE_RANGE_FUNC_BODY(type, _cargo_type, _member);         \
    }

#define _CARGO_CREATE_VALIDATE_FLOAT_RANGE_FUNC(name, type, _cargo_type, _member)   \
    cargo_validation_t *cargo_validate_##name(type min, type max, type epsilon)     \
    {                                                                               \
        _CARGO_CREATE_VALIDATE_RANGE_FUNC_BODY(type, _cargo_type, _member);         \
    }

_CARGO_CREATE_VALIDATE_RANGE_FUNC(int_range, int, CARGO_INT, i)
_CARGO_CREATE_VALIDATE_RANGE_FUNC(uint_range, unsigned int, CARGO_UINT, u)
_CARGO_CREATE_VALIDATE_RANGE_FUNC(longlong_range, long long int, CARGO_LONGLONG, ll)
_CARGO_CREATE_VALIDATE_RANGE_FUNC(ulonglong_range, unsigned long long int, CARGO_ULONGLONG, ull)
_CARGO_CREATE_VALIDATE_FLOAT_RANGE_FUNC(float_range, float, CARGO_FLOAT, f)
_CARGO_CREATE_VALIDATE_FLOAT_RANGE_FUNC(double_range, double, CARGO_DOUBLE, d)

typedef struct cargo_choices_validation_s
{
    cargo_type_t type;
    size_t count;
    char **strs;
    cargo_vals_t *nums;
    double epsilon;
    cargo_validate_choices_flags_t flags;
    char *err;
} cargo_choices_validation_t;

static void _cargo_validate_choices_destroy_cb(void *user)
{
    cargo_choices_validation_t *vc = (cargo_choices_validation_t *)user;

    _cargo_xfree(&vc->nums);
    _cargo_xfree(&vc->err);
    _cargo_free_str_list(&vc->strs, &vc->count);
}

int _cargo_validate_choices_cb(cargo_t ctx,
                            cargo_validation_flags_t flags,
                            const char *opt, cargo_validation_t *vd,
                            void *value)
{
    size_t i;
    cargo_choices_validation_t *vc = NULL;
    int case_sensitive = 0;
    assert(ctx);
    assert(vd);

    vc = (cargo_choices_validation_t *)cargo_validator_get_context(vd);
    assert(vc);

    case_sensitive = vc->flags & CARGO_VALIDATE_CHOICES_CASE_SENSITIVE;

    for (i = 0; i < vc->count; i++)
    {
        switch (vc->type)
        {
            case CARGO_STRING:
            {
                char *str = ((char *)value);
                if (case_sensitive)
                {
                    if (!strcmp(vc->strs[i], str))
                    {
                        return 0;
                    }
                }
                else
                {
                    if (!strcasecmp(vc->strs[i], str))
                    {
                        return 0;
                    }
                }
                break;
            }
            case CARGO_INT:
                if (vc->nums[i].i == *((int *)value))
                    return 0;
                break;
            case CARGO_UINT:
                if (vc->nums[i].u == *((unsigned int *)value))
                    return 0;
                break;
            case CARGO_FLOAT:
                if (_cargo_nearly_equal(vc->nums[i].f, *((float *)value),
                                        vc->epsilon))
                    return 0;
                break;
            case CARGO_DOUBLE:
                if (_cargo_nearly_equal(vc->nums[i].d, *((double *)value),
                                        vc->epsilon))
                    return 0;
                break;
            case CARGO_LONGLONG:
                if (vc->nums[i].ll == *((long long int *)value))
                    return 0;
                break;
            case CARGO_ULONGLONG:
                if (vc->nums[i].ull == *((unsigned long long int *)value))
                    return 0;
                break;
            default:
                break;
        }
    }

    cargo_set_error(ctx, 0, "The value for %s, must be one of these %s: %s",
                    opt, case_sensitive ? "(case sensitive)" : "", vc->err);

    return -1;
}

cargo_validation_t *cargo_validate_choices(cargo_validate_choices_flags_t flags,
                                            cargo_type_t type,
                                            size_t count, ...)
{
    va_list ap;
    size_t i;
    cargo_astr_t str;
    cargo_validation_t *v = NULL;
    cargo_choices_validation_t *vc = NULL;
    memset(&str, 0, sizeof(str));

    if (!(vc = _cargo_calloc(1, sizeof(cargo_choices_validation_t))))
    {
        return NULL;
    }

    if (!(v = cargo_create_validator("choices",
                _cargo_validate_choices_cb,
                _cargo_validate_choices_destroy_cb,
                (CARGO_STRING | CARGO_INT | CARGO_UINT | CARGO_FLOAT |
                 CARGO_DOUBLE | CARGO_LONGLONG | CARGO_ULONGLONG),
                vc)))
    {
        _cargo_free(vc);
        return NULL;
    }

    vc->flags = flags;
    vc->type = type;
    vc->count = count;

    if (vc->type == CARGO_STRING)
    {
        if (!(vc->strs = _cargo_calloc(vc->count, sizeof(char *))))
        {
            CARGODBG(1, "Out of memory\n");
            goto fail;
        }
    }
    else
    {
        if (!(vc->nums = _cargo_calloc(vc->count, sizeof(cargo_vals_t))))
        {
            CARGODBG(1, "Out of memory\n");
            goto fail;
        }
    }

    // Start out with room strings of 20 chars
    // to keep reallocation to a minimum.
    str.s = &vc->err;
    str.l = count * 20;

    va_start(ap, count);

    if (vc->flags & CARGO_VALIDATE_CHOICES_SET_EPSILON)
    {
        vc->epsilon = va_arg(ap, double);
    }
    else
    {
        vc->epsilon = CARGO_DEFAULT_EPSILON;
    }

    for (i = 0; i < vc->count; i++)
    {
        switch (vc->type)
        {
            case CARGO_STRING:
            {
                if (!(vc->strs[i] = _cargo_strdup(va_arg(ap, char *))))
                {
                    goto fail;
                }

                cargo_aappendf(&str, "%s", vc->strs[i]);
                break;
            }
            case CARGO_INT:
            {
                vc->nums[i].i = va_arg(ap, int);
                cargo_aappendf(&str, "%d", vc->nums[i].i);
                break;
            }
            case CARGO_UINT:
            {
                vc->nums[i].u = va_arg(ap, unsigned int);
                cargo_aappendf(&str, "%u", vc->nums[i].u);
                break;
            }
            case CARGO_FLOAT:
            {
                vc->nums[i].f = (float)va_arg(ap, double);
                cargo_aappendf(&str, "%.2f", vc->nums[i].f);
                break;
            }
            case CARGO_DOUBLE:
            {
                vc->nums[i].d = va_arg(ap, double);
                cargo_aappendf(&str, "%.2f", vc->nums[i].d);
                break;
            }
            case CARGO_LONGLONG:
            {
                vc->nums[i].ll = va_arg(ap, long long int);
                cargo_aappendf(&str, "%"CARGO_LONGLONG_FMT, vc->nums[i].ll);
                break;
            }
            case CARGO_ULONGLONG:
            {
                vc->nums[i].ull = va_arg(ap, unsigned long long int);
                cargo_aappendf(&str, "%"CARGO_ULONGLONG_FMT, vc->nums[i].ull);
                break;
            }
            default:
                break;
        }

        if (i + 1 < vc->count)
        {
            cargo_aappendf(&str, ", ");
        }
    }

    va_end(ap);

    return v;
fail:
    _cargo_xfree(&vc->strs);
    _cargo_xfree(&vc->nums);
    _cargo_xfree(&vc->err);
    _cargo_free(vc);
    _cargo_free(v);
    return NULL;
}

int cargo_add_validation(cargo_t ctx, cargo_validation_flags_t flags,
                        const char *opt, cargo_validation_t *vd)
{
    size_t opt_i;
    size_t name_i;
    cargo_opt_t *o;
    assert(ctx);
    assert(opt);

    if (!vd)
    {
        CARGODBG(1, "Got NULL validation for options \"%s\"\n", opt);
        return -1;
    }

    if (!(vd->validator))
    {
        CARGODBG(1, "Validation missing validator function for \"%s\"\n", opt);
        goto fail;
    }

    if (_cargo_find_option_name(ctx, opt, &opt_i, &name_i))
    {
        CARGODBG(1, "Failed to find option \"%s\"\n", opt);
        goto fail;
    }

    o = &ctx->options[opt_i];

    if (!(o->type & vd->types))
    {
        CARGODBG(1, "\"%s\" of type \"%s\" is not supported by the validation %s\n",
                opt, _cargo_type_to_str(o->type), vd->name);
        goto fail;
    }

    // Remove current validation.
    _cargo_option_destroy_validation(o);

    // We have a reference count so that
    // multiple options can use the same validation.
    vd->ref_count++;
    o->validation = vd;
    o->validation_flags = flags;

    return 0;
fail:
    _cargo_free_validation(&vd);
    return -1;
}

void cargo_free_commandline(char ***argv, int argc)
{
    size_t i;
    assert(argv);

    if (*argv)
    {
        for (i = 0; i < (size_t)argc; i++)
        {
            _cargo_xfree(&((*argv)[i]));
        }

        _cargo_xfree(argv);
    }
}

char **cargo_split_commandline(cargo_splitcmd_flags_t flags, const char *cmdline, int *argc)
{
    int i;
    char **argv = NULL;
    assert(argc);

    if (!cmdline)
    {
        CARGODBG(1, "Got NULL input\n");
        return NULL;
    }

    // Posix.
    #ifndef _WIN32
    {
        int ret;
        wordexp_t p;
        memset(&p, 0, sizeof(p));

        // Note! This expands shell variables.
        if ((ret = wordexp(cmdline, &p, 0)))
        {
            CARGODBG(1, "wordexp error %d: '%s'\n", ret, cmdline);
            return NULL;
        }

        *argc = p.we_wordc;

        if (!(argv = _cargo_calloc(*argc, sizeof(char *))))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }

        for (i = 0; i < p.we_wordc; i++)
        {
            if (!(argv[i] = _cargo_strdup(p.we_wordv[i])))
            {
                CARGODBG(1, "Out of memory!\n");
                goto fail;
            }
        }

        // wordfree seems to fail to free everything in the wordexp_t struct
        // at least on OSX 10.9.5. This is supposed to help on 10.8.5 at least.
        p.we_offs = 0;
        wordfree(&p);

        return argv;
    fail:
        p.we_offs = 0;
        wordfree(&p);
    }
    #else // WIN32
    {
        // TODO: __getmainargs is an alternative... https://msdn.microsoft.com/en-us/library/ff770599.aspx
        wchar_t **wargs = NULL;
        size_t needed = 0;
        wchar_t *cmdlinew = NULL;
        size_t len = strlen(cmdline) + 1;

        if (!(cmdlinew = _cargo_calloc(len, sizeof(wchar_t))))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }

        if (!MultiByteToWideChar(CP_ACP, 0, cmdline, -1, cmdlinew, len))
        {
            CARGODBG(1, "Failed to convert to unicode!\n");
            goto fail;
        }

        if (!(wargs = CommandLineToArgvW(cmdlinew, argc)))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }

        if (!(argv = _cargo_calloc(*argc, sizeof(char *))))
        {
            CARGODBG(1, "Out of memory!\n");
            goto fail;
        }

        // Convert from wchar_t * to ANSI char *
        for (i = 0; i < *argc; i++)
        {
            // Get the size needed for the target buffer.
            // CP_ACP = Ansi Codepage.
            needed = WideCharToMultiByte(CP_ACP, 0, wargs[i], -1,
                                        NULL, 0, NULL, NULL);

            if (!(argv[i] = _cargo_malloc(needed)))
            {
                CARGODBG(1, "Out of memory!\n");
                goto fail;
            }

            // Do the conversion.
            needed = WideCharToMultiByte(CP_ACP, 0, wargs[i], -1,
                                        argv[i], needed, NULL, NULL);
        }

        if (wargs) LocalFree(wargs);
        _cargo_xfree(&cmdlinew);
        return argv;

    fail:
        if (wargs) LocalFree(wargs);
        _cargo_xfree(&cmdlinew);
    }
    #endif // WIN32

    if (argv)
    {
        for (i = 0; i < *argc; i++)
        {
            _cargo_xfree(&argv[i]);
        }

        _cargo_free(argv);
    }

    return NULL;
}

const char *cargo_get_version()
{
    return CARGO_VERSION_STR;
}

void cargo_set_context(cargo_t ctx, void *user)
{
    assert(ctx);
    ctx->user = user;
}

void *cargo_get_context(cargo_t ctx)
{
    assert(ctx);
    return ctx->user;
}

int cargo_set_group_context(cargo_t ctx, const char *group, void *user)
{
    if (!group) group = "";
    return _cargo_set_group_context(ctx, group,
                ctx->groups, ctx->group_count, user);
}

void *cargo_get_group_context(cargo_t ctx, const char *group)
{
    if (!group) group = "";
    return _cargo_get_group_context(ctx, group, ctx->groups, ctx->group_count);
}

int cargo_set_mutex_group_context(cargo_t ctx, const char *mutex_group, void *user)
{
    assert(mutex_group);
    return _cargo_set_group_context(ctx, mutex_group,
                ctx->mutex_groups, ctx->mutex_group_count, user);
}

void *cargo_get_mutex_group_context(cargo_t ctx, const char *mutex_group)
{
    assert(mutex_group);
    return _cargo_get_group_context(ctx, mutex_group,
                ctx->mutex_groups, ctx->mutex_group_count);
}

const char *cargo_get_option_group(cargo_t ctx, const char *opt)
{
    return _cargo_option_get_group(ctx, opt, ctx->groups, ctx->group_count);
}

const char **cargo_get_option_mutex_groups(cargo_t ctx,
                                            const char *opt, size_t *count)
{
    size_t i;
    cargo_opt_t *o = NULL;
    cargo_group_t *mgrp = NULL;
    size_t opt_i;
    assert(ctx);
    assert(opt);

    if (count) *count = 0;

    if (_cargo_find_option_name(ctx, opt, &opt_i, NULL))
    {
        CARGODBG(1, "No such option \"%s\"\n", opt);
        return NULL;
    }

    o = &ctx->options[opt_i];

    // Use cached version. Mutex group count should
    // not be changed between calls anyway.
    if (o->mutex_group_names)
    {
        if (count) *count = o->mutex_group_count;
        return (const char **)o->mutex_group_names;
    }

    if (!(o->mutex_group_names = _cargo_calloc(o->mutex_group_count, sizeof(char *))))
    {
        CARGODBG(1, "Out of memory\n");
        return NULL;
    }

    for (i = 0; i < o->mutex_group_count; i++)
    {
        mgrp = &ctx->mutex_groups[o->mutex_group_idxs[i]];
        o->mutex_group_names[i] = _cargo_strdup(mgrp->name);
    }

    if (count) *count = o->mutex_group_count;

    if (o->mutex_group_count == 0)
    {
        return NULL;
    }

    return (const char **)o->mutex_group_names;
}

cargo_type_t cargo_get_option_type(cargo_t ctx, const char *opt)
{
    cargo_opt_t *o = NULL;
    size_t opt_i;
    assert(ctx);
    assert(opt);

    if (_cargo_find_option_name(ctx, opt, &opt_i, NULL))
    {
        CARGODBG(1, "No such option \"%s\"\n", opt);
        return -1;
    }

    o = &ctx->options[opt_i];

    return o->type;
}

#endif // !CARGO_NOLIB

// -----------------------------------------------------------------------------
// Tests.
// -----------------------------------------------------------------------------
// LCOV_EXCL_START
#ifdef CARGO_TEST

#define LOREM_IPSUM                                                         \
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do"       \
    " eiusmod tempor incididunt ut labore et dolore magna aliqua. "         \
    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "   \
    "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "    \
    "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "  \
    "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in "    \
    "culpa qui officia deserunt mollit anim id est laborum."


#define cargo_assert(test, message) \
do                                  \
{                                   \
    if (!(test))                    \
    {                               \
        msg = message;              \
        goto fail;                  \
    }                               \
} while (0)

// Used for asserting in custom callbacks.
#define cargo_cb_assert(cond, msg)      \
do                                      \
{                                       \
    if (!(cond))                        \
    {                                   \
        fprintf(stderr, "%s\n", msg);   \
        return -1;                      \
    }                                   \
} while (0);

#define cargo_assert_array(count, expected_count, array, array_expected)    \
do                                                                          \
{                                                                           \
    size_t k;                                                               \
    printf("Check that \""#array"\" has "#expected_count" elements\n");     \
    cargo_assert((count) == (expected_count),                               \
        #array" array count "#count" is not expected "#expected_count);     \
    for (k = 0; k < (count); k++)                                           \
    {                                                                       \
        cargo_assert((array)[k] == (array_expected)[k],                     \
            "Array contains unexpected value");                             \
    }                                                                       \
} while (0)

#define cargo_assert_str_array(count, expected_count, array, array_expected)    \
do                                                                              \
{                                                                               \
    size_t k;                                                                   \
    printf("Check that \""#array"\" has "#expected_count" elements\n");         \
    cargo_assert((count) == (expected_count),                                   \
        #array" array count "#count" is not expected "#expected_count);         \
    for (k = 0; k < count; k++)                                                 \
    {                                                                           \
        printf("  %lu: \"%s\" -> \"%s\"\n",                                     \
            k+1, (char *)(array)[k], (char *)(array_expected)[k]);              \
        cargo_assert(!strcmp((char *)(array)[k], (char *)(array_expected)[k]),  \
            "Array contains unexpected value");                                 \
    }                                                                           \
} while (0)

static int malloc_fail_count;
static int malloc_current;
static int realloc_fail_count;
static int realloc_current;

void _cargo_test_set_malloc_fail_count(int count)
{
    malloc_current = 0;
    malloc_fail_count = count;
}

void *_cargo_test_malloc(size_t sz)
{
    malloc_current++;

    if ((malloc_fail_count > 0)
        && (malloc_current >= malloc_fail_count))
        return NULL;

    return malloc(sz);
}

void _cargo_test_set_realloc_fail_count(int count)
{
    realloc_current = 0;
    realloc_fail_count = count;
}

void *_cargo_test_realloc(void *ptr, size_t sz)
{
    realloc_current++;

    if ((realloc_fail_count > 0)
        && (realloc_current >= realloc_fail_count))
        return NULL;

    return realloc(ptr, sz);
}

//
// Some helper macros for creating test functions.
//
#define _MAKE_TEST_FUNC_NAME(f) f()

// Start a test. Initializes the lib.
#define _TEST_START_EX(testname, flags)                 \
static const char *_MAKE_TEST_FUNC_NAME(testname)       \
{                                                       \
    const char *msg = NULL;                             \
    int ret = 0;                                        \
    cargo_t cargo;                                      \
                                                        \
    if (cargo_init(&cargo, flags, "program"))           \
    {                                                   \
        return "Failed to init cargo";                  \
    }

#define _TEST_START(testname) _TEST_START_EX(testname, 0)

// Cleanup point, must be placed at the end of the test
//
// scope _TEST_START(name) { ... _TEST_CLEANUP(); free(something) } _TEST_END()
//
// cargo_assert jumps to this point on failure. Any cleanup should be
// placed after this call.
#define _TEST_CLEANUP()     \
    fail: (void)0

#define _TEST_END()         \
    cargo_destroy(&cargo);  \
    return msg;             \
}

// For tests where we want to assert after
// cargo_destroy has been called.
#define _TEST_END_NODESTROY()   \
    return msg;                 \
}

// =================================================================
// Test functions.
// =================================================================

_TEST_START(TEST_no_args_bool_option)
{
    int a;
    char *args[] = { "program", "--alpha" };
    int argc = sizeof(args) / sizeof(args[0]);

    ret = cargo_add_option(cargo, 0, "--alpha", "Description", "b", &a);
    cargo_assert(ret == 0, "Failed to add valid bool option");

    if (cargo_parse(cargo, 0, 1, argc, args))
    {
        msg = "Failed to parse bool with no argument";
        goto fail;
    }

    cargo_assert(a == 1, "Failed to parse bool with no argument to 1");

    _TEST_CLEANUP();
}
_TEST_END()

//
// Simple add option tests.
//
#define _TEST_ADD_SIMPLE_OPTION(name, type, value, fmt, ...)                \
    _TEST_START(name)                                                       \
    {                                                                       \
        char *args[] = { "program", "--alpha", #value };                    \
        type a;                                                             \
        ret = cargo_add_option(cargo, 0, "--alpha -a",                      \
                                "Description",                              \
                                fmt,                                        \
                                &a, ##__VA_ARGS__);                         \
        cargo_assert(ret == 0, "Failed to add valid "#type" option");       \
        if (cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args)) \
        {                                                                   \
            msg = "Failed to parse "#type" with value \""#value"\"";        \
            goto fail;                                                      \
        }                                                                   \
        printf("Attempt to parse value: "#value"\n");                       \
        cargo_assert(a == value, "Failed to parse correct value "#value);   \
        _TEST_CLEANUP();                                                    \
    }                                                                       \
    _TEST_END()

_TEST_ADD_SIMPLE_OPTION(TEST_add_integer_option, int, -3, "i")
_TEST_ADD_SIMPLE_OPTION(TEST_add_uinteger_option, unsigned int, 3, "u")
_TEST_ADD_SIMPLE_OPTION(TEST_add_float_option, float, 0.3f, "f")
_TEST_ADD_SIMPLE_OPTION(TEST_add_bool_option, int, 1, "b")
_TEST_ADD_SIMPLE_OPTION(TEST_add_double_option, double, 0.4, "d")

_TEST_START(TEST_add_static_string_option)
{
    char b[10];
    char *args[] = { "program", "--beta", "abc" };
    ret = cargo_add_option(cargo, 0, "--beta -b",
                            "Description",
                            ".s#",
                            &b, sizeof(b));
    cargo_assert(ret == 0, "Failed to add valid static string option");

    if (cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args))
    {
        msg = "Failed to parse static char * with value \"abc\"";
        goto fail;
    }
    printf("Attempt to parse value: abc\n");
    cargo_assert(!strcmp(b, "abc"), "Failed to parse correct value abc");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_alloc_string_option)
{
    char *b = NULL;
    char *args[] = { "program", "--beta", "abc" };
    ret = cargo_add_option(cargo, 0, "--beta -b",
                            "Description",
                            "s",
                            &b);
    cargo_assert(ret == 0, "Failed to add valid alloc string option");

    if (cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args))
    {
        msg = "Failed to parse alloc char * with value \"abc\"";
        goto fail;
    }
    printf("Attempt to parse value: abc\n");
    cargo_assert(b, "pointer is null");
    cargo_assert(!strcmp(b, "abc"), "Failed to parse correct value abc");

    _TEST_CLEANUP();
    _cargo_free(b);
}
_TEST_END()

//
// =========================== Array Tests ====================================
//
#define _TEST_ARRAY_OPTION(array, array_size, args, argc, fmt, ...)          \
{                                                                            \
    ret = cargo_add_option(cargo, 0, "--beta -b", "Description",             \
                            fmt, ##__VA_ARGS__);                             \
    cargo_assert(ret == 0,                                                   \
        "Failed to add "#array"["#array_size"] array option");               \
    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);    \
    cargo_assert(ret == 0, "Failed to parse array: "#array"["#array_size"]");\
}

#define ARG_SIZE (sizeof(args) / sizeof(args[0]))
#define ARRAY_SIZE (sizeof(a_expect) / sizeof(a_expect[0]))
#define _ADD_TEST_FIXED_ARRAY(fmt, printfmt)                            \
do                                                                      \
{                                                                       \
    size_t count = 0;                                                       \
    size_t i;                                                           \
    _TEST_ARRAY_OPTION(a, ARRAY_SIZE, args, ARG_SIZE,                   \
        fmt, &a, &count, ARRAY_SIZE);                                   \
    cargo_assert(a != NULL, "Array is null");                           \
    cargo_assert(count == ARRAY_SIZE, "Array count is invalid");        \
    printf("Read %lu values from int array:\n", count);                 \
    for (i = 0; i < ARRAY_SIZE; i++) printf("  "printfmt"\n", a[i]);    \
    cargo_assert_array(count, ARRAY_SIZE, a, a_expect);                 \
} while (0)

///
/// Simple add array tests.
///
_TEST_START(TEST_add_static_int_array_option)
{
    int a[3];
    int a_expect[3] = { 1, -2, 3 };
    char *args[] = { "program", "--beta", "1", "-2", "3" };
    _ADD_TEST_FIXED_ARRAY(".[i]#", "%d");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_static_uint_array_option)
{
    unsigned int a[3];
    unsigned int a_expect[3] = { 1, 2, 3 };
    char *args[] = { "program", "--beta", "1", "2", "3" };
    _ADD_TEST_FIXED_ARRAY(".[u]#", "%u");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_static_long_long_int_array_option)
{
    long long int a[3];
    long long int a_expect[3] = { 9223372036854775807, -9223372036854775807, 3 };
    char *args[] = { "program", "--beta", "9223372036854775807", "-9223372036854775807", "3" };
    memset(a, 0, sizeof(a));
    _ADD_TEST_FIXED_ARRAY(".[L]#", "%"CARGO_LONGLONG_FMT);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_static_unsigned_long_long_int_array_option)
{
    long long int a[3];
    long long int a_expect[3] = { 1844674407370955161, 9223372036854775807, 3 };
    char *args[] = { "program", "--beta", "1844674407370955161", "9223372036854775807", "3" };
    _ADD_TEST_FIXED_ARRAY(".[U]#", "%"CARGO_ULONGLONG_FMT);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_static_float_array_option)
{
    float a[3];
    float a_expect[3] = { 0.1f, 0.2f, 0.3f };
    char *args[] = { "program", "--beta", "0.1", "0.2", "0.3" };
    _ADD_TEST_FIXED_ARRAY(".[f]#", "%f");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_static_double_array_option)
{
    double a[3];
    double a_expect[3] = { 0.1, 0.2, 0.3 };
    char *args[] = { "program", "--beta", "0.1", "0.2", "0.3" };
    _ADD_TEST_FIXED_ARRAY(".[d]#", "%f");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_static_string_array_option)
{
    #define LENSTR 5
    char a[3][LENSTR];
    size_t count = 0;
    char *args[] = { "program", "--beta", "abc", "def", "ghi" };
    #define ARRAY2_SIZE (sizeof(a) / sizeof(a[0]))
    #define ARG_SIZE (sizeof(args) / sizeof(args[0]))
    memset(a, 0, sizeof(a));

    _TEST_ARRAY_OPTION(a, ARRAY2_SIZE, args, ARG_SIZE,
                        ".[s#]#", &a, LENSTR, &count, ARRAY2_SIZE);

    printf("Read %lu values from int array: %s, %s, %s\n",
            count, a[0], a[1], a[2]);
    cargo_assert(count == ARRAY2_SIZE, "Array count is not 3 as expected");
    cargo_assert(!strcmp(a[0], "abc"), "Array value at index 0 is not \"abc\" as expected");
    cargo_assert(!strcmp(a[1], "def"), "Array value at index 1 is not \"def\" as expected");
    cargo_assert(!strcmp(a[2], "ghi"), "Array value at index 2 is not \"ghi\" as expected");
    _TEST_CLEANUP();
}
_TEST_END()

///
/// Alloc fixed size array tests.
///
_TEST_START(TEST_add_alloc_fixed_int_array_option)
{
    int *a = NULL;
    int a_expect[3] = { 1, -2, 3 };
    char *args[] = { "program", "--beta", "1", "-2", "3" };
    _ADD_TEST_FIXED_ARRAY("[i]#", "%d");
    _TEST_CLEANUP();
    _cargo_free(a);
}
_TEST_END()

_TEST_START(TEST_add_alloc_fixed_uint_array_option)
{
    unsigned int *a = NULL;
    unsigned int a_expect[3] = { 1, 2, 3 };
    char *args[] = { "program", "--beta", "1", "2", "3" };
    _ADD_TEST_FIXED_ARRAY("[u]#", "%u");
    _TEST_CLEANUP();
    _cargo_free(a);
}
_TEST_END()

_TEST_START(TEST_add_alloc_fixed_float_array_option)
{
    float *a = NULL;
    float a_expect[3] = { 1.1f, -2.2f, 3.3f };
    char *args[] = { "program", "--beta", "1.1", "-2.2", "3.3" };
    _ADD_TEST_FIXED_ARRAY("[f]#", "%f");
    _TEST_CLEANUP();
    _cargo_free(a);
}
_TEST_END()

_TEST_START(TEST_add_alloc_fixed_double_array_option)
{
    double *a = NULL;
    double a_expect[3] = { 1.1, -2.2, 3.3 };
    char *args[] = { "program", "--beta", "1.1", "-2.2", "3.3" };
    _ADD_TEST_FIXED_ARRAY("[d]#", "%f");
    _TEST_CLEANUP();
    _cargo_free(a);
}
_TEST_END()

_TEST_START(TEST_add_alloc_fixed_string_array_option)
{
    #define LENSTR 5
    char **a = NULL;
    size_t count;
    char *args[] = { "program", "--beta", "abc", "def", "ghi" };
    #define ARG_SIZE (sizeof(args) / sizeof(args[0]))

    _TEST_ARRAY_OPTION(a, 3, args, ARG_SIZE,
                        "[s#]#", &a, LENSTR, &count, 3);

    cargo_assert(a != NULL, "Array is null");
    cargo_assert(count == 3, "Array count is not 3");
    printf("Read %lu values from int array: %s, %s, %s\n",
            count, a[0], a[1], a[2]);
    cargo_assert(count == 3, "Array count is not 3 as expected");
    cargo_assert(!strcmp(a[0], "abc"), "Array value at index 0 is not \"abc\" as expected");
    cargo_assert(!strcmp(a[1], "def"), "Array value at index 1 is not \"def\" as expected");
    cargo_assert(!strcmp(a[2], "ghi"), "Array value at index 2 is not \"ghi\" as expected");
    _TEST_CLEANUP();
    _cargo_free_str_list(&a, &count);
}
_TEST_END()

_TEST_START(TEST_add_alloc_fixed_string_array_option2)
{
    char **a = NULL;
    size_t count;
    char *args[] = { "program", "--beta", "abc", "def", "ghi" };
    #define ARG_SIZE (sizeof(args) / sizeof(args[0]))

    _TEST_ARRAY_OPTION(a, 3, args, ARG_SIZE,
                        "[s]#", &a, &count, 3);

    cargo_assert(a != NULL, "Array is null");
    cargo_assert(count == 3, "Array count is not 3");
    printf("Read %lu values from string array: %s, %s, %s\n",
            count, a[0], a[1], a[2]);
    cargo_assert(count == 3, "Array count is not 3 as expected");
    cargo_assert(!strcmp(a[0], "abc"), "Array value at index 0 is not \"abc\" as expected");
    cargo_assert(!strcmp(a[1], "def"), "Array value at index 1 is not \"def\" as expected");
    cargo_assert(!strcmp(a[2], "ghi"), "Array value at index 2 is not \"ghi\" as expected");
    _TEST_CLEANUP();
    _cargo_free_str_list(&a, &count);
}
_TEST_END()

_TEST_START(TEST_add_alloc_fixed_string_array_option3)
{
    #define NUM_ELEMENTS 3
    char *a[NUM_ELEMENTS];
    size_t count = 0;
    size_t i;
    char *args[] = { "program", "--beta", "abc", "def", "ghi" };
    #define ARG_SIZE (sizeof(args) / sizeof(args[0]))
    memset(a, 0, sizeof(a));

    _TEST_ARRAY_OPTION(a, 3, args, ARG_SIZE,
                        ".[s]#", &a, &count, NUM_ELEMENTS);

    cargo_assert(a != NULL, "Array is null");
    cargo_assert(count == 3, "Array count is not 3");
    printf("Read %lu values from string array: %s, %s, %s\n",
            count, a[0], a[1], a[2]);
    cargo_assert(count == NUM_ELEMENTS, "Array count is not 3 as expected");
    cargo_assert(!strcmp(a[0], "abc"), "Array value at index 0 is not \"abc\" as expected");
    cargo_assert(!strcmp(a[1], "def"), "Array value at index 1 is not \"def\" as expected");
    cargo_assert(!strcmp(a[2], "ghi"), "Array value at index 2 is not \"ghi\" as expected");
    _TEST_CLEANUP();
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if (a[i]) free(a[i]);
    }
}
_TEST_END()

_TEST_START(TEST_add_alloc_fixed_string_array_option4)
{
    #define NUM_ELEMENTS 3
    char *a[NUM_ELEMENTS];
    size_t count = 0;
    size_t i;
    char *args[] = { "program", "--beta", "abc", "def", "ghi" };
    #define ARG_SIZE (sizeof(args) / sizeof(args[0]))
    memset(a, 0, sizeof(a));

    _TEST_ARRAY_OPTION(a, 3, args, ARG_SIZE,
                        ".[s]+", &a, &count, NUM_ELEMENTS);

    cargo_assert(a != NULL, "Array is null");
    cargo_assert(count == 3, "Array count is not 3");
    printf("Read %lu values from string array: %s, %s, %s\n",
            count, a[0], a[1], a[2]);
    cargo_assert(count == NUM_ELEMENTS, "Array count is not 3 as expected");
    cargo_assert(!strcmp(a[0], "abc"), "Array value at index 0 is not \"abc\" as expected");
    cargo_assert(!strcmp(a[1], "def"), "Array value at index 1 is not \"def\" as expected");
    //cargo_assert(!strcmp(a[2], "ghi"), "Array value at index 2 is not \"ghi\" as expected");
    _TEST_CLEANUP();
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if (a[i]) free(a[i]);
    }
}
_TEST_END()

//
// Dynamic sized alloc array tests.
//
_TEST_START(TEST_add_alloc_dynamic_int_array_option)
{
    int *a = NULL;
    int a_expect[3] = { 1, -2, 3 };
    char *args[] = { "program", "--beta", "1", "-2", "3" };
    _ADD_TEST_FIXED_ARRAY("[i]+", "%d");
    _TEST_CLEANUP();
    _cargo_free(a);
}
_TEST_END()

_TEST_START(TEST_add_alloc_dynamic_int_array_option_noargs)
{
    int *a = NULL;
    char *args[] = { "program", "--beta" };
    size_t count;

    ret = cargo_add_option(cargo, 0, "--beta -b", "Description", "[i]+",
                            &a, &count);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Successfully parsed when no args");

    _TEST_CLEANUP();
}
_TEST_END()

//
// Misc output tests.
//
_TEST_START(TEST_print_usage)
{
    int a[3];
    size_t a_count = 0;
    float b;
    double c;
    char *s = NULL;
    int *vals = NULL;
    size_t val_count = 0;

    ret |= cargo_add_option(cargo, 0, "--alpha -a",
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
            "eiusmod tempor incididunt ut labore et dolore magna aliqua. "
            "Ut enim ad minim veniam, quis nostrud exercitation ullamco "
            "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure "
            "dolor in reprehenderit in voluptate velit esse cillum dolore eu "
            "fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
            "proident, sunt in culpa qui officia deserunt mollit anim id est "
            "laborum",
            ".[i]#",
            &a, &a_count, sizeof(a) / sizeof(a[0]));

    ret |= cargo_add_option(cargo, 0, "--beta -b",
            "Shorter description",
            "f",
            &b);
    ret |= cargo_set_metavar(cargo, "--beta", "FLOAT");

    ret |= cargo_add_option(cargo, 0, "--call_this_a_long_option_that_wont_fit -c",
            "Sed ut perspiciatis unde omnis iste natus error sit voluptatem "
            "accusantium doloremque laudantium, totam rem aperiam, eaque ipsa "
            "quae ab illo inventore veritatis et quasi architecto beatae vitae "
            "dicta sunt explicabo",
            "d",
            &c);

    ret |= cargo_add_option(cargo, 0, "--shorter -s",
            "Sed ut perspiciatis unde omnis iste natus error sit voluptatem "
            "accusantium doloremque laudantium, totam rem aperiam, eaque ipsa "
            "quae ab illo inventore veritatis et quasi architecto beatae vitae "
            "dicta sunt explicabo",
            "s",
            &s);

    ret |= cargo_add_option(cargo, 0, "--vals -v",
            "Shorter description",
            "[ i ]+",
            &vals, &val_count);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_set_epilog(cargo, "That's it!");
    cargo_set_description(cargo, "Introductionary description");

    cargo_print_usage(cargo, 0);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_get_usage_settings)
{
    typedef struct _test_usage_settings_s
    {
        cargo_usage_t fmt;
        char *expect[4];
        size_t expect_count;
    } _test_usage_settings_t;

    #define DESCRIPT "Brown fox"
    #define EPILOG "Lazy fox"
    #define OPT_TXT "The alpha"
    size_t j;
    size_t k;
    int i;
    const char *usage = NULL;
    _test_usage_settings_t tus[] =
    {
        { 0,                                { DESCRIPT, EPILOG, OPT_TXT }, 3},
        { CARGO_USAGE_HIDE_EPILOG,          { DESCRIPT, OPT_TXT }, 2},
        { CARGO_USAGE_HIDE_DESCRIPTION,     { EPILOG, OPT_TXT }, 2}
    };

    ret |= cargo_add_option(cargo, 0, "--alpha -a", OPT_TXT, "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_set_description(cargo, "Brown fox");
    cargo_set_epilog(cargo, "Lazy fox");

    for (j = 0; j < sizeof(tus) / sizeof(tus[0]); j++)
    {
        usage = cargo_get_usage(cargo, tus[j].fmt);
        cargo_assert(usage != NULL, "Got null usage");
        printf("\n\n");

        for (k = 0; k < tus[j].expect_count; k++)
        {
            char *s = tus[j].expect[k];
            char *found = strstr(usage, s);
            printf("-------------------------------------\n");
            printf("%s\n", usage);
            printf("-------------------------------------\n");
            printf("Expecting to find in usage: \"%s\"\n", s);
            cargo_assert(found != NULL,
                "Usage formatting unexpected");
        }

        printf("-------------------------------------\n");
        printf("%s\n", usage);
        printf("-------------------------------------\n");
    }

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_autohelp_default)
{
    int i;
    const char *usage = NULL;

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);

    // Default is to automatically add --help option.
    usage = cargo_get_usage(cargo, 0);
    cargo_assert(usage != NULL, "Got NULL usage");
    printf("-------------------------------------\n");
    printf("%s", usage);
    printf("-------------------------------------\n");
    cargo_assert(strstr(usage, "help") != NULL, "No help found by default");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START_EX(TEST_autohelp_off, CARGO_NO_AUTOHELP)
{
    int i;
    const char *usage = NULL;

    // Turn off auto_help (--help).
    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);

    usage = cargo_get_usage(cargo, 0);
    cargo_assert(usage != NULL, "Got NULL usage");
    printf("-------------------------------------\n");
    printf("%s", usage);
    printf("-------------------------------------\n");
    cargo_assert(strstr(usage, "help") == NULL,
                "Help found when auto_help turned off");

    _TEST_CLEANUP();
    cargo_destroy(&cargo);
}
_TEST_END_NODESTROY()

#define _ADD_TEST_USAGE_OPTIONS()                                           \
do                                                                          \
{                                                                           \
    int *k;                                                                 \
    size_t k_count;                                                         \
    int i;                                                                  \
    float f;                                                                \
    int b;                                                                  \
    ret |= cargo_add_option(cargo, 0, "pos", "Positional arg", "[i]+",      \
                            &k, &k_count);                                  \
    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha" LOREM_IPSUM, "i", &i);  \
    ret |= cargo_add_option(cargo, 0, "--beta", "The alpha", "f", &f);      \
    ret |= cargo_add_option(cargo, 0, "--crash -c", "The alpha", "b", &b);  \
    cargo_assert(ret == 0, "Failed to add options");                        \
} while (0)

_TEST_START(TEST_get_usage)
{
    const char *usage = NULL;

    _ADD_TEST_USAGE_OPTIONS();
    usage = cargo_get_usage(cargo, 0);
    cargo_assert(usage != NULL, "Failed to get allocated usage");
    printf("%s\n", usage);
    printf("Cargo v%s\n", cargo_get_version());

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_misspelled_argument)
{
    int i;
    float f;
    int b;
    char *args[] = { "program", "--bota", "0.1" };

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    ret |= cargo_add_option(cargo, 0, "--beta", "The alpha", "f", &f);
    ret |= cargo_add_option(cargo, 0, "--crash -c", "The alpha", "b", &b);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, 3, args);
    cargo_assert(ret == -1, "Got valid parse with invalid input");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_add_duplicate_option)
{
    int i;

    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");
    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    cargo_assert(ret != 0, "Succesfully added duplicated, not allowed");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_get_extra_args)
{
    size_t argc = 0;
    const char **extra_args = NULL;
    char *extra_args_expect[] = { "abc", "def", "ghi" };
    int i;
    char *args[] = { "program", "-a", "1", "abc", "def", "ghi" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse extra args input");

    // Get left over arguments.
    extra_args = cargo_get_args(cargo, &argc);
    cargo_assert(extra_args != NULL, "Got NULL extra args");

    printf("argc = %lu\n", argc);
    cargo_assert(argc == 3, "Expected argc == 3");
    cargo_assert_str_array(argc, 3, extra_args, extra_args_expect);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_get_unknown_opts)
{
    size_t unknown_count = 0;
    const char **unknown_opts = NULL;
    char *unknown_opts_expect[] = { "-b", "-c" };
    int i;
    char *args[] = { "program", "-a", "1", "-b", "-c", "3" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succeeded parsing with unknown options");

    // Get left over arguments.
    unknown_opts = cargo_get_unknown(cargo, &unknown_count);
    cargo_assert(unknown_opts != NULL, "Got NULL unknown options");

    printf("Unknown option count = %lu\n", unknown_count);
    cargo_assert(unknown_count == 2, "Unknown option count == 2");
    cargo_assert_str_array(unknown_count, 2, unknown_opts, unknown_opts_expect);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_split)
{
    size_t i;
    size_t j;

    char *in[] =
    {
        " abc def   ghi   ",
        "abc",
        NULL
    };
    #define NUM (sizeof(in) / sizeof(in[0]))

    char *expect0[] = { "abc", "def", "ghi" };
    char *expect1[] = { "abc" };
    char *expect2[] = { NULL };
    char **expect[] = { expect0, expect1, expect2 };
    size_t expect_count[] =
    {
        3,
        1,
        0
    };

    char **out[NUM];
    size_t out_count[NUM];
    ret = 0;

    for (i = 0; i < NUM; i++)
    {
        printf("Split: \"%s\"", in[i]);
        out[i] = _cargo_split(in[i], " ", &out_count[i]);
        printf(" into %lu substrings\n", out_count[i]);

        if (in[i] != NULL)
            cargo_assert(out[i] != NULL, "Got null split result");

        for (j = 0; j < out_count[i]; j++)
        {
            printf("\"%s\"%s ",
                out[i][j], ((j + 1) != out_count[i]) ? "," : "");
        }

        printf("\n");
    }

    for (i = 0; i < NUM; i++)
    {
        cargo_assert_str_array(out_count[i], expect_count[i], out[i], expect[i]);
    }

    _TEST_CLEANUP();
    for (i = 0; i < NUM; i++)
    {
        _cargo_free_str_list(&out[i], &out_count[i]);
    }
    #undef NUM
}
_TEST_END()

_TEST_START(TEST_parse_invalid_value)
{
    int i;
    int j;
    char *args[] = { "program", "--alpha", "1", "--beta", "a" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    ret = cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &j);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succesfully parsed invalid value");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_twice)
{
    // This test make sure the parser is reset between
    // different parse sessions.
    #define PORTS_COUNT 3
    int ports[PORTS_COUNT];
    size_t ports_count = 0;
    char *name = NULL;
    char **vals = NULL;
    size_t vals_count = 0;

    char *args1[] =
    {
        "program",
        "--ports", "22", "24", "26",
        "--vals", "abc", "def", "123456789101112", "ghi", "jklmnopq",
        "--name", "server"
    };
    int args1_ports_expect[] = { 22, 24, 26 };
    char *args1_vals_expect[] = { "abc", "def", "123456789101112", "ghi", "jklmnopq" };

    char *args2[] =
    {
        "program",
        "--vals", "abc", "def", "123456789101112", "ghi", "jklmnopq",
        "--ports", "33", "34", "36",
        "--name", "server"
    };
    int args2_ports_expect[] = { 33, 34, 36 };

    ret |= cargo_add_option(cargo, 0, "--ports -p", "Ports", ".[i]#",
                            &ports, &ports_count, PORTS_COUNT);
    ret |= cargo_add_option(cargo, 0, "--name -n", "Name", "s", &name);
    ret |= cargo_add_option(cargo, 0, "--vals -v", "Description of vals", "[s]+",
                            &vals, &vals_count);
    cargo_assert(ret == 0, "Failed to add options");

    printf("\nArgs 1:\n");
    {
        ret = cargo_parse(cargo, 0, 1, sizeof(args1) / sizeof(args1[0]), args1);

        cargo_assert(ret == 0, "Failed to parse advanced example");
        cargo_assert_array(ports_count, PORTS_COUNT, ports, args1_ports_expect);
        cargo_assert_str_array(vals_count, 5, vals, args1_vals_expect);
        cargo_assert(name && !strcmp(name, "server"), "Expected name = \"server\"");

        // TODO: Remove this and make sure these are freed at cargo_parse instead
        _cargo_free_str_list(&vals, &vals_count);
        _cargo_xfree(&name);
        memset(&ports, 0, sizeof(ports));
    }

    printf("\nArgs 2:\n");
    {
        ret = cargo_parse(cargo, 0, 1, sizeof(args2) / sizeof(args2[0]), args2);

        cargo_assert(ret == 0, "Failed to parse advanced example");
        cargo_assert_str_array(vals_count, 5, vals, args1_vals_expect);
        cargo_assert_array(ports_count, PORTS_COUNT, ports, args2_ports_expect);
        cargo_assert(!strcmp(name, "server"), "Expected name = \"server\"");
    }

    _TEST_CLEANUP();
    _cargo_free_str_list(&vals, &vals_count);
    _cargo_xfree(&name);
}
_TEST_END()

_TEST_START(TEST_parse_missing_value)
{
    int i;
    int j;
    char *args[] = { "program", "--alpha", "1", "--beta" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    ret = cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &j);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succesfully parsed missing value");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_missing_array_value)
{
    int i[3];
    size_t i_count = 0;
    int j = 0;
    char *args[] = { "program", "--beta", "2", "--alpha", "1", "2" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", ".[i]#", &i, &i_count, 3);
    ret = cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &j);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succesfully parsed missing value");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_missing_array_value_ensure_free)
{
    // Here we make sure allocated values get freed on a failed parse.
    int i[3];
    size_t i_count = 0;
    int *j = NULL;
    size_t j_count = 0;
    char *args[] = { "program", "--beta", "2", "3", "--alpha", "1", "2" };

    memset(i, 0, sizeof(i));

    ret = cargo_add_option(cargo, 0, "--alpha -a", "The alpha", ".[i]#", &i, &i_count, 3);
    ret = cargo_add_option(cargo, 0, "--beta -b", "The beta", "[i]#", &j, &j_count, 2);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succesfully parsed missing value");
    cargo_assert(j == NULL, "Array non-NULL after failed parse");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_same_option_twice)
{
    int i;
    int j;
    char *args[] = { "program", "--alpha", "1", "--beta", "4", "--alpha", "2" };

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "i", &i);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &j);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    printf("--alpha == %d\n", i);
    cargo_assert(ret == 0, "Failed to parsed duplicate option without unique");
    cargo_assert(i == 2, "Expected --alpha to have value 2");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_same_option_twice_string)
{
    char *s = NULL;
    int i;
    char *args[] = { "program", "--alpha", "abc", "--beta", "4", "--alpha", "def" };

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "s", &s);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parsed duplicate option without unique");
    cargo_assert(s && !strcmp(s, "def"), "Expected --alpha to have value \"def\"");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START(TEST_parse_same_option_twice_with_unique)
{
    int i;
    int j;
    char *args[] = { "program", "--alpha", "1", "--beta", "4", "--alpha", "2" };

    ret |= cargo_add_option(cargo, CARGO_OPT_UNIQUE,
                                "--alpha -a", "The alpha", "i", &i);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &j);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    printf("--alpha == %d\n", i);
    cargo_assert(ret != 0, "Succesfully parsed duplicate option");
    cargo_assert(i == 1, "Expected --alpha to have value 1");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_same_option_twice_string_with_unique)
{
    char *s = NULL;
    int i;
    char *args[] = { "program", "--alpha", "abc", "--beta", "4", "--alpha", "def" };

    ret |= cargo_add_option(cargo, CARGO_OPT_UNIQUE,
                            "--alpha -a", "The alpha", "s", &s);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    printf("--alpha = %s\n", s);
    cargo_assert(ret != 0, "Succesfully parsed duplicate option");
    cargo_assert(s == NULL, "Expected --alpha to have value NULL");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START(TEST_highlight_args)
{
    char *args[] =
    {
        "program",
        "--alpha", "abc",
        "--beta", "def", "ghi",
        "--crazy", "banans"
    };
    char *s = NULL;
    int argc = sizeof(args) / sizeof(args[0]);

    printf("With color highlight & args:\n");
    printf("----------------------------\n");
    ret = cargo_fprint_args(stdout,
                        argc,
                        args,
                        1, // Start index.
                        0, // flags
                        CARGO_DEFAULT_MAX_WIDTH,
                        3, // Highlight count.
                        1, "^"CARGO_COLOR_RED,
                        3, "~"CARGO_COLOR_GREEN,
                        4, "-");
    cargo_assert(ret == 0, "Failed call cargo_fprint_args");

    printf("With highlight & args:\n");
    printf("----------------------\n");
    ret = cargo_fprint_args(stdout,
                        argc,
                        args,
                        1, // Start index.
                        CARGO_FPRINT_NOCOLOR, // flags
                        CARGO_DEFAULT_MAX_WIDTH,
                        3, // Highlight count.
                        1, "^"CARGO_COLOR_RED,
                        3, "~"CARGO_COLOR_GREEN,
                        4, "-");
    cargo_assert(ret == 0, "Failed call cargo_fprint_args");

    printf("With highlight & no args:\n");
    printf("-------------------------\n");
    ret = cargo_fprint_args(stdout,
                        argc,
                        args,
                        1, // Start index.
                        CARGO_FPRINT_NOARGS, // flags
                        CARGO_DEFAULT_MAX_WIDTH,
                        3, // Highlight count.
                        1, "^"CARGO_COLOR_RED, 3, "~"CARGO_COLOR_GREEN, 4, "-");
    cargo_assert(ret == 0, "Failed call cargo_fprint_args");

    printf("With no highlight & args:\n");
    printf("-------------------------\n");
    ret = cargo_fprint_args(stdout,
                        argc,
                        args,
                        1, // Start index.
                        CARGO_FPRINT_NOHIGHLIGHT, // flags
                        CARGO_DEFAULT_MAX_WIDTH,
                        3, // Highlight count.
                        1, "^"CARGO_COLOR_RED, 3, "~"CARGO_COLOR_GREEN, 4, "-");
    cargo_assert(ret == 0, "Failed call cargo_fprint_args");

    printf("With default & not ordered:\n");
    printf("-------------------------\n");
    s = cargo_get_fprint_args(
                        argc,
                        args,
                        1, // Start index.
                        0, // flags
                        CARGO_DEFAULT_MAX_WIDTH,
                        3, // Highlight count.
                        3, "~"CARGO_COLOR_GREEN, 1, "^"CARGO_COLOR_RED, 4, "-");
    cargo_assert(s, "Failed call cargo_get_fprint_args");
    printf("%s\n", s);
    cargo_assert(strstr(s, "~~~~~~"), "Did not find highlight for --beta");
    cargo_assert(strstr(s, "^^^^^^^"), "Did not find highlight for --alpha");
    cargo_assert(strstr(s, "---"), "Did not find highlight for 'def'");
    cargo_assert(strstr(s, "^^^^^^^") < strstr(s, "---"),
        "--beta highlight after --alpha");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START(TEST_positional_argument)
{
    // Here we make sure allocated values get freed on a failed parse.
    int i;
    int j;
    char *args[] = { "program", "--beta", "123", "456" };

    ret |= cargo_add_option(cargo, 0, "alpha", "The alpha", "i", &j);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    printf("alpha = %d\n", j);
    cargo_assert(ret == 0, "Failed to parse positional argument");
    cargo_assert(j == 456, "Expected \"alpha\" to have value 456");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_positional_array_argument)
{
    // Here we make sure allocated values get freed on a failed parse.
    size_t k;
    int i;
    int j[3];
    int j_expect[] = { 456, 789, 101112 };
    size_t j_count = 0;
    char *args[] = { "program", "--beta", "123", "456", "789", "101112" };

    ret |= cargo_add_option(cargo, 0, "alpha", "The alpha", ".[i]#", &j, &j_count, 3);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    for (k = 0; k < j_count; k++)
    {
        printf("alpha = %d\n", j[k]);
    }
    cargo_assert(ret == 0, "Failed to parse positional argument");
    cargo_assert_array(j_count, 3, j, j_expect);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_multiple_positional_array_argument)
{
    size_t k;
    int i;
    int j[3];
    int j_expect[] = { 456, 789, 101112 };
    size_t j_count = 0;
    float m[3];
    float m_expect[] = { 4.3f, 2.3f, 50.34f };
    size_t m_count = 0;
    char *args[] =
    {
        "program",
        "--beta", "123",
        "456", "789", "101112",
        "4.3", "2.3", "50.34"
    };

    ret |= cargo_add_option(cargo, 0, "alpha", "The alpha", ".[i]#",
                            &j, &j_count, 3);
    ret |= cargo_add_option(cargo, 0, "mad", "Mutual Assured Destruction", ".[f]#",
                            &m, &m_count, 3);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);

    for (k = 0; k < j_count; k++)
    {
        printf("alpha = %d\n", j[k]);
    }

    for (k = 0; k < m_count; k++)
    {
        printf("mad = %f\n", m[k]);
    }

    cargo_assert(ret == 0, "Failed to parse positional argument");
    cargo_assert_array(j_count, 3, j, j_expect);
    cargo_assert_array(m_count, 3, m, m_expect);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_multiple_positional_array_argument2)
{
    // Shuffle the order of arguments
    size_t k;
    int i;
    int j[3];
    int j_expect[] = { 456, 789, 101112 };
    size_t j_count = 0;
    float m[3];
    float m_expect[] = { 4.3f, 2.3f, 50.34f };
    size_t m_count = 0;
    char *args[] =
    {
        "program",
        "456", "789", "101112",
        "--beta", "123",
        "4.3", "2.3", "50.34"
    };

    ret |= cargo_add_option(cargo, 0, "alpha", "The alpha", ".[i]#",
                            &j, &j_count, 3);
    ret |= cargo_add_option(cargo, 0, "mad", "Mutual Assured Destruction", ".[f]#",
                            &m, &m_count, 3);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);

    for (k = 0; k < j_count; k++)
    {
        printf("alpha = %d\n", j[k]);
    }

    for (k = 0; k < m_count; k++)
    {
        printf("mad = %f\n", m[k]);
    }

    cargo_assert(ret == 0, "Failed to parse positional argument");
    cargo_assert_array(j_count, 3, j, j_expect);
    cargo_assert(m, "m is NULL");
    cargo_assert_array(m_count, 3, m, m_expect);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_multiple_positional_array_argument3)
{
    // Test with an allocated array of 1 or more positional arguments.
    size_t k;
    int i;
    int j[3];
    int j_expect[] = { 456, 789, 101112 };
    size_t j_count = 0;
    float *m = NULL;
    float m_expect[] = { 4.3f, 2.3f, 50.34f, 0.99f };
    size_t m_count = 0;
    char *args[] =
    {
        "program",
        "456", "789", "101112",
        "--beta", "123",
        "4.3", "2.3", "50.34", "0.99"
    };

    ret |= cargo_add_option(cargo, 0, "alpha", "The alpha", ".[i]#",
                            &j, &j_count, 3);
    ret |= cargo_add_option(cargo, 0, "mad", "Mutual Assured Destruction", "[f]+",
                            &m, &m_count);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);

    printf("j_count = %lu\n", j_count);
    for (k = 0; k < j_count; k++)
    {
        printf("alpha = %d\n", j[k]);
    }

    printf("m_count = %lu\n", m_count);
    for (k = 0; k < m_count; k++)
    {
        printf("mad = %f\n", m[k]);
    }

    cargo_assert(ret == 0, "Failed to parse positional argument");
    cargo_assert_array(j_count, 3, j, j_expect);
    cargo_assert_array(m_count, 4, m, m_expect);

    _TEST_CLEANUP();
    _cargo_xfree(&m);
}
_TEST_END()

_TEST_START_EX(TEST_autoclean_flag, CARGO_AUTOCLEAN)
{
    char *s = NULL;
    char *args[] = { "program", "--alpha", "abc" };

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "s", &s);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse valid option");
    cargo_assert(s && !strcmp(s, "abc"), "Expected --alpha to have value \"abc\"");

    _TEST_CLEANUP();
    // Note! We're not using the normal _TEST_END here so we can assert after
    // the cargo_destroy call. Important not to use cargo_assert after
    // _TEST_CLEANUP. Since it will result in an infinite loop!
    cargo_destroy(&cargo);
    if (s != NULL) return "Expected \"s\" to be freed and NULL";
}
_TEST_END_NODESTROY()

_TEST_START_EX(TEST_autoclean_flag_off, 0)
{
    char *s = NULL;
    char *args[] = { "program", "--alpha", "abc" };

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "s", &s);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse valid option");
    cargo_assert(s && !strcmp(s, "abc"), "Expected --alpha to have value \"abc\"");

    _TEST_CLEANUP();
    cargo_destroy(&cargo);
    if (s == NULL) return "Expected \"s\" to be non-NULL";
    _cargo_free(s);
}
_TEST_END_NODESTROY()

_TEST_START(TEST_parse_zero_or_more_with_args)
{
    int *i = NULL;
    int i_expect[] = { 1, 2 };
    size_t i_count = 0;
    int j = 0;
    char *args[] = { "program", "--beta", "2", "--alpha", "1", "2" };

    cargo_get_flags(cargo);
    cargo_set_flags(cargo, CARGO_AUTOCLEAN);

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "[i]*", &i, &i_count);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &j);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert_array(i_count, 2, i, i_expect);

    _TEST_CLEANUP();
    // Auto cleanup enabled.
}
_TEST_END()

_TEST_START(TEST_parse_zero_or_more_without_args)
{
    int *i = NULL;
    size_t i_count = 0;
    int j = 0;
    char *args[] = { "program", "--beta", "2", "--alpha" };

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "[i]*", &i, &i_count);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &j);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(i == NULL, "Expected i to be NULL");
    cargo_assert(i_count == 0, "Expected i count to be 0");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_zero_or_more_with_positional)
{
    int *pos = NULL;
    int pos_expect[] = { 1, 2 };
    size_t pos_count = 0;
    int *i = NULL;
    int i_expect[] = { 3, 4 };
    size_t i_count = 0;
    int *j = NULL;
    int j_expect[] = { 5, 6 };
    size_t j_count = 0;
    char *args[] = { "program", "1", "2", "--alpha", "3", "4", "--beta", "5", "6" };

    cargo_get_flags(cargo);
    cargo_set_flags(cargo, CARGO_AUTOCLEAN);

    ret |= cargo_add_option(cargo, 0, "pos", "The positional", "[i]*", &pos, &pos_count);
    ret |= cargo_add_option(cargo, 0, "--alpha -a", "The alpha", "[i]*", &i, &i_count);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "[i]*", &j, &j_count);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert_array(pos_count, 2, pos, pos_expect);
    cargo_assert_array(i_count, 2, i, i_expect);
    cargo_assert_array(j_count, 2, j, j_expect);

    _TEST_CLEANUP();
    // Auto cleanup enabled.
}
_TEST_END()

_TEST_START(TEST_required_option_missing)
{
    int i;
    int j;
    char *args[] = { "program", "--beta", "123", "456" };

    ret |= cargo_add_option(cargo, CARGO_OPT_REQUIRED,
                            "--alpha", "The alpha", "i", &j);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_set_internal_usage_flags(cargo, CARGO_USAGE_HIDE_DESCRIPTION);

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succeeded with missing required option");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_required_option)
{
    int i;
    int j;
    char *args[] = { "program", "--beta", "123", "456", "--alpha", "789" };

    ret |= cargo_add_option(cargo, CARGO_OPT_REQUIRED,
                            "--alpha", "The alpha", "i", &j);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed with existing required option");
    cargo_assert(j == 789, "Expected j == 789");

    _TEST_CLEANUP();
}
_TEST_END()

typedef struct _test_data_s
{
    int width;
    int height;
} _test_data_t;

static int _test_cb(cargo_t ctx, void *user, const char *optname,
                    int argc, char **argv)
{
    assert(ctx);
    assert(user);

    _test_data_t *u = (_test_data_t *)user;
    memset(u, 0, sizeof(_test_data_t));

    if (argc > 0)
    {
        if (sscanf(argv[0], "%dx%d", &u->width, &u->height) != 2)
        {
            return -1;
        }

        return 1; // We ate 1 argument.
    }

    return -1;
}

_TEST_START(TEST_custom_callback)
{
    _test_data_t data;
    char *args[] = { "program", "--alpha", "128x64" };

    cargo_set_flags(cargo, CARGO_AUTOCLEAN);

    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "c", _test_cb, &data);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    printf("%dx%d\n", data.width, data.height);
    cargo_assert(data.width == 128, "Width expected to be 128");
    cargo_assert(data.height == 64, "Height expected to be 128");

    _TEST_CLEANUP();
}
_TEST_END()

static int _test_cb_fixed_array(cargo_t ctx, void *user, const char *optname,
                                int argc, char **argv)
{
    int i;
    assert(ctx);
    assert(user);

    _test_data_t *u = (_test_data_t *)user;
    memset(u, 0, sizeof(_test_data_t));

    for (i = 0; i < argc; i++)
    {
        if (sscanf(argv[i], "%dx%d", &u[i].width, &u[i].height) != 2)
        {
            return -1;
        }
    }

    return i;
}

_TEST_START(TEST_custom_callback_fixed_array)
{
    size_t i;
    #define DATA_COUNT 5
    _test_data_t data[DATA_COUNT];
    size_t data_count = 0;
    _test_data_t data_expect[] =
    {
        { 128, 64 },
        { 32, 16 },
        { 24, 32 },
        { 32, 32 },
        { 24, 24 }
    };
    char *args[] = { "program", "--alpha", "128x64", "32x16", "24x32", "32x32", "24x24" };

    cargo_set_flags(cargo, CARGO_AUTOCLEAN);

    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "[c]#",
                            _test_cb_fixed_array, &data, &data_count, DATA_COUNT);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);

    cargo_assert(data_count == DATA_COUNT, "Expected data_count == DATA_COUNT");

    for (i = 0; i < data_count; i++)
    {
        printf("%dx%d == %dx%d\n",
            data[i].width, data[i].height,
            data_expect[i].width, data_expect[i].height);
        cargo_assert(data[i].width == data_expect[i].width, "Unexpected width");
        cargo_assert(data[i].height == data_expect[i].height, "Unexpected height");
    }

    _TEST_CLEANUP();
}
_TEST_END()

typedef struct _test_rect_s
{
    int x;
    int y;
    int w;
    int h;
} _test_rect_t;

static int _test_cb_fixed_array_no_count(cargo_t ctx, void *user,
                                        const char *optname,
                                        int argc, char **argv)
{
    _test_rect_t *rect = (_test_rect_t *)user;

    if (argc != 4)
    {
        cargo_set_error(ctx, 0, "Not enough!");
        return -1;
    }

    rect->x = atoi(argv[0]);
    rect->y = atoi(argv[1]);
    rect->w = atoi(argv[2]);
    rect->h = atoi(argv[3]);

    return argc;
}

_TEST_START(TEST_many_options_custom)
{
    size_t j = 0;
    _test_rect_t rect;
    #define OPT_COUNT 100
    int vals[OPT_COUNT];
    char optname[256];
    char *args[] = { "program", "--help" };

    // This reproduces a bug seen on the raspberry pi where
    // you get a SIGSEV because the internal list for a custom
    // variable would be freed and then target count was dereferenced
    // (which now was freed).
    for (j = 0; j < OPT_COUNT + 1; j++)
    {
        if (j == (OPT_COUNT / 2))
        {
            ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "[c]#",
                            _test_cb_fixed_array_no_count,
                            &rect, NULL, 4);
            cargo_assert(ret == 0, "Failed to add options");
            continue;
        }
        cargo_snprintf(optname, sizeof(optname), "--opto%02d",  j+1);
        ret |= cargo_add_option(cargo, 0, optname, LOREM_IPSUM, "i", &vals[j]);
        cargo_assert(ret == 0, "Failed to add option");
    }

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);

    cargo_print_usage(cargo, 0);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_custom_callback_fixed_array_no_count)
{
    _test_rect_t rect;
    char *args[] = { "program", "--alpha", "1", "2", "3", "4" };

    cargo_set_flags(cargo, CARGO_AUTOCLEAN);

    // We set the count var to NULL. This should work fine!
    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "[c]#",
                            _test_cb_fixed_array_no_count,
                            &rect, NULL, 4);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse");

    cargo_assert(rect.x == 1, "Expected x = 1");
    cargo_assert(rect.y == 2, "Expected y = 2");
    cargo_assert(rect.w == 3, "Expected w = 3");
    cargo_assert(rect.h == 4, "Expected h = 4");

    _TEST_CLEANUP();
}
_TEST_END()

static int _test_cb_array(cargo_t ctx, void *user, const char *optname,
                                int argc, char **argv)
{
    int i;
    assert(ctx);
    assert(user);
    _test_data_t **u = (_test_data_t **)user;
    _test_data_t *data;

    if (!(*u = _cargo_calloc(argc, sizeof(_test_data_t))))
    {
        return -1;
    }

    data = *u;

    for (i = 0; i < argc; i++)
    {
        if (sscanf(argv[i], "%dx%d", &data[i].width, &data[i].height) != 2)
        {
            return -1;
        }
    }

    return i; // How many arguments we ate.
}

_TEST_START(TEST_custom_callback_array)
{
    size_t i;
    _test_data_t *data = NULL;
    size_t data_count = 0;
    _test_data_t data_expect[] =
    {
        { 128, 64 },
        { 32, 16 },
        { 24, 32 },
        { 32, 32 },
        { 24, 24 }
    };
    char *args[] = { "program", "--alpha", "128x64", "32x16", "24x32", "32x32", "24x24" };

    cargo_set_flags(cargo, CARGO_AUTOCLEAN);

    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "[c]*",
                            _test_cb_array, &data, &data_count, DATA_COUNT);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);

    cargo_assert(data_count == 5, "Expected data_count == 5");

    for (i = 0; i < data_count; i++)
    {
        printf("%dx%d == %dx%d\n",
            data[i].width, data[i].height,
            data_expect[i].width, data_expect[i].height);
        cargo_assert(data[i].width == data_expect[i].width, "Unexpected width");
        cargo_assert(data[i].height == data_expect[i].height, "Unexpected height");
    }

    _TEST_CLEANUP();
    _cargo_xfree(&data);
}
_TEST_END()

_TEST_START(TEST_zero_or_more_with_arg)
{
    int i = 0;
    int j = 0;
    char *args[] = { "program", "--alpha", "789", "--beta", "123", "456" };

    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "i?", &j, "3");
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed zero or more args parse");
    cargo_assert(j == 789, "Expected j == 789");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_zero_or_more_without_arg)
{
    int i = 0;
    int j = 5;
    char *args[] = { "program", "--alpha", "--beta", "123", "456" };

    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "i?", &j, "3");
    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed zero or more args parse");
    cargo_assert(j == 3, "Expected j == 3");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_group)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    char *args[] = { "program", "--alpha", "--beta", "123", "456" };
    cargo_set_max_width(cargo, 60);

    ret = cargo_add_group(cargo, 0, "group1", "The Group 1", "This group is 1st");
    ret |= cargo_add_group(cargo, 0, "group2", "The Group 2", LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM);
    cargo_assert(ret == 0, "Failed to add group");

    // Add options to the group using inline method.
    ret |= cargo_add_option(cargo, 0, "   <group1> --alpha", "The alpha", "i?", &j, "3");
    ret |= cargo_add_option(cargo, 0, "<group1> --beta -b", LOREM_IPSUM, "i", &i);

    // Try using the API to add the option to the group.
    ret |= cargo_add_option(cargo, 0, "--centauri", "The alpha centauri", "i?", &k, "4");
    ret |= cargo_group_add_option(cargo, "group1", "--centauri");

    ret |= cargo_add_option(cargo, 0, "<group2> --delta", LOREM_IPSUM LOREM_IPSUM, "i?", &l, "5");
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed zero or more args parse");

    cargo_print_usage(cargo, 0);

    // Make sure the group can be hidden.
    cargo_assert(strstr(cargo_get_usage(cargo, 0), "Group 2") != NULL, "Did not find Group 2");
    cargo_group_set_flags(cargo, "group2", CARGO_GROUP_HIDE);
    cargo_assert(strstr(cargo_get_usage(cargo, 0), "Group 2") == NULL, "Found Group 2");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_many_groups)
{
    size_t i = 0;
    size_t j = 0;
    #define GROUP_COUNT 20
    #define GROUP_OPT_COUNT 20
    int vals[GROUP_COUNT][GROUP_OPT_COUNT];
    char grpname[256];
    char title[256];
    char optname[256];
    char *args[] = { "program", "--optg01o01", "5", "--optg02o02", "123" };

    for (i = 0; i < GROUP_COUNT; i++)
    {
        cargo_snprintf(grpname, sizeof(grpname), "group%d", i+1);
        cargo_snprintf(title, sizeof(title), "The Group%d", i+1);
        ret |= cargo_add_group(cargo, 0, grpname, title, "Group");
        cargo_assert(ret == 0, "Failed to add group");

        for (j = 0; j < GROUP_OPT_COUNT; j++)
        {
            cargo_snprintf(optname, sizeof(optname), "--optg%02do%02d", i+1, j+1);
            ret |= cargo_add_option(cargo, 0, optname, LOREM_IPSUM, "i", &vals[i][j]);
            ret |= cargo_group_add_option(cargo, grpname, optname);
            cargo_assert(ret == 0, "Failed to add option");
        }
    }

    cargo_assert(ret == 0, "Failed to add groups");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed zero or more args parse");
    cargo_assert(vals[0][0] == 5, "Expected --optg01o01 to be 5");
    cargo_assert(vals[1][1] == 123, "Expected --optg02o02 to be 123");

    cargo_print_usage(cargo, 0);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_mutex_group_guard)
{
    int i = 0;
    int j = 0;
    int k = 0;
    char *args[] = { "program", "--alpha", "--beta", "123", "456" };

    ret = cargo_add_mutex_group(cargo, 0, "mutex_group1", NULL, NULL);
    cargo_assert(ret == 0, "Failed to add mutex group");

    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "i?", &j, "3");
    ret |= cargo_mutex_group_add_option(cargo, "mutex_group1", "--alpha");

    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    ret |= cargo_mutex_group_add_option(cargo, "mutex_group1", "--beta");

    ret |= cargo_add_option(cargo, 0, "--centauri", "The centauri", "i?", &k, "3");
    ret |= cargo_mutex_group_add_option(cargo, "mutex_group1", "--centauri");
    cargo_assert(ret == 0, "Failed to add options");

    // We parse args with 2 members of the mutex group.
    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succesfully parsed mutex group with 2 group members");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_mutex_group_require_one)
{
    int i = 0;
    int j = 0;
    int k = 0;
    char *args[] = { "program", "--centauri", "5" };

    ret = cargo_add_mutex_group(cargo,
                                CARGO_MUTEXGRP_ONE_REQUIRED,
                                "mutex_group1", NULL, NULL);
    cargo_assert(ret == 0, "Failed to add mutex group");

    ret |= cargo_add_option(cargo, 0, "--alpha", "The alpha", "i", &j);
    ret |= cargo_mutex_group_add_option(cargo, "mutex_group1", "--alpha");

    ret |= cargo_add_option(cargo, 0, "--beta -b", "The beta", "i", &i);
    ret |= cargo_mutex_group_add_option(cargo, "mutex_group1", "--beta");

    // Don't add this to te mutex group.
    ret |= cargo_add_option(cargo, 0, "--centauri", "The centauri", "i?", &k, "3");
    cargo_assert(ret == 0, "Failed to add options");

    // We parse args with no members of the mutex group.
    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0,
        "Succesfully parsed mutex group with no mutex member when 1 required");
    cargo_assert(k == 5, "Expected k == 5");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_split_commandline)
{
    const char *cmd = "abc def \"ghi jkl\"";
    char *argv_expect[] =
    {
        "abc", "def", "ghi jkl"
    };
    char **argv = NULL;
    int argc = 0;
    int i;
    ret = 0;

    argv = cargo_split_commandline(0, cmd, &argc);
    for (i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    cargo_assert(argv != NULL, "Got NULL argv");
    cargo_assert_str_array((size_t)argc, 3, argv, argv_expect);
    cargo_free_commandline(&argv, argc);

    argv = cargo_split_commandline(0, NULL, &argc);
    cargo_assert(argv == NULL, "Expected NULL argv");

    #ifndef _WIN32
    argv = cargo_split_commandline(0, "cmd not \"bla closed", &argc);
    cargo_assert(argv == NULL, "Invalid commandline parsed as ok");
    #endif

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_missing_last_arg)
{
    int k = 0;
    char *args[] = { "program", "--centauri" };

    ret |= cargo_add_option(cargo, 0, "--centauri", "The centauri", "i", &k);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Succesfully parsed option without argument");

    _TEST_CLEANUP();
}
_TEST_END()

static const char *_cargo_test_verify_usage_length(const char *usage, int width)
{
    char *ret = NULL;
    char **lines = NULL;
    size_t line_count = 0;
    size_t len = 0;
    size_t i = 0;
    assert(usage);
    assert(width);

    printf("%s\n", usage);

    if (!(lines = _cargo_split(usage, "\n", &line_count)))
    {
        return "Failed to split usage";
    }

    printf("Line count: %lu\n", line_count);

    for (i = 0; i < line_count; i++)
    {
        len = strlen(lines[i]);
        printf("%02lu Len: %02lu:%s\n", i, len, lines[i]);

        if ((int)len > width)
        {
            ret = "Got usage line longer than specified";
            break;
        }
    }

    _cargo_free_str_list(&lines, &line_count);
    return ret;
}

_TEST_START(TEST_cargo_set_max_width)
{
    int i;
    int j;
    const char *err = NULL;
    const char *usage = NULL;
    char *args[] = { "program", "--alpha", "789", "--beta", "123" };

    cargo_set_description(cargo, LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM);
    cargo_set_epilog(cargo, LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM);

    ret |= cargo_add_option(cargo, 0, "--alpha", LOREM_IPSUM, "i", &j);
    ret |= cargo_add_option(cargo, 0, "--beta -b", LOREM_IPSUM, "i", &i);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_set_max_width(cargo, 40);
    cargo_assert(cargo_get_width(cargo, 0) == 40, "Unexpected witdth returned");
    usage = cargo_get_usage(cargo, 0);
    cargo_assert(usage != NULL, "Got NULL usage on width 40");
    err = _cargo_test_verify_usage_length(usage, 40);
    cargo_assert(err == NULL, err);

    // Set a size bigger than CARGO_MAX_MAX_WIDTH.
    cargo_set_max_width(cargo, CARGO_MAX_MAX_WIDTH * 2);
    cargo_assert(cargo_get_width(cargo, 0) == CARGO_MAX_MAX_WIDTH,
        "Got greater width than CARGO_MAX_MAX_WIDTH");
    usage = cargo_get_usage(cargo, 0);
    cargo_assert(usage != NULL, "Got NULL usage on width CARGO_MAX_MAX_WIDTH * 2");
    err = _cargo_test_verify_usage_length(usage, CARGO_MAX_MAX_WIDTH);
    cargo_assert(err == NULL, err);

    // Just make sure this doesn't crash.
    // This could return -1, and we cannot assert on any return value
    // since it's specific to the system the test is run on.
    cargo_get_width(cargo, CARGO_WIDTH_RAW);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_snprintf)
{
    char buf[32];
    ret = cargo_snprintf(buf, 0, "%d", 123);
    printf("%d\n", ret);
    cargo_assert(ret == 0, "Expected ret == 0 when given 0 buflen");

    // Let memcheckers make sure this doesn't write past end of buffer
    ret = cargo_snprintf(buf, sizeof(buf), "%s", LOREM_IPSUM);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_set_prefix)
{
    int i;
    int j;
    int k;
    char *args[] = { "program", "--alpha", "789", "++beta", "123" };

    cargo_set_prefix(cargo, "+");

    ret = cargo_add_option(cargo, 0, "--alpha", LOREM_IPSUM, "i", &j);
    cargo_assert(ret != 0, "Succesfully added argument with invalid prefix");

    ret = cargo_add_option(cargo, 0, "centauri c", LOREM_IPSUM, "i", &k);
    cargo_assert(ret != 0, "Expected positional argument to reject alias");

    ret = cargo_add_option(cargo, 0, "++beta +b", LOREM_IPSUM, "i", &i);
    cargo_assert(ret == 0, "Failed to add option with valid prefix '+'");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse with custom prefix");
    cargo_assert(i == 123, "Expected ++beta to have value 123");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_aapendf)
{
    char *s = NULL;
    size_t lbefore = 0;
    size_t lorem_lens = (strlen(LOREM_IPSUM) * 3);
    cargo_astr_t astr;

    // This test is only valid if the test strings causes a realloc.
    assert(lorem_lens > CARGO_ASTR_DEFAULT_SIZE);

    memset(&astr, 0, sizeof(cargo_astr_t));
    astr.s = &s;

    ret = cargo_aappendf(&astr, "%s", "Some short string");
    cargo_assert(ret > 0, "Expected ret > 0");
    lbefore = astr.l;

    // Trigger realloc.
    cargo_aappendf(&astr, "%s%s%s", LOREM_IPSUM, LOREM_IPSUM, LOREM_IPSUM);
    cargo_assert(ret > 0, "Expected ret > 0");
    cargo_assert(astr.offset > lorem_lens, "Expected longer string");
    cargo_assert(astr.l > lbefore, "Expected realloc");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START(TEST_cargo_get_fprint_args)
{
    int i = 0;
    int j = 0;
    char *argv[] = { "program", "first", "second", "--third", "123" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    char *s = NULL;
    int start;
    cargo_highlight_t highlights[] =
    {
        { 1, "#"CARGO_COLOR_YELLOW },
        { 4, "="CARGO_COLOR_GREEN }
    };
    ret = 0;

    start = 0;
    printf("\nStart %d:\n", start);
    printf("----------\n");
    s = cargo_get_fprint_args(argc, argv,
                            start,  // start.
                            0,      // flags.
                            CARGO_DEFAULT_MAX_WIDTH,
                            3,      // highlight_count (how many follows).
                            0, "^"CARGO_COLOR_RED,
                            2 ,"~",
                            4, "*"CARGO_COLOR_CYAN);
    cargo_assert(s != NULL, "Got NULL fprint string");
    printf("%s\n", s);
    for (i = start; i < argc; i++)
    {
        cargo_assert(strstr(s, argv[i]), "Expected to find argv params");
    }
    cargo_assert(strstr(s, "^"), "Expected ^ highlight");
    cargo_assert(strstr(s, CARGO_COLOR_RED), "Expected red color for ^");
    cargo_assert(strstr(s, "~"), "Expected ~ highlight");
    cargo_assert(strstr(s, "*"), "Expected * highlight");
    cargo_assert(strstr(s, CARGO_COLOR_CYAN), "Expected red color for *");
    _cargo_xfree(&s);

    // Other start index.
    start = 1;
    printf("\nStart %d:\n", start);
    printf("----------\n");
    s = cargo_get_fprint_args(argc, argv,
                            start,  // start.
                            0,      // flags.
                            CARGO_DEFAULT_MAX_WIDTH,
                            3,      // highlight_count (how many follows).
                            0, "^"CARGO_COLOR_RED, // Should not be shown.
                            2 ,"~",
                            4, "*"CARGO_COLOR_CYAN);
    cargo_assert(s != NULL, "Got NULL fprint string");
    printf("%s\n", s);
    for (i = start; i < argc; i++)
    {
        cargo_assert(strstr(s, argv[i]), "Expected to find argv params");
    }
    cargo_assert(!strstr(s, "^"), "Expected NO ^ highlight");
    cargo_assert(!strstr(s, CARGO_COLOR_RED), "Expected NO red color for ^");
    cargo_assert(strstr(s, "~"), "Expected ~ highlight");
    cargo_assert(strstr(s, "*"), "Expected red color for *");
    cargo_assert(strstr(s, CARGO_COLOR_CYAN), "Expected red color for *");
    _cargo_xfree(&s);

    // Pass a list instead of var args.
    for (i = 0; i < argc+1; i++)
    {
        start = i;
        printf("\nList start %d:\n", start);
        printf("-------------\n");
        s = cargo_get_fprintl_args(argc, argv,
                                start,  // start.
                                0,      // flags
                                CARGO_DEFAULT_MAX_WIDTH,
                                sizeof(highlights) / sizeof(highlights[0]),
                                highlights);
        printf("%s\n", s);
        for (j = start; j < argc; j++)
        {
            cargo_assert(strstr(s, argv[i]), "Expected to find argv params");
        }

        if (start <= highlights[0].i)
        {
            cargo_assert(strstr(s, "#"), "Expected # highlight");
            cargo_assert(strstr(s, CARGO_COLOR_YELLOW), "Expected yellow color for #");
        }
        else
        {
            cargo_assert(!strstr(s, "#"), "Expected NO # highlight");
            cargo_assert(!strstr(s, CARGO_COLOR_YELLOW), "Expected NO yellow color for #");
        }

        if (start <= highlights[1].i)
        {
            cargo_assert(strstr(s, "="), "Expected = highlight");
            cargo_assert(strstr(s, CARGO_COLOR_GREEN), "Expected red color for =");
        }
        else
        {
            cargo_assert(!strstr(s, "="), "Expected NO = highlight");
            cargo_assert(!strstr(s, CARGO_COLOR_GREEN), "Expected NO red color for =");
        }

        _cargo_xfree(&s);
    }

    ret = cargo_fprintl_args(stdout, argc, argv, 0, 0, CARGO_DEFAULT_MAX_WIDTH,
                            sizeof(highlights) / sizeof(highlights[0]),
                            highlights);
    cargo_assert(ret == 0, "Expected success");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START(TEST_cargo_get_fprint_args_long)
{
    char *s = NULL;
    char **argv = NULL;
    int argc = 0;

    argv = cargo_split_commandline(0, LOREM_IPSUM, &argc);

    cargo_assert(argv, "Got NULL argv");

    s = cargo_get_fprint_args(argc, argv,
                            0,      // start.
                            0,      // flags.
                            CARGO_AUTO_MAX_WIDTH,
                            3,      // highlight_count (how many follows).
                            0, "^"CARGO_COLOR_RED, // Should not be shown.
                            2 ,"~",
                            4, "*"CARGO_COLOR_CYAN);

    cargo_assert(s, "Got NULL string");

    printf("%s\n", s);
    cargo_assert(strstr(s, "^"), "Missing \"^\" highlight");
    cargo_assert(strstr(s, "~"), "Missing \"~\" highlight");
    cargo_assert(strstr(s, "*"), "Missing \"*\" highlight");
    _cargo_free(s);

    s = cargo_get_fprint_args(argc, argv,
                            0,      // start.
                            0,      // flags.
                            150,
                            3,      // highlight_count (how many follows).
                            0, "^"CARGO_COLOR_RED, // Should not be shown.
                            2 ,"~",
                            25, "*"CARGO_COLOR_CYAN);

    cargo_assert(s, "Got NULL string");

    printf("%s\n", s);
    cargo_assert(strstr(s, "^"), "Missing \"^\" highlight");
    cargo_assert(strstr(s, "~"), "Missing \"~\" highlight");
    cargo_assert(!strstr(s, "*"), "Got \"*\" highlight when it should be off screen");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
    cargo_free_commandline(&argv, argc);
}
_TEST_END()

_TEST_START(TEST_cargo_fprintf)
{
    char *s = NULL;
    cargo_fprintf(stdout, "%shej%s poop\n",
        CARGO_COLOR_YELLOW, CARGO_COLOR_RESET);

    cargo_asprintf(&s, "%shej%s\n",
        CARGO_COLOR_YELLOW, CARGO_COLOR_RESET);

    cargo_assert(s, "Got NULL string");
    _cargo_free(s);

    ret = cargo_asprintf(&s, "");
    cargo_assert(ret == 0, "Expected empty string");
    cargo_assert(!strcmp(s, ""), "Expecterd empty string");

    cargo_print_ansicolor(stdout, "Test " CARGO_COLOR_RED "RED" CARGO_COLOR_RESET "\n");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START(TEST_cargo_bool_count)
{
    int i = 0;
    char *args[] = { "program ", "123", "-v", "3", "-v", "-v" };

    ret |= cargo_add_option(cargo, 0,
                            "--verbose -v", LOREM_IPSUM, "b!", &i);

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    printf("i == %d\n", i);
    cargo_assert(i == 3, "Expected i to be 3");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_bool_count_compact)
{
    int i = 0;
    int j;
    char *args[] = { "program ", "-vvv", "123", "-vv" };

    ret |= cargo_add_option(cargo, 0, "args", LOREM_IPSUM, "i", &j);
    ret |= cargo_add_option(cargo, 0, "--verbose -v", LOREM_IPSUM, "b!", &i);

    cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    printf("i == %d\n", i);
    cargo_assert(i == 5, "Expected i to be 5");

    printf("j == %d\n", j);
    cargo_assert(j == 123, "Expected j to be 123");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_fixed_one_or_more_array)
{
    int a[4];
    int a_expect[2] = { 1, 2 };
    char *args[] = { "program", "--beta", "1", "2" };
    _ADD_TEST_FIXED_ARRAY(".[i]+", "%d");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_fixed_zero_or_more_array)
{
    int a[4];
    int a_expect[2] = { 1, 2 };
    char *args[] = { "program", "--beta", "1", "2" };
    _ADD_TEST_FIXED_ARRAY(".[i]*", "%d");
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_fixed_fail_array)
{
    int i;
    int vals[4];
    int vals_expect[] = { 1, 2 };
    size_t val_count = 0;
    char *args[] = { "program ", "--alpha", "1", "2", "--beta", "4" };

    ret |= cargo_add_option(cargo, 0, "--alpha", LOREM_IPSUM,
                            ".[i]#", &vals, &val_count, 4);
    ret |= cargo_add_option(cargo, 0, "--beta", LOREM_IPSUM, "i", &i);

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Parse succeeded when it should fail");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_user_context)
{
    int i = 3;
    cargo_set_context(cargo, &i);
    cargo_assert(&i == cargo_get_context(cargo), "Got invalid user context");
    cargo_assert(i == 3, "Got invalid user context value");

    _TEST_CLEANUP();
}
_TEST_END()

typedef struct test_grp_ctx_s
{
    char name[32];
    int val;
} test_grp_ctx_t;

static int _test_TEST_group_user_context_cb(cargo_t ctx, void *user,
                                const char *optname, int argc, char **argv)
{
    test_grp_ctx_t *grpctx = NULL;
    test_grp_ctx_t *mgrpctx = NULL;
    const char *group = NULL;
    const char **mgroups = NULL;
    size_t mgroup_count = 0;
    cargo_cb_assert(ctx, "NULL context in callback");

    group = cargo_get_option_group(ctx, optname);
    cargo_cb_assert(group, "Got NULL Group name");

    printf("Group context callback %s: %s\n", optname, group);

    grpctx = cargo_get_group_context(ctx, group);
    cargo_cb_assert(grpctx, "Got NULL group context");

    printf("Group context contents: %s = %d\n", grpctx->name, grpctx->val);

    mgroups = cargo_get_option_mutex_groups(ctx, optname, &mgroup_count);

    printf("Mutex group count for %s: %lu\n", optname, mgroup_count);

    if (!strcmp(optname, "--alpha"))
    {
        int *a = (int *)user;
        cargo_cb_assert(a, "Got NULL user context for --alpha");
        *a = atoi(argv[0]);
        printf("a == %d\n", *a);
        cargo_cb_assert(*a == 15, "Got unexpected value for --alpha");
        cargo_cb_assert(!strcmp(group, "group1"), "Expected group1 for --alpha");
        cargo_cb_assert(!strcmp(grpctx->name, "hello"), "Invalid group context name");
        cargo_cb_assert(grpctx->val == 20, "Unexpected group context value");

        cargo_cb_assert(mgroup_count == 0, "Got non-zero mutex group count");
        cargo_cb_assert(mgroups == NULL, "Got non-NULL mutex group");
    }
    else if (!strcmp(optname, "--delta"))
    {
        char **d = (char **)user;
        cargo_cb_assert(d, "Got NULL user context for --delta");
        cargo_cb_assert(*d == NULL, "Got non-NULL char pointer for --delta");
        cargo_cb_assert(!strcmp(group, "group2"), "Expected group2 for --delta");
        cargo_cb_assert(!strcmp(grpctx->name, "world"), "Invalid group context name");

        mgrpctx = cargo_get_mutex_group_context(ctx, mgroups[0]);
        cargo_cb_assert(mgrpctx, "Got NULL mutex group 1 context");
        cargo_cb_assert(!strcmp(mgrpctx->name, "great"), "--delta not member of mutex group 1");
        cargo_cb_assert(mgrpctx->val == 60, "Got unexpected mutex group 1 value");

        *d = _cargo_strdup(argv[0]);
        cargo_cb_assert(!strcmp(*d, "bla"), "Unexpected --delta value");
        cargo_cb_assert(grpctx->val == 40, "Unexpected group context value");
    }
    else
    {
        fprintf(stderr, "Unexpected option: %s\n", optname);
        return -1;
    }

    return argc;
}

_TEST_START(TEST_group_user_context)
{
    test_grp_ctx_t group_ctx1 = { "hello", 20 };
    test_grp_ctx_t group_ctx2 = { "world", 40 };
    test_grp_ctx_t mgroup_ctx1 = { "great", 60 };
    test_grp_ctx_t mgroup_ctx2 = { "work", 80 };
    int a = 0;
    double b = 0.0;
    float c = 0;
    char *d = NULL;
    int e = 123;
    char *args[] = { "program", "--alpha", "15", "-b", "7.0", "-c", "5", "-d", "bla" };

    ret |= cargo_add_mutex_group(cargo, 0, "mgroup1", "Mutex group 1", "Mutex group 1");
    ret |= cargo_set_mutex_group_context(cargo, "mgroup1", &mgroup_ctx1);
    cargo_assert(ret == 0, "Failed to add mutex group 1");

    ret |= cargo_add_mutex_group(cargo, 0, "mgroup2", "Mutex group 2", "Mutex group 2");
    ret |= cargo_set_mutex_group_context(cargo, "mgroup2", &mgroup_ctx2);
    cargo_assert(ret == 0, "Failed to add mutex group 2");

    ret |= cargo_add_group(cargo, 0, "group1", "The Group 1", "This group is 1st");
    ret |= cargo_set_group_context(cargo, "group1", &group_ctx1);
    cargo_assert(ret == 0, "Failed to add group");

    ret |= cargo_add_option(cargo, 0, "<group1> --alpha -a", "Alpha Description",
            "c", _test_TEST_group_user_context_cb, &a);
    ret |= cargo_add_option(cargo, 0, "<group1> --beta -b", "Alpha Description", "d", &b);
    cargo_assert(ret == 0, "Failed to add options");

    ret |= cargo_add_group(cargo, 0, "group2", "The Group 2", LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM);
    ret |= cargo_set_group_context(cargo, "group2", &group_ctx2);
    cargo_assert(ret == 0, "Failed to add group");

    ret |= cargo_add_option(cargo, 0, "<group2> --centauri -c", "Centauri Description", "f", &c);
    ret |= cargo_add_option(cargo, 0, "<!mgroup1, group2> --delta -d", "Delta Description",
            "c", _test_TEST_group_user_context_cb, &d);
    cargo_assert(ret == 0, "Failed to add options");

    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --error -e", "Error Description", "i", &e);
    ret |= cargo_mutex_group_add_option(cargo, "mgroup2", "--error");
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse");

    cargo_assert(a == 15, "a unexpected");
    cargo_assert(b == 7.0, "b unexpected");
    cargo_assert(c == 5.0f, "c unexpected");
    cargo_assert(!strcmp(d, "bla"), "d unexpected");
    cargo_assert(e == 123, "e changed unexpectedly");

    ret = cargo_set_group_context(cargo, NULL, &group_ctx1);
    cargo_assert(ret == 0, "Failed to set group context for default group");

    cargo_assert(cargo_get_group_context(cargo, NULL) == &group_ctx1,
        "Got different group context back for default group");

    ret = cargo_set_group_context(cargo, "unknown", &group_ctx1);
    cargo_assert(ret != 0, "Able to set group context for un-existent group");

    ret = cargo_set_group_context(cargo, "unknown", &group_ctx1);
    cargo_assert(ret != 0, "Able to set group context for un-existent group");

    _TEST_CLEANUP();
    _cargo_xfree(&d);
}
_TEST_END()

_TEST_START(TEST_mutex_group_context_fail)
{
    void *user = NULL;
    test_grp_ctx_t mgroup_ctx1 = { "hello", 20 };
    int i = 3;
    ret |= cargo_add_mutex_group(cargo, 0, "mgroup1", NULL, NULL);
    ret |= cargo_set_mutex_group_context(cargo, "mgroup1", &mgroup_ctx1);
    cargo_assert(ret == 0, "Failed to add mutex group 1");

    user = cargo_get_mutex_group_context(cargo, "mgroup2");
    cargo_assert(user == NULL, "Got non-NULL mutex group context for non-existent group");

    user = cargo_get_mutex_group_context(cargo, "mgroup1");
    cargo_assert(user == &mgroup_ctx1, "Did not get correct mutex group context");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_get_option_mutex_groups)
{
    size_t i;
    size_t count = 0;
    const char **mgroups = NULL;
    int a = 0;

    ret |= cargo_add_mutex_group(cargo, 0, "mgroup1", NULL, NULL);
    cargo_assert(ret == 0, "Failed to add mutex group 1");

    ret |= cargo_add_mutex_group(cargo, 0, "mgroup2", "hello", "world");
    cargo_assert(ret == 0, "Failed to add mutex group 2");

    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --alpha -a", "Error Description", "i", &a);
    ret |= cargo_mutex_group_add_option(cargo, "mgroup2", "--alpha");
    cargo_assert(ret == 0, "Failed to add option");

    mgroups = cargo_get_option_mutex_groups(cargo, "--alpha", &count);
    cargo_assert(mgroups != NULL, "Got NULL mutex group list");
    printf("Mutex group count: %lu\n", count);

    for (i = 0; i < count; i++)
    {
        printf("%lu: \"%s\"\n", i, mgroups[i]);
    }

    cargo_assert(count == 2, "Expected 2 mutex groups");
    cargo_assert(!strcmp(mgroups[0], "mgroup1"), "Mutex group 1 expected");
    cargo_assert(!strcmp(mgroups[1], "mgroup2"), "Mutex group 2 expected");

    // Get it twice, make sure it was cached.
    mgroups = cargo_get_option_mutex_groups(cargo, "--alpha", &count);

    cargo_assert(count == 2, "Expected 2 mutex groups twice");
    cargo_assert(!strcmp(mgroups[0], "mgroup1"), "Mutex group 1 expected");
    cargo_assert(!strcmp(mgroups[1], "mgroup2"), "Mutex group 2 expected");

    // Get non-existent option.
    mgroups = cargo_get_option_mutex_groups(cargo, "--unknown", &count);

    cargo_assert(count == 0, "Expected 0 options");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_invalid_format_char)
{
    int a = 0;
    int *c = NULL;
    size_t c_count = 0;
    int d[4];
    size_t d_count = 0;

    ret |= cargo_add_option(cargo, 0, "--alpha -a", NULL, "p", &a);
    cargo_assert(ret != 0, "Invalid format character allowed");

    ret |= cargo_add_option(cargo, 0, "--beta -b", NULL, "[i", &a);
    cargo_assert(ret != 0, "Impartial format string allowed");

    ret |= cargo_add_option(cargo, 0, "--centauri -c", NULL, "[i],", &c, &c_count);
    cargo_assert(ret != 0, "Invalid format string allowed");

    ret |= cargo_add_option(cargo, 0, "--delta -d", NULL, ".[i],", &d, &d_count);
    cargo_assert(ret != 0, "Invalid format string allowed");

    ret |= cargo_add_option(cargo, 0, "--error -e", NULL, "im", &a);
    cargo_assert(ret != 0, "Invalid format string allowed");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_option_target_null)
{
    int a = 0;
    int *c = NULL;
    size_t c_count = 0;
    int d[4];
    size_t d_count = 0;

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL, "i", NULL);
    cargo_assert(ret != 0, "NULL target was allowed");

    ret = cargo_add_option(cargo, 0, "--centauri -c", NULL, "[i]+", &c, NULL);
    cargo_assert(ret != 0, "NULL target count was allowed");

    ret = cargo_add_option(cargo, 0, "--delta -d", NULL, "[i]#", &d, NULL, 4);
    cargo_assert(ret != 0, "NULL target count was allowed");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_autoclean_string_list)
{
    size_t i;
    char **strs = NULL;
    size_t str_count = 0;
    char *args[] = { "program", "--alpha", "bla", "blo", "bli" };

    cargo_set_flags(cargo, CARGO_AUTOCLEAN);

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL, "[s]+", &strs, &str_count);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse");
    cargo_assert(strs != NULL, "Got NULL string list");

    printf("strs: %p\n", strs);

    for (i = 0; i < str_count; i++)
    {
        printf("%s\n", strs[i]);
    }

    _TEST_CLEANUP();
}
_TEST_END()

static int _test_fail_callback(cargo_t ctx, void *user, const char *optname,
                                int argc, char **argv)
{
    return -1;
}

_TEST_START(TEST_fail_custom_callback)
{
    size_t i = 0;
    char *args[] = { "program", "-a", "bla" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL, "c", _test_fail_callback, &i);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Parse expected to fail");

    _TEST_CLEANUP();
}
_TEST_END()

static int _test_mutex_group_usage_add_options(cargo_t cargo,
                cargo_mutex_group_flags_t flags1,
                cargo_mutex_group_flags_t flags2,
                int *a, int *b, int *c, int *d)
{
    int ret = 0;
    ret |= cargo_add_mutex_group(cargo, flags1, "mgroup1", "Mutex group 1",
            "MGROUP1_DESCRIPTION " LOREM_IPSUM);
    ret |= cargo_add_mutex_group(cargo, flags2, "mgroup2", "Mutex group2",
            "MGROUP2_DESCRIPTION " LOREM_IPSUM LOREM_IPSUM);

    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --alpha -a", "Error Description", "i", a);
    ret |= cargo_mutex_group_add_option(cargo, "mgroup2", "--alpha");

    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --beta -b", LOREM_IPSUM, "i", b);
    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --centauri -c", "bla bla" LOREM_IPSUM, "i", c);
    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --delta -d", "bla bla" LOREM_IPSUM, "i", d);
    ret |= cargo_mutex_group_add_option(cargo, "mgroup2", "--delta");

    ret |= cargo_add_option(cargo, 0, "--error -e", "AAWESOME" LOREM_IPSUM, "i", d);
    ret |= cargo_add_option(cargo, 0, "--fail -f", "Fail " LOREM_IPSUM, "i", d);

    return ret;
}

_TEST_START(TEST_mutex_group_usage)
{
    int a;
    int b;
    int c;
    int d;
    const char *s = NULL;

    ret = _test_mutex_group_usage_add_options(cargo, 0, 0, &a, &b, &c, &d);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_print_usage(cargo, 0);

    s = cargo_get_usage(cargo, 0);
    cargo_assert(s, "Got NULL usage");
    cargo_assert(!strstr(s, "MGROUP1_DESCRIPTION"),
        "MGROUP2_DESCRIPTION should not show by default");
    cargo_assert(!strstr(s, "MGROUP2_DESCRIPTION"),
        "MGROUP2_DESCRIPTION should not show by default");
    cargo_assert(strstr(s, "--alpha"), "Missing --alpha");
    cargo_assert(strstr(s, "--beta"), "Missing --beta");
    cargo_assert(strstr(s, "--centauri"), "Missing --centauri");
    cargo_assert(strstr(s, "--delta"), "Missing --delta");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_mutex_group_usage2)
{
    int a;
    int b;
    int c;
    int d;
    const char *s = NULL;

    ret = _test_mutex_group_usage_add_options(cargo,
            CARGO_MUTEXGRP_ONE_REQUIRED |
            CARGO_MUTEXGRP_NO_GROUP_SHORT_USAGE |
            CARGO_MUTEXGRP_GROUP_USAGE,
            0, &a, &b, &c, &d);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_print_usage(cargo, 0);

    s = cargo_get_usage(cargo, 0);
    cargo_assert(s, "Got NULL usage");
    cargo_assert(strstr(s, "MGROUP1_DESCRIPTION"),
        "MGROUP1_DESCRIPTION should be shown when grouped");
    cargo_assert(!strstr(s, "MGROUP2_DESCRIPTION"),
        "MGROUP2_DESCRIPTION should not show by default");

    s = cargo_get_usage(cargo, CARGO_USAGE_SHORT);
    cargo_assert(strstr(s, "--alpha"), "Missing --alpha");
    cargo_assert(strstr(s, "--beta"), "Missing --beta");
    cargo_assert(strstr(s, "--centauri"), "Missing --centauri");
    cargo_assert(strstr(s, "--delta"), "Missing --delta");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_mutex_group_usage3)
{
    int a;
    int b;
    int c;
    int d;
    const char *s = NULL;

    ret = _test_mutex_group_usage_add_options(cargo,
            CARGO_MUTEXGRP_ONE_REQUIRED |
            CARGO_MUTEXGRP_NO_GROUP_SHORT_USAGE |
            CARGO_MUTEXGRP_GROUP_USAGE,
            CARGO_MUTEXGRP_GROUP_USAGE |
            CARGO_MUTEXGRP_RAW_DESCRIPTION, &a, &b, &c, &d);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_print_usage(cargo, 0);

    s = cargo_get_usage(cargo, 0);
    cargo_assert(strstr(s, "MGROUP1_DESCRIPTION"),
        "MGROUP1_DESCRIPTION should be shown when grouped");
    cargo_assert(strstr(s, "MGROUP2_DESCRIPTION"),
        "MGROUP2_DESCRIPTION should be shown when grouped");

    s = cargo_get_usage(cargo, CARGO_USAGE_SHORT);
    cargo_assert(strstr(s, "--alpha"), "Missing --alpha");
    cargo_assert(strstr(s, "--beta"), "Missing --beta");
    cargo_assert(strstr(s, "--centauri"), "Missing --centauri");
    cargo_assert(strstr(s, "--delta"), "Missing --delta");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_mutex_group_usage_set_metavar)
{
    int a;
    int b;
    int c;
    int d;
    const char *s = NULL;

    ret = _test_mutex_group_usage_add_options(cargo,
            CARGO_MUTEXGRP_ONE_REQUIRED |
            CARGO_MUTEXGRP_GROUP_USAGE,
            CARGO_MUTEXGRP_GROUP_USAGE |
            CARGO_MUTEXGRP_RAW_DESCRIPTION, &a, &b, &c, &d);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_mutex_group_set_metavar(cargo, "mgroup1", "MUTEX");
    cargo_assert(ret == 0, "Failed to set mutex group metavar");

    cargo_print_usage(cargo, 0);

    s = cargo_get_usage(cargo, CARGO_USAGE_SHORT);
    cargo_assert(s, "Got NULL short usage");
    cargo_assert(strstr(s, "MUTEX"), "Expected to find mutex group metavar");
    cargo_assert(!strstr(s, "--beta"), "--beta should be hidden");
    cargo_assert(!strstr(s, "--centauri"), "--centauri should be hidden");

    // Try to set for non-existing mutex group.
    ret = cargo_mutex_group_set_metavar(cargo, "mgroup3", "MUTEX");
    cargo_assert(ret != 0, "Succeeded to set metavar on invalid mutex group");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_dummy_callback)
{
    size_t i = 0;
    char *args[] = { "program", "-a", "bla" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL, "D");
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_group_add_missing_group)
{
    int i = 0;
    int *j = NULL;
    size_t j_count = 0;
    char *args[] = { "program", "-a", "bla" };

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL, "i", &i);
    cargo_assert(ret == 0, "Failed to add option");

    // Add to non-existent group.
    ret = cargo_group_add_option(cargo, "group1", "--alpha");
    cargo_assert(ret != 0, "Able to add option to non-existent group");

        // Add non-existent option to non-existent group.
    ret = cargo_group_add_option(cargo, "group1", "--beta");
    cargo_assert(ret != 0, "Able to add non-existent option to non-existent group");

    ret = cargo_add_group(cargo, CARGO_GROUP_RAW_DESCRIPTION,
            "group1", "The Group 1", "This group is 1st");
    cargo_assert(ret == 0, "Failed to add group");

    // Add non-existent option to group.
    ret = cargo_group_add_option(cargo, "group1", "--beta");
    cargo_assert(ret != 0, "Able to add non-existent option to group");

    ret = cargo_add_group(cargo, 0, "group2", "The Group 2", "This group is 2nd");
    cargo_assert(ret == 0, "Failed to add second group");

    ret = cargo_group_add_option(cargo, "group1", "--alpha");
    cargo_assert(ret == 0, "Failed to add option to first group");
    ret = cargo_group_add_option(cargo, "group2", "--alpha");
    cargo_assert(ret != 0, "Able to add option to second group");

    ret = cargo_add_option(cargo, 0, "<group1> --beta -b", LOREM_IPSUM, "i", &i);
    cargo_assert(ret == 0, "Failed to add option");

    cargo_print_usage(cargo, 0);

    ret = cargo_add_option(cargo, 0, "<group1 --centauri -c", LOREM_IPSUM, "i", &i);
    cargo_assert(ret != 0, "Succesfully added invalid option with invalid group");

    ret = cargo_add_option(cargo, 0, "", LOREM_IPSUM, "i", &i);
    cargo_assert(ret != 0, "Succesfully added option with empty name");

    ret = cargo_add_option(cargo, 0, "<group1>", LOREM_IPSUM, "i", &i);
    cargo_assert(ret != 0, "Succesfully added option with empty name");

    ret = cargo_add_option(cargo, 0, "--delta", LOREM_IPSUM, "[i]#", &j, &j_count, -5);
    cargo_assert(ret != 0, "Succesfully added option with nargs = -5");

    ret = cargo_add_option(cargo, 0, "--unicorn u", LOREM_IPSUM, "i", &i);
    cargo_assert(ret != 0, "Succesfully added option with invalid alias");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_bool_acc_or)
{
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    unsigned int expected_bits = 0;
    char *args[] = { "program", "-a", "-b", "-b", "123", "-ccc", "-b"};

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL,
            "b|", &a, 1, (1 << 1));
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--beta -b", NULL,
            "b|", &b, 2, (1 << 1), (1 << 2));
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--centauri -c", NULL,
            "b|", &c, 3, (1 << 1), (1 << 2), (1 << 5));
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    expected_bits = (1 << 1);
    printf("a = Expect: 0x%x Got: 0x%x\n", expected_bits, a);
    cargo_assert((a & expected_bits) == expected_bits, "Expected bit 1 to be set");
    cargo_assert((a ^ expected_bits) == 0, "Expected no other bit than 1 to be set");

    // We have 3 occurrances of "-b" but only 2 bits should still be set.
    expected_bits = (1 << 1) | (1 << 2);
    printf("b = Expect: 0x%x Got: 0x%x\n", expected_bits, b);
    cargo_assert((b & (expected_bits)) == expected_bits, "Expected bit 1 and 2 to be set");
    cargo_assert((b ^ (expected_bits)) == 0, "Expected no other bit than 1 and 2 to be set");

    expected_bits = (1 << 1) | (1 << 2) | (1 << 5);
    printf("c = Expect: 0x%x Got: 0x%x\n", expected_bits, c);
    cargo_assert((c & expected_bits) == expected_bits, "Expected bit 1, 2 and 5 to be set");
    cargo_assert((c ^ expected_bits) == 0, "Expected no other bit than 1, 2 and 5 to be set");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_bool_acc_and)
{
    unsigned int a = -1;
    unsigned int b = -1;
    unsigned int c = -1;
    unsigned int expected_bits = 0;
    char *args[] = { "program", "-a", "-b", "-b", "123", "-ccc", "-b"};

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL,
            "b&", &a, 1, (1 << 1));
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--beta -b", NULL,
            "b&", &b, 2, ((1 << 1) | (1 << 2) | (1 << 3)), (1 << 2));
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--centauri -c", NULL,
            "b&", &c, 2, (1 << 1) | (1 << 2) | (1 << 5),  (1 << 1) | (1 << 5));
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    expected_bits = (1 << 1);
    printf("a = Expect: 0x%x Got: 0x%x\n", expected_bits, a);
    cargo_assert((a & expected_bits) == expected_bits, "Expected no bit to be set");
    cargo_assert((a ^ expected_bits) == 0, "Expected no other bit than 1 to be set");

    expected_bits = (1 << 2);
    printf("b = Expect: 0x%x Got: 0x%x\n", expected_bits, b);
    cargo_assert((b & (expected_bits)) == expected_bits, "Expected bit 2 to be set");
    cargo_assert((b ^ (expected_bits)) == 0, "Expected no other bit than 2 to be set");

    expected_bits = (1 << 1) | (1 << 5);
    printf("c = Expect: 0x%x Got: 0x%x\n", expected_bits, c);
    cargo_assert((c & expected_bits) == expected_bits, "Expected bit 1 and 5 to be set");
    cargo_assert((c ^ expected_bits) == 0, "Expected no other bit than 1 and 5 to be set");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_bool_acc_plus)
{
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    char *args[] = { "program", "-a", "-b", "-b", "123", "-ccc", "-b"};

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL,
            "b+", &a, 1, 1);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--beta -b", NULL,
            "b+", &b, 2, 1, 2);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--centauri -c", NULL,
            "b+", &c, 3, 1, 2, 3);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    printf("a = Expect: %u Got: %u\n", 1, a);
    cargo_assert(a == 1, "Expected a == 1");

    printf("b = Expect: %u Got: %u\n", (1 + 2), b);
    cargo_assert(b == (1 + 2), "Expected b == 3");

    printf("c = Expect: %u Got: %u\n", (1 + 2 + 3), c);
    cargo_assert(c == (1 + 2 + 3), "Expected c == 6");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_bool_acc_store)
{
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    char *args[] = { "program", "-a", "-b", "-b", "123", "-ccc", "-b"};

    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL,
            "b_", &a, 1, 40);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--beta -b", NULL,
            "b_", &b, 2, 33, 44);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--centauri -c", NULL,
            "b_", &c, 3, 11, 22, 33);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    printf("a = Expect: %u Got: %u\n", 40, a);
    cargo_assert(a == 40, "Expected a == 40");

    printf("b = Expect: %u Got: %u\n", 44, b);
    cargo_assert(b == 44, "Expected b == 44");

    printf("c = Expect: %u Got: %u\n", 33, c);
    cargo_assert(c == 33, "Expected c == 33");

    _TEST_CLEANUP();
}
_TEST_END()

static char *_test_mutex_order_group(cargo_t cargo, cargo_mutex_group_flags_t flags,
                                     int args1_expect, int args2_expect)
{
    char *msg = NULL;
    int ret = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    char *args1[] = { "program", "--alpha", "--beta", "--centauri", "--error" };
    char *args2[] = { "program", "--fail", "--beta", "--centauri", "--alpha", "--error" };

    ret = cargo_add_mutex_group(cargo, flags, "mgroup1", "Mutex group 1",
            "MGROUP1_DESCRIPTION " LOREM_IPSUM);

    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --alpha -a", "Error Description", "b", &a);

    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --beta -b", LOREM_IPSUM, "b", &b);
    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --centauri -c", "bla bla" LOREM_IPSUM, "b", &c);
    ret |= cargo_add_option(cargo, 0, "<!mgroup1> --delta -d", "bla bla" LOREM_IPSUM, "b", &d);

    ret |= cargo_add_option(cargo, 0, "--error -e", "AAWESOME" LOREM_IPSUM, "b", &d);
    ret |= cargo_add_option(cargo, 0, "--fail -f", "Fail " LOREM_IPSUM, "b", &d);

    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args1) / sizeof(args1[0]), args1);
    cargo_assert(ret == args1_expect, "Unexpected result on parse");

    ret = cargo_parse(cargo, 0, 1, sizeof(args2) / sizeof(args2[0]), args2);
    cargo_assert(ret == args2_expect, "Unexpected result on parse");
fail:
    return msg;
}

_TEST_START(TEST_mutex_order_group_before)
{
    printf("Test order before:\n");
    msg = _test_mutex_order_group(cargo,
            CARGO_MUTEXGRP_ORDER_BEFORE, CARGO_PARSE_MUTEX_CONFLICT_ORDER, 0);
}
_TEST_END()

_TEST_START(TEST_mutex_order_group_after)
{
    printf("Test order after:\n");
    msg = _test_mutex_order_group(cargo,CARGO_MUTEXGRP_ORDER_AFTER, 0,
            CARGO_PARSE_MUTEX_CONFLICT_ORDER);
}
_TEST_END()

_TEST_START(TEST_medium_length_usage)
{
    ret |= cargo_add_option(cargo, 0, "--alpha",
                            "Hello there\n"
                            "This isn't very long!", "D");
    cargo_assert(ret == 0, "Failed to add options");
    cargo_print_usage(cargo, 0);

    cargo_assert(strstr(cargo_get_usage(cargo, 0), "   This isn't"),
                "Medium length usage not indented properly");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_get_unknown_copy)
{
    size_t unknown_count = 0;
    char **unknowns = NULL;
    int a = 0;
    char *args[] = { "--beta", "--alpha", "--centauri", "--delta" };
    char *unknown_expect[] = { "--beta", "--centauri", "--delta" };

    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "b", &a);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 0, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == -1, "Expected fail on unknown options");

    cargo_assert(cargo_get_error(cargo) != NULL, "Got NULL error");

    unknowns = cargo_get_unknown_copy(cargo, &unknown_count);

    cargo_assert_str_array(unknown_count,
        sizeof(unknown_expect) / sizeof(unknown_expect[0]),
        unknowns, unknown_expect);

    _TEST_CLEANUP();
    _cargo_free_str_list(&unknowns, &unknown_count);
}
_TEST_END()

_TEST_START(TEST_cargo_get_args_copy)
{
    size_t extra_args_count = 0;
    char **extra_args = NULL;
    int a = 0;
    char *args[] = { "--beta", "123", "--alpha", "--centauri", "beep", "--delta" };
    char *extra_expect[] = { "--beta", "123", "--centauri", "beep", "--delta" };

    cargo_set_flags(cargo, CARGO_NO_FAIL_UNKNOWN);

    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "b", &a);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 0, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse");

    extra_args = cargo_get_args_copy(cargo, &extra_args_count);

    cargo_assert_str_array(extra_args_count,
        sizeof(extra_expect) / sizeof(extra_expect[0]),
        extra_args, extra_expect);

    _TEST_CLEANUP();
    _cargo_free_str_list(&extra_args, &extra_args_count);
}
_TEST_END()

_TEST_START(TEST_cargo_set_option_description)
{
    int a = 0;
    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "b", &a);
    cargo_assert(ret == 0, "Failed to create option");

    ret = cargo_set_option_description(cargo, "--alpha",
            "%s world %d", "hello", 123);
    cargo_assert(ret == 0, "Failed to set description");

    cargo_print_usage(cargo, 0);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_parse_stop)
{
    size_t extra_args_count = 0;
    char **extra_args = NULL;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    char *args[] = { "--beta", "123", "--alpha", "--centauri", "beep", "--delta" };
    // We stop parsing here ---------------------^
    char *extra_expect[] = { "123", "beep", "--delta" };

    cargo_set_flags(cargo, 0);

    ret |= cargo_add_option(cargo, 0, "--alpha", "an option", "b", &a);
    ret |= cargo_add_option(cargo, 0, "--beta", "an option", "b", &b);
    ret |= cargo_add_option(cargo, CARGO_OPT_STOP, "--centauri", "an option", "b", &c);
    ret |= cargo_add_option(cargo, 0, "--delta", "an option", "b", &d);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 0, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse");

    extra_args = cargo_get_args_copy(cargo, &extra_args_count);

    cargo_assert_str_array(extra_args_count,
        sizeof(extra_expect) / sizeof(extra_expect[0]),
        extra_args, extra_expect);

    _TEST_CLEANUP();
    _cargo_free_str_list(&extra_args, &extra_args_count);
}
_TEST_END()

_TEST_START(TEST_parse_stop_hard)
{
    // The difference in this test is that
    // --delta is required, but should be skipped.

    // TODO: This is a copy paste of the test above, split up into a common function.
    size_t extra_args_count = 0;
    char **extra_args = NULL;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    char *args[] = { "--beta", "123", "--alpha", "--centauri", "beep", "--delta" };
    // We stop parsing here ---------------------^
    char *extra_expect[] = { "123", "beep", "--delta" };

    cargo_set_flags(cargo, 0);

    ret |= cargo_add_option(cargo, 0, "--alpha", "an option", "b", &a);
    ret |= cargo_add_option(cargo, 0, "--beta", "an option", "b", &b);

    // This should make it so that --delta isn't required.
    ret |= cargo_add_option(cargo, CARGO_OPT_STOP | CARGO_OPT_STOP_HARD,
                            "--centauri", "an option", "b", &c);
    ret |= cargo_add_option(cargo, CARGO_OPT_REQUIRED,
                            "--delta", "an option", "b", &d);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 0, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed to parse");

    extra_args = cargo_get_args_copy(cargo, &extra_args_count);

    cargo_assert_str_array(extra_args_count,
        sizeof(extra_expect) / sizeof(extra_expect[0]),
        extra_args, extra_expect);

    _TEST_CLEANUP();
    _cargo_free_str_list(&extra_args, &extra_args_count);
}
_TEST_END()

_TEST_START(TEST_parse_stop_hard_fail)
{
    // The difference in this test is that
    // --delta is required, and we should fail.

    // TODO: This is a copy paste of the test above, split up into a common function.
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    char *args[] = { "--beta", "123", "--alpha", "--centauri", "beep", "--delta" };
    // We stop parsing here ---------------------^

    cargo_set_flags(cargo, 0);

    ret |= cargo_add_option(cargo, 0, "--alpha", "an option", "b", &a);
    ret |= cargo_add_option(cargo, 0, "--beta", "an option", "b", &b);

    // This should make it so that --delta isn't required.
    ret |= cargo_add_option(cargo, CARGO_OPT_STOP,
                            "--centauri", "an option", "b", &c);
    ret |= cargo_add_option(cargo, CARGO_OPT_REQUIRED,
                            "--delta", "an option", "b", &d);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 0, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret < 0, "Succeeded to parse when expected to fail");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_zero_or_one_with_arg)
{
    char *args[] = { "program", "--alpha", "6" };
    int a = 0;
    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "i?", &a, "5");
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed parse");
    cargo_assert(a == 6, "Unexpected value");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_zero_or_one_without_arg)
{
    char *args[] = { "program", "--alpha" };
    int a = 0;
    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "i?", &a, "5");
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed parse");
    cargo_assert(a == 5, "Unexpected value");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_zero_or_one_without_arg2)
{
    char *args[] = { "program", "--alpha", "--beta", "3" };
    int a = 0;
    int b = 0;
    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "i?", &a, "5");
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Failed parse");
    cargo_assert(a == 5, "Unexpected value");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_late_unknown_options)
{
    char *args[] = { "program", "--alpha", "123", "--centauri", "--beta", "3" };
    int a = 0;
    int b = 0;
    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "i", &a);
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Parse success on unknown option (late)");

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    cargo_assert(a == 123, "a != 123 after unknown option (check after)");
    cargo_assert(b == 3,   "b != 3 after unknown option (check after)");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_early_unknown_options)
{
    char *args[] = { "program", "--alpha", "123", "--centauri", "--beta", "3" };
    int a = 0;
    int b = 0;

    cargo_set_flags(cargo, CARGO_UNKNOWN_EARLY);

    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "i", &a);
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret != 0, "Parse success on unknown option (early)");

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    cargo_assert(a == 0, "a != 0, when early unknown option");
    cargo_assert(b == 0, "b != 0, when early unknown option");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_late_unknown_options_no_fail)
{
    char *args[] =
    {
        "program", "--alpha", "123", "--centauri", "--delta", "--beta", "3"
    };
    char *expected[] = { "--centauri", "--delta" };
    size_t expected_count = sizeof(expected) / sizeof(expected[0]);
    int a = 0;
    int b = 0;
    size_t i = 0;
    const char **unknowns = NULL;
    size_t unknown_count = 0;
    char **unknowns_cpy = NULL;
    size_t unknown_count_cpy = 0;

    cargo_set_flags(cargo, CARGO_NO_FAIL_UNKNOWN);

    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "i", &a);
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failure on unknown option (late, no fail)");

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    cargo_assert(a == 123, "a != 123 after unknown option (check after)");
    cargo_assert(b == 3,   "b != 3 after unknown option (check after)");

    unknowns = cargo_get_unknown(cargo, &unknown_count);
    cargo_assert(unknowns, "Got NULL unknowns list");
    for (i = 0; i < unknown_count; i++) printf("%s\n", unknowns[i]);
    cargo_assert_str_array(unknown_count, expected_count, unknowns, expected);

    unknowns_cpy = cargo_get_unknown_copy(cargo, &unknown_count_cpy);
    cargo_assert(unknowns_cpy, "Got NULL unknowns list (copy)");
    for (i = 0; i < unknown_count_cpy; i++) printf("%s\n", unknowns_cpy[i]);
    cargo_assert_str_array(unknown_count_cpy, expected_count, unknowns_cpy, expected);

    _TEST_CLEANUP();
    cargo_free_commandline(&unknowns_cpy, unknown_count_cpy);
}
_TEST_END()

_TEST_START(TEST_late_unknown_options_no_fail_stop)
{
    char *args[] =
    {
        "program", "--alpha", "123", "--centauri", "--delta", "--beta", "3",
        "--bla", "--blo", "--many", "--other", "options"
    };
    char *expected[] = { "--centauri", "--delta" };
    size_t expected_count = sizeof(expected) / sizeof(expected[0]);
    int a = 0;
    int b = 0;
    size_t i = 0;
    const char **unknowns = NULL;
    size_t unknown_count = 0;
    char **unknowns_cpy = NULL;
    size_t unknown_count_cpy = 0;

    cargo_set_flags(cargo, CARGO_NO_FAIL_UNKNOWN);

    ret = cargo_add_option(cargo, 0, "--alpha", "an option", "i", &a);
    ret = cargo_add_option(cargo, CARGO_OPT_STOP, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failure on unknown option (late, no fail)");

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    cargo_assert(a == 123, "a != 123 after unknown option (check after)");
    cargo_assert(b == 3,   "b != 3 after unknown option (check after)");

    unknowns = cargo_get_unknown(cargo, &unknown_count);
    cargo_assert(unknowns, "Got NULL unknowns list");
    for (i = 0; i < unknown_count; i++) printf("%s\n", unknowns[i]);
    cargo_assert_str_array(unknown_count, expected_count, unknowns, expected);

    unknowns_cpy = cargo_get_unknown_copy(cargo, &unknown_count_cpy);
    cargo_assert(unknowns_cpy, "Got NULL unknowns list (copy)");
    for (i = 0; i < unknown_count_cpy; i++) printf("%s\n", unknowns_cpy[i]);
    cargo_assert_str_array(unknown_count_cpy, expected_count, unknowns_cpy, expected);

    _TEST_CLEANUP();
    cargo_free_commandline(&unknowns_cpy, unknown_count_cpy);
}
_TEST_END()

_TEST_START(TEST_cargo_set_error)
{
    cargo_set_error(cargo, 0, "Hello %s\n", "world");
    cargo_assert(!strcmp(cargo_get_error(cargo), "Hello world\n"), "Unexpected string");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_set_error_append)
{
    cargo_set_error(cargo, 0, "Hello ");
    cargo_assert(!strcmp(cargo_get_error(cargo), "Hello "), "Unexpected string");
    cargo_set_error(cargo, CARGO_ERR_APPEND, "world\n");
    cargo_assert(!strcmp(cargo_get_error(cargo), "Hello world\n"), "Unexpected string");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_set_error_append2)
{
    cargo_set_error(cargo, CARGO_ERR_APPEND, "Hello ");
    cargo_assert(!strcmp(cargo_get_error(cargo), "Hello "), "Unexpected string");
    cargo_set_error(cargo, CARGO_ERR_APPEND, "world\n");
    cargo_assert(!strcmp(cargo_get_error(cargo), "Hello world\n"), "Unexpected string");
    cargo_set_error(cargo, 0, "The other\n");
    cargo_assert(!strcmp(cargo_get_error(cargo), "The other\n"), "Unexpected string");

    _TEST_CLEANUP();
}
_TEST_END()


_TEST_START(TEST_cargo_set_memfunctions)
{
    int i;
    const char *s = NULL;
    cargo_set_memfunctions(_cargo_test_malloc, _cargo_test_realloc, free);

    _cargo_test_set_malloc_fail_count(0);
    _cargo_test_set_realloc_fail_count(0);
    s = TEST_add_integer_option();
    cargo_assert(s == NULL, "Got unexpected mem error");

    for (i = 1; i < 20; i++)
    {
        _cargo_test_set_malloc_fail_count(i);
        s = TEST_add_integer_option();
        cargo_assert(s != NULL, "Did not get expected mem error");
    }

    _cargo_test_set_malloc_fail_count(0);
    _cargo_test_set_realloc_fail_count(0);
    s = TEST_many_groups();
    cargo_assert(s == NULL, "Got unexpected mem error");

    for (i = 1; i < 20; i++)
    {
        _cargo_test_set_malloc_fail_count(i);
        s = TEST_many_groups();
        cargo_assert(s != NULL, "Did not get expected mem error TEST_many_groups");
    }

    for (i = 1; i < 20; i++)
    {
        _cargo_test_set_realloc_fail_count(i);
        s = TEST_many_groups();
        cargo_assert(s != NULL, "Did not get expected mem error TEST_many_groups");
    }


    _TEST_CLEANUP();
    cargo_set_memfunctions(NULL, NULL, NULL);
    _cargo_test_set_malloc_fail_count(0);
    _cargo_test_set_realloc_fail_count(0);
}
_TEST_END()

_TEST_START(TEST_cargo_malloc_zero_bytes)
{
    cargo_set_memfunctions(_cargo_test_malloc, NULL, NULL);
    _cargo_test_set_malloc_fail_count(0);

    cargo_assert(_cargo_malloc(0) == NULL, "Did not get expected error");
    cargo_assert(malloc_current == 0, "Unexpected call to malloc");

    _TEST_CLEANUP();
    cargo_set_memfunctions(NULL, NULL, NULL);
    _cargo_test_set_malloc_fail_count(0);
}
_TEST_END()

_TEST_START(TEST_test_hidden_option)
{
    char *args[] = { "program", "--alpha", "123", "--centauri", "--beta", "3" };
    int a = 0;
    int b = 0;
    const char *usage = NULL;
    ret = cargo_add_option(cargo, CARGO_OPT_HIDE, "--alpha", "the hidden", "i", &a);
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    usage = cargo_get_usage(cargo, 0);
    printf("%s\n", usage);

    cargo_assert(strstr(usage, "--alpha") == NULL,
        "Found hidden option in usage");

    cargo_assert(strstr(usage, "the hidden") == NULL,
        "Found hidden option description in usage");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_test_hidden_short_option)
{
    char *args[] = { "program", "--alpha", "123", "--centauri", "--beta", "3" };
    int a = 0;
    int b = 0;
    const char *usage = NULL;
    ret = cargo_add_option(cargo, CARGO_OPT_HIDE_SHORT, "--alpha", "the hidden", "i", &a);
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    // Only hide in short usage.
    usage = cargo_get_usage(cargo, CARGO_USAGE_SHORT);
    printf("%s\n", usage);

    cargo_assert(strstr(usage, "--alpha") == NULL,
        "Found hidden short option in short usage");

    cargo_assert(strstr(usage, "the hidden") == NULL,
        "Found hidden short option description in short usage");

    // It should be shown in full usage.
    usage = cargo_get_usage(cargo, CARGO_USAGE_FULL);
    printf("%s\n", usage);

    cargo_assert(strstr(usage, "--alpha") != NULL,
        "Did not find hidden short option in full usage");

    cargo_assert(strstr(usage, "the hidden") != NULL,
        "Did not find hidden short option description in full usage");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_override_short_usage)
{
    char *args[] = { "program", "--alpha", "123", "--centauri", "--beta", "3" };
    int a = 0;
    int b = 0;
    const char *usage = NULL;
    ret = cargo_add_option(cargo, CARGO_OPT_HIDE, "--alpha", "the hidden", "i", &a);
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "i", &b);
    cargo_assert(ret == 0, "Failed to add options");

    cargo_set_progname(cargo, "%s [options] args", args[0]);
    usage = cargo_get_usage(cargo,
        CARGO_USAGE_OVERRIDE_SHORT |
        CARGO_USAGE_SHORT);
    printf("%s\n", usage);

    cargo_assert(strstr(usage, "--help") == NULL,
        "Short usage not overridden, found --help");

    cargo_assert(strstr(usage, "--beta") == NULL,
        "Short usage not overridden, found --beta");

    cargo_assert(!strcmp(usage, "Usage: program [options] args"),
        "Did not find overriden short usage");

    // Test stripping path. First no strip.
    cargo_set_progname(cargo, "%s [options] args", "some/long/path/program");
    usage = cargo_get_usage(cargo,
        CARGO_USAGE_OVERRIDE_SHORT |
        CARGO_USAGE_SHORT);
    printf("STRIP:\n%s\n", usage);

    cargo_assert(strstr(usage, "some/long/path/") == NULL,
        "Found path when stripping");

    usage = cargo_get_usage(cargo,
        CARGO_USAGE_NO_STRIP_PROGNAME |
        CARGO_USAGE_OVERRIDE_SHORT |
        CARGO_USAGE_SHORT);
    printf("NO STRIP:\n%s\n", usage);

    cargo_assert(strstr(usage, "some/long/path/"),
        "Did not find path when no strip");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_duplicate_alias)
{
    const char *usage = NULL;
    ret = cargo_add_option(cargo, 0, "--beta -b", "an option", "D");
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_add_option(cargo, 0, "--bravo -b", "another option", "D");
    cargo_assert(ret != 0, "Successfully added option with taken alias");

    ret = cargo_add_option(cargo, 0, "--bravo", "another option", "D");
    cargo_assert(ret == 0, "Failed to add valid option");

    ret = cargo_add_alias(cargo, "--bravo", "-r");
    cargo_assert(ret == 0, "Failed to add valid alias");

    ret = cargo_add_alias(cargo, "--bravo", "-b");
    cargo_assert(ret != 0, "Successfully added option with taken alias");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_get_option_type)
{
    int a;
    int b;
    float c;
    char *s = NULL;
    const char *usage = NULL;
    ret = cargo_add_option(cargo, 0, "--alpha -a", "an option", "b", &a);
    cargo_assert(ret == 0, "Failed to add option");
    cargo_assert(cargo_get_option_type(cargo, "--alpha") == CARGO_BOOL,
                "Expected alpha to be a bool");

    ret = cargo_add_option(cargo, 0, "--beta -b", "another option", "i", &b);
    cargo_assert(ret == 0, "Failed to add option");
    cargo_assert(cargo_get_option_type(cargo, "--beta") == CARGO_INT,
                "Expected beta to be a int");

    ret = cargo_add_option(cargo, 0, "--centauri -c", "another option", "f", &c);
    cargo_assert(ret == 0, "Failed to add option");
    cargo_assert(cargo_get_option_type(cargo, "--centauri") == CARGO_FLOAT,
                "Expected centauri to be a float");

    ret = cargo_add_option(cargo, 0, "--delta -d", "another option", "s", &s);
    cargo_assert(ret == 0, "Failed to add option");
    cargo_assert(cargo_get_option_type(cargo, "--delta") == CARGO_STRING,
                "Expected delta to be a string");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_large_list_and_usage)
{
    #define LST_SIZE 256
    char *lst[LST_SIZE];
    size_t lst_count = 0;
    const char *usage = NULL;

    // This tests a specific bug where a large array created
    // a buffer overflow when generating the metavar for the usage.
    ret |= cargo_add_option(cargo, 0,
            "--template --input",
            "Path to one or more template files generated on specified events. "
            "(Not to be confused with the template matcher)",
            ".[s]#", &lst, &lst_count, LST_SIZE);
    cargo_assert(ret == 0, "Failed to add option");

    usage = cargo_get_usage(cargo, CARGO_USAGE_SHORT);

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_large_list_and_usage2)
{
    #define LST_SIZE 256
    char *lst[LST_SIZE];
    size_t lst_count = 0;
    const char *usage = NULL;

    ret |= cargo_add_option(cargo, 0,
            "--template --input",
            "Path to one or more template files generated on specified events. "
            "(Not to be confused with the template matcher)",
            ".[s]+", &lst, &lst_count, LST_SIZE);
    cargo_assert(ret == 0, "Failed to add option");

    usage = cargo_get_usage(cargo, 0);
    printf("%s\n", usage);

    _TEST_CLEANUP();
}
_TEST_END()

#define _CARGO_ADD_TEST_VALIDATE(fmt, opt, funcname, ...)                   \
    ret = cargo_add_option(cargo, 0, "--alpha -a", NULL, fmt, opt);         \
    cargo_assert(ret == 0, "Failed to add option");                         \
    ret = cargo_add_validation(cargo, 0, "--alpha", funcname(__VA_ARGS__)); \
    cargo_assert(ret == 0, "Failed to add validation")

#define _CARGO_TEST_VALIDATE_VALUE(val, dofail)                             \
    do                                                                      \
    {                                                                       \
        char *args[] = { "program", "--alpha", #val };                      \
        ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);\
        if (dofail) cargo_assert(ret < 0, "Expected parse failure for " #val);\
        else  cargo_assert(ret == 0, "Failed to parse");                    \
    } while (0)

_TEST_START(TEST_int_range_validation)
{
    int a = 0;
    _CARGO_ADD_TEST_VALIDATE("i", &a, cargo_validate_int_range, 3, 6);
    _CARGO_TEST_VALIDATE_VALUE(-200, 1);
    _CARGO_TEST_VALIDATE_VALUE(2, 1);
    _CARGO_TEST_VALIDATE_VALUE(3, 0);
    _CARGO_TEST_VALIDATE_VALUE(5, 0);
    _CARGO_TEST_VALIDATE_VALUE(6, 0);
    _CARGO_TEST_VALIDATE_VALUE(123, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_float_range_validation)
{
    float a = 0;
    _CARGO_ADD_TEST_VALIDATE("f", &a, cargo_validate_float_range, 3.0f, 6.0f, 0.00001f);
    _CARGO_TEST_VALIDATE_VALUE(-312.4, 1);
    _CARGO_TEST_VALIDATE_VALUE(2.4, 1);
    _CARGO_TEST_VALIDATE_VALUE(3.0, 0);
    _CARGO_TEST_VALIDATE_VALUE(5.5, 0);
    _CARGO_TEST_VALIDATE_VALUE(6.0, 0);
    _CARGO_TEST_VALIDATE_VALUE(6.2, 1);
    _CARGO_TEST_VALIDATE_VALUE(123.424, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_double_range_validation)
{
    double a = 0;
    _CARGO_ADD_TEST_VALIDATE("d", &a, cargo_validate_double_range, 3.0, 6.0, 0.00001);
    _CARGO_TEST_VALIDATE_VALUE(2.3, 1);
    _CARGO_TEST_VALIDATE_VALUE(3.0, 0);
    _CARGO_TEST_VALIDATE_VALUE(5.2, 0);
    _CARGO_TEST_VALIDATE_VALUE(6.0, 0);
    _CARGO_TEST_VALIDATE_VALUE(123.0, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_uint_range_validation)
{
    unsigned int a = 0;
    _CARGO_ADD_TEST_VALIDATE("u", &a, cargo_validate_uint_range, 10, 20);
    _CARGO_TEST_VALIDATE_VALUE(2, 1);
    _CARGO_TEST_VALIDATE_VALUE(10, 0);
    _CARGO_TEST_VALIDATE_VALUE(15, 0);
    _CARGO_TEST_VALIDATE_VALUE(20, 0);
    _CARGO_TEST_VALIDATE_VALUE(123, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_longlong_range_validation)
{
    long long int a = 0;
    _CARGO_ADD_TEST_VALIDATE("L", &a, cargo_validate_longlong_range, 10, 20);
    _CARGO_TEST_VALIDATE_VALUE(-2, 1);
    _CARGO_TEST_VALIDATE_VALUE(10, 0);
    _CARGO_TEST_VALIDATE_VALUE(15, 0);
    _CARGO_TEST_VALIDATE_VALUE(20, 0);
    _CARGO_TEST_VALIDATE_VALUE(123, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_ulonglong_range_validation)
{
    unsigned long long int a = 0;
    _CARGO_ADD_TEST_VALIDATE("U", &a, cargo_validate_ulonglong_range, 10, 20);
    _CARGO_TEST_VALIDATE_VALUE(2, 1);
    _CARGO_TEST_VALIDATE_VALUE(10, 0);
    _CARGO_TEST_VALIDATE_VALUE(15, 0);
    _CARGO_TEST_VALIDATE_VALUE(20, 0);
    _CARGO_TEST_VALIDATE_VALUE(123, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_choices_validation)
{
    char *str = NULL;
    _CARGO_ADD_TEST_VALIDATE("s", &str,
            cargo_validate_choices, 0, CARGO_STRING,
            2, "abc", "def");
    _CARGO_TEST_VALIDATE_VALUE(abc, 0);
    _CARGO_TEST_VALIDATE_VALUE(def, 0);
    _CARGO_TEST_VALIDATE_VALUE(klmn, 1);
    _CARGO_TEST_VALIDATE_VALUE(ABC, 0);
    _CARGO_TEST_VALIDATE_VALUE(DEF, 0);
    _TEST_CLEANUP();
    _cargo_xfree(&str);
}
_TEST_END()

_TEST_START(TEST_choices_validation_case_sensitive)
{
    char *str = NULL;
    _CARGO_ADD_TEST_VALIDATE("s", &str,
            cargo_validate_choices,
            CARGO_VALIDATE_CHOICES_CASE_SENSITIVE,
            CARGO_STRING,
            2, "abc", "def");
    _CARGO_TEST_VALIDATE_VALUE(abc, 0);
    _CARGO_TEST_VALIDATE_VALUE(def, 0);
    _CARGO_TEST_VALIDATE_VALUE(klmn, 1);
    _CARGO_TEST_VALIDATE_VALUE(ABC, 1);
    _CARGO_TEST_VALIDATE_VALUE(DEF, 1);
    _TEST_CLEANUP();
    _cargo_xfree(&str);
}
_TEST_END()

_TEST_START(TEST_choices_validation_int)
{
    int a;
    _CARGO_ADD_TEST_VALIDATE("i", &a,
            cargo_validate_choices, 0, CARGO_INT,
            3, -40, 30, 10);
    _CARGO_TEST_VALIDATE_VALUE(-40, 0);
    _CARGO_TEST_VALIDATE_VALUE(30, 0);
    _CARGO_TEST_VALIDATE_VALUE(40, 1);
    _CARGO_TEST_VALIDATE_VALUE(20, 1);
    _CARGO_TEST_VALIDATE_VALUE(10, 0);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_choices_validation_uint)
{
    int a;
    _CARGO_ADD_TEST_VALIDATE("u", &a,
            cargo_validate_choices, 0, CARGO_UINT,
            3, 40, 30, 10);
    _CARGO_TEST_VALIDATE_VALUE(-45, 1);
    _CARGO_TEST_VALIDATE_VALUE(30, 0);
    _CARGO_TEST_VALIDATE_VALUE(40, 0);
    _CARGO_TEST_VALIDATE_VALUE(20, 1);
    _CARGO_TEST_VALIDATE_VALUE(10, 0);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_choices_validation_float)
{
    float a;
    _CARGO_ADD_TEST_VALIDATE("f", &a,
            cargo_validate_choices, 0, CARGO_FLOAT,
            3, 3.2, 44.35, 500402.4);
    _CARGO_TEST_VALIDATE_VALUE(3131.0, 1);
    _CARGO_TEST_VALIDATE_VALUE(3.2, 0);
    _CARGO_TEST_VALIDATE_VALUE(44.35, 0);
    _CARGO_TEST_VALIDATE_VALUE(500402.4, 0);
    _CARGO_TEST_VALIDATE_VALUE(10, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_choices_validation_double)
{
    double a;
    _CARGO_ADD_TEST_VALIDATE("d", &a,
            cargo_validate_choices, 0, CARGO_DOUBLE,
            3, 3.2, 44.35, 500402.4);
    _CARGO_TEST_VALIDATE_VALUE(3131.0, 1);
    _CARGO_TEST_VALIDATE_VALUE(3.2, 0);
    _CARGO_TEST_VALIDATE_VALUE(44.35, 0);
    _CARGO_TEST_VALIDATE_VALUE(500402.4, 0);
    _CARGO_TEST_VALIDATE_VALUE(10, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_choices_validation_longlong)
{
    long long int a;

    _CARGO_ADD_TEST_VALIDATE("L", &a,
            cargo_validate_choices, 0, CARGO_LONGLONG,
            3,
            (long long int)-1111111,
            (long long int)32323232,
            (long long int)1010101);
    _CARGO_TEST_VALIDATE_VALUE(5, 1);
    _CARGO_TEST_VALIDATE_VALUE(-1111111, 0);
    _CARGO_TEST_VALIDATE_VALUE(32323232, 0);
    _CARGO_TEST_VALIDATE_VALUE(1010101, 0);
    _CARGO_TEST_VALIDATE_VALUE(4224422, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_choices_validation_ulonglong)
{
    unsigned long long int a;
    _CARGO_ADD_TEST_VALIDATE("U", &a,
            cargo_validate_choices, 0, CARGO_ULONGLONG,
            3,
            (unsigned long long int)11111111,
            (unsigned long long int)32323232,
            (unsigned long long int)1010101);
    _CARGO_TEST_VALIDATE_VALUE(5, 1);
    _CARGO_TEST_VALIDATE_VALUE(11111111, 0);
    _CARGO_TEST_VALIDATE_VALUE(32323232, 0);
    _CARGO_TEST_VALIDATE_VALUE(1010101, 0);
    _CARGO_TEST_VALIDATE_VALUE(4224422, 1);
    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_double_parse)
{
    // Set default values and make sure they are not cleared if we parse
    // multiple times.
    char *args[] = { "program", "--beta", "3" };
    char *args2[] = { "program", "--alpha", "2" };
    int a = 5;
    int b = 6;
    float c = 7.0f;
    char *s = strdup("abc");
    cargo_assert(s, "Failed to allocate s");

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "an option", "i", &a);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "another option", "i", &b);
    ret |= cargo_add_option(cargo, 0, "--centauri -c", "another option", "f", &c);
    ret |= cargo_add_option(cargo, 0, "--delta -d", "another option", "s", &s);
    cargo_assert(ret == 0, "Failed to add options");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");

    printf("First parse:\n");
    printf("a == %d\n", a);
    printf("b == %d\n", b);
    printf("c == %f\n", c);
    printf("s == %s\n", s);
    cargo_assert(a == 5, "Expected a == 5");
    cargo_assert(b == 3, "Expected b == 3 after parse");
    cargo_assert(c == 7.0f, "Expected b == 7.0f");
    cargo_assert(s && !strcmp(s, "abc"), "Expected s == 'abc'");

    ret = cargo_parse(cargo, 0, 1, sizeof(args2) / sizeof(args2[0]), args2);
    cargo_assert(ret == 0, "Parse failed");

    printf("\nSecond parse\n");
    printf("a == %d\n", a);
    printf("b == %d\n", b);
    printf("c == %f\n", c);
    printf("s == %s\n", s);
    cargo_assert(a == 2, "Expected a == 2");
    cargo_assert(b == 3, "Expected b == 3 after parse");
    cargo_assert(c == 7.0f, "Expected b == 7.0f");
    cargo_assert(s && !strcmp(s, "abc"), "Expected s == 'abc'");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START(TEST_double_parse_clear)
{
    char *args[] = { "program", "--beta", "3" };
    char *args2[] = { "program", "--delta", "def" };
    int a = 5;
    int b = 6;
    float c = 7.0f;
    char *s = strdup("abc");
    cargo_assert(s, "Failed to allocate");

    ret |= cargo_add_option(cargo, 0, "--alpha -a", "an option", "i", &a);
    ret |= cargo_add_option(cargo, 0, "--beta -b", "another option", "i", &b);
    ret |= cargo_add_option(cargo, 0, "--centauri -c", "another option", "f", &c);
    ret |= cargo_add_option(cargo, 0, "--delta -d", "another option", "s", &s);

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(s && !strcmp(s, "abc"), "Expected s to be 'abc'");

    ret = cargo_parse(cargo, 0, 1, sizeof(args2) / sizeof(args2[0]), args2);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(s && !strcmp(s, "def"), "Expected s to be 'def'");

    _TEST_CLEANUP();
    _cargo_xfree(&s);
}
_TEST_END()

_TEST_START_EX(TEST_default_str, CARGO_AUTOCLEAN)
{
    char *args[] = { "program", "--alpha", "def" };
    char *args2[] = { "program", "--beta", "DEF" };
    char *s1 = strdup("abc");
    char *s2 = "ABC"; // This should not be freed!
    cargo_assert(s1, "Failed to allocate");

    ret |= cargo_add_option(cargo, 0,
            "--alpha -a", "an option", "s", &s1);
    ret |= cargo_add_option(cargo, CARGO_OPT_DEFAULT_LITERAL,
            "--beta -b", "another option", "s", &s2);

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(s1 && !strcmp(s1, "def"), "Expected s to be 'def'");

    ret = cargo_parse(cargo, 0, 1, sizeof(args2) / sizeof(args2[0]), args2);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(s2 && !strcmp(s2, "DEF"), "Expected s to be 'DEF'");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START_EX(TEST_default_str2, CARGO_AUTOCLEAN | CARGO_DEFAULT_LITERALS)
{
    char *args[] = { "program", "--alpha", "def" };
    char *args2[] = { "program", "--beta", "DEF" };
    char *s1 = "abc";
    char *s2 = "ABC"; // This should not be freed!
    cargo_assert(s1, "Failed to allocate");

    ret |= cargo_add_option(cargo, 0,
            "--alpha -a", "an option", "s", &s1);
    ret |= cargo_add_option(cargo, 0,
            "--beta -b", "another option", "s", &s2);

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(s1 && !strcmp(s1, "def"), "Expected s to be 'def'");

    ret = cargo_parse(cargo, 0, 1, sizeof(args2) / sizeof(args2[0]), args2);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(s2 && !strcmp(s2, "DEF"), "Expected s to be 'DEF'");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START_EX(TEST_default_str3, CARGO_AUTOCLEAN | CARGO_DEFAULT_LITERALS)
{
    char *args[] = { "program", "--alpha", "def" };
    char *s1 = "abc";
    char *s2 = "ABC"; // Don't touch this and make sure we don't try to free it.
    cargo_assert(s1, "Failed to allocate");

    ret |= cargo_add_option(cargo, 0,
            "--alpha -a", "an option", "s", &s1);
    ret |= cargo_add_option(cargo, 0,
            "--beta -b", "another option", "s", &s2);

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(s1 && !strcmp(s1, "def"), "Expected s1 to be 'def'");
    cargo_assert(s2 && !strcmp(s2, "ABC"), "Expected s2 to be 'ABC'");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START_EX(TEST_default_str_add_fail, CARGO_DEFAULT_LITERALS)
{
    char *s1 = "abc";
    cargo_assert(s1, "Failed to allocate");

    // This should fail sinc CARGO_AUTOCLEAN is not on.
    ret = cargo_add_option(cargo, 0,
            "--alpha -a", "an option", "s", &s1);
    cargo_assert(ret != 0, "Expected to fail");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_default_str_add_fail2)
{
    char *s1 = "abc";
    cargo_assert(s1, "Failed to allocate");

    // This should fail sinc CARGO_AUTOCLEAN is not on.
    ret = cargo_add_option(cargo, CARGO_OPT_DEFAULT_LITERAL,
            "--alpha -a", "an option", "s", &s1);
    cargo_assert(ret != 0, "Expected to fail");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_nearly_equal)
{
    ret = _cargo_nearly_equal(3.2000001, 3.2, 0.00000001);
    ret |= _cargo_nearly_equal(3.2001, 3.2, 0.00000001);
    cargo_assert(ret == 0, "Should be equal");

    _TEST_CLEANUP();
}
_TEST_END()

_TEST_START(TEST_cargo_strdup_invalid_arg)
{
    char *p = _cargo_strdup(NULL);
    cargo_assert(p == NULL, "Expected strdup to fail");
    cargo_assert(errno == EINVAL, "Expected indication of faulty input argument");

    _TEST_CLEANUP();
}
_TEST_END()


_TEST_START(TEST_cargo_static_list_alloced_items)
{
    #define MAX_INPUT_TEMPLATES 5
    char *inputs[MAX_INPUT_TEMPLATES];
    size_t input_count = 0;
    char *args[] = { "catcierge", "--input", "abc", "def" };

    ret |= cargo_add_option(cargo, 0,
            "--template --input", NULL,
            ".[s]+", &inputs, &input_count, MAX_INPUT_TEMPLATES);
    cargo_assert(ret == 0, "Failed to add option");

    ret = cargo_parse(cargo, 0, 1, sizeof(args) / sizeof(args[0]), args);
    cargo_assert(ret == 0, "Parse failed");
    cargo_assert(inputs[0] && !strcmp(inputs[0], "abc"),
        "Expected inputs[0] == 'abc'");
    cargo_assert(inputs[1] && !strcmp(inputs[1], "def"),
        "Expected inputs[0] == 'def'");
    cargo_assert(input_count == 2, "Expected two inputs");

    _TEST_CLEANUP();
    free(inputs[0]);
    free(inputs[1]);
}
_TEST_END()


// TODO: Test default values for string lists
// TODO: Test giving add_option an invalid alias
// TODO: Test --help
// TODO: Test CARGO_UNIQUE_OPTS
// TODO: Test CARGO_NOWARN
// TODO: Test cargo_parse_result_t
// TODO: Add test to make sure bool array is not allowed.

//
// List of all test functions to run:
//
typedef const char *(* cargo_test_f)();

typedef struct cargo_test_s
{
    const char *name;
    cargo_test_f f;
    int success;
    int ran;
    const char *error;
} cargo_test_t;

#define CARGO_ADD_TEST(test) { #test, test, 0, 0, NULL }

cargo_test_t tests[] =
{
    CARGO_ADD_TEST(TEST_no_args_bool_option),
    CARGO_ADD_TEST(TEST_add_integer_option),
    CARGO_ADD_TEST(TEST_add_uinteger_option),
    CARGO_ADD_TEST(TEST_add_float_option),
    CARGO_ADD_TEST(TEST_add_bool_option),
    CARGO_ADD_TEST(TEST_add_double_option),
    CARGO_ADD_TEST(TEST_add_static_string_option),
    CARGO_ADD_TEST(TEST_add_alloc_string_option),
    CARGO_ADD_TEST(TEST_add_static_int_array_option),
    CARGO_ADD_TEST(TEST_add_static_uint_array_option),
    CARGO_ADD_TEST(TEST_add_static_long_long_int_array_option),
    CARGO_ADD_TEST(TEST_add_static_unsigned_long_long_int_array_option),
    CARGO_ADD_TEST(TEST_add_static_float_array_option),
    CARGO_ADD_TEST(TEST_add_static_double_array_option),
    CARGO_ADD_TEST(TEST_add_static_string_array_option),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_int_array_option),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_uint_array_option),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_float_array_option),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_double_array_option),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_string_array_option),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_string_array_option2),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_string_array_option3),
    CARGO_ADD_TEST(TEST_add_alloc_fixed_string_array_option4),
    CARGO_ADD_TEST(TEST_add_alloc_dynamic_int_array_option),
    CARGO_ADD_TEST(TEST_add_alloc_dynamic_int_array_option_noargs),
    CARGO_ADD_TEST(TEST_print_usage),
    CARGO_ADD_TEST(TEST_get_usage_settings),
    CARGO_ADD_TEST(TEST_autohelp_default),
    CARGO_ADD_TEST(TEST_autohelp_off),
    CARGO_ADD_TEST(TEST_get_usage),
    CARGO_ADD_TEST(TEST_misspelled_argument),
    CARGO_ADD_TEST(TEST_add_duplicate_option),
    CARGO_ADD_TEST(TEST_get_extra_args),
    CARGO_ADD_TEST(TEST_get_unknown_opts),
    CARGO_ADD_TEST(TEST_cargo_split),
    CARGO_ADD_TEST(TEST_parse_invalid_value),
    CARGO_ADD_TEST(TEST_parse_twice),
    CARGO_ADD_TEST(TEST_parse_missing_value),
    CARGO_ADD_TEST(TEST_parse_missing_array_value),
    CARGO_ADD_TEST(TEST_parse_missing_array_value_ensure_free),
    CARGO_ADD_TEST(TEST_parse_same_option_twice),
    CARGO_ADD_TEST(TEST_parse_same_option_twice_string),
    CARGO_ADD_TEST(TEST_parse_same_option_twice_with_unique),
    CARGO_ADD_TEST(TEST_parse_same_option_twice_string_with_unique),
    CARGO_ADD_TEST(TEST_highlight_args),
    CARGO_ADD_TEST(TEST_positional_argument),
    CARGO_ADD_TEST(TEST_positional_array_argument),
    CARGO_ADD_TEST(TEST_multiple_positional_array_argument),
    CARGO_ADD_TEST(TEST_multiple_positional_array_argument2),
    CARGO_ADD_TEST(TEST_multiple_positional_array_argument3),
    CARGO_ADD_TEST(TEST_autoclean_flag),
    CARGO_ADD_TEST(TEST_autoclean_flag_off),
    CARGO_ADD_TEST(TEST_parse_zero_or_more_with_args),
    CARGO_ADD_TEST(TEST_parse_zero_or_more_without_args),
    CARGO_ADD_TEST(TEST_parse_zero_or_more_with_positional),
    CARGO_ADD_TEST(TEST_required_option_missing),
    CARGO_ADD_TEST(TEST_required_option),
    CARGO_ADD_TEST(TEST_custom_callback),
    CARGO_ADD_TEST(TEST_custom_callback_fixed_array),
    CARGO_ADD_TEST(TEST_custom_callback_fixed_array_no_count),
    CARGO_ADD_TEST(TEST_many_options_custom),
    CARGO_ADD_TEST(TEST_custom_callback_array),
    CARGO_ADD_TEST(TEST_zero_or_more_with_arg),
    CARGO_ADD_TEST(TEST_zero_or_more_without_arg),
    CARGO_ADD_TEST(TEST_group),
    CARGO_ADD_TEST(TEST_many_groups),
    CARGO_ADD_TEST(TEST_mutex_group_guard),
    CARGO_ADD_TEST(TEST_mutex_group_require_one),
    CARGO_ADD_TEST(TEST_cargo_split_commandline),
    CARGO_ADD_TEST(TEST_missing_last_arg),
    CARGO_ADD_TEST(TEST_cargo_set_max_width),
    CARGO_ADD_TEST(TEST_cargo_snprintf),
    CARGO_ADD_TEST(TEST_cargo_set_prefix),
    CARGO_ADD_TEST(TEST_cargo_aapendf),
    CARGO_ADD_TEST(TEST_cargo_get_fprint_args),
    CARGO_ADD_TEST(TEST_cargo_get_fprint_args_long),
    CARGO_ADD_TEST(TEST_cargo_fprintf),
    CARGO_ADD_TEST(TEST_cargo_bool_count),
    CARGO_ADD_TEST(TEST_cargo_bool_count_compact),
    CARGO_ADD_TEST(TEST_fixed_one_or_more_array),
    CARGO_ADD_TEST(TEST_fixed_zero_or_more_array),
    CARGO_ADD_TEST(TEST_fixed_fail_array),
    CARGO_ADD_TEST(TEST_user_context),
    CARGO_ADD_TEST(TEST_group_user_context),
    CARGO_ADD_TEST(TEST_mutex_group_context_fail),
    CARGO_ADD_TEST(TEST_cargo_get_option_mutex_groups),
    CARGO_ADD_TEST(TEST_invalid_format_char),
    CARGO_ADD_TEST(TEST_option_target_null),
    CARGO_ADD_TEST(TEST_autoclean_string_list),
    CARGO_ADD_TEST(TEST_fail_custom_callback),
    CARGO_ADD_TEST(TEST_mutex_group_usage),
    CARGO_ADD_TEST(TEST_mutex_group_usage2),
    CARGO_ADD_TEST(TEST_mutex_group_usage3),
    CARGO_ADD_TEST(TEST_mutex_group_usage_set_metavar),
    CARGO_ADD_TEST(TEST_dummy_callback),
    CARGO_ADD_TEST(TEST_group_add_missing_group),
    CARGO_ADD_TEST(TEST_bool_acc_or),
    CARGO_ADD_TEST(TEST_bool_acc_and),
    CARGO_ADD_TEST(TEST_bool_acc_plus),
    CARGO_ADD_TEST(TEST_bool_acc_store),
    CARGO_ADD_TEST(TEST_mutex_order_group_before),
    CARGO_ADD_TEST(TEST_mutex_order_group_after),
    CARGO_ADD_TEST(TEST_medium_length_usage),
    CARGO_ADD_TEST(TEST_cargo_get_unknown_copy),
    CARGO_ADD_TEST(TEST_cargo_get_args_copy),
    CARGO_ADD_TEST(TEST_cargo_set_option_description),
    CARGO_ADD_TEST(TEST_parse_stop),
    CARGO_ADD_TEST(TEST_parse_stop_hard),
    CARGO_ADD_TEST(TEST_parse_stop_hard_fail),
    CARGO_ADD_TEST(TEST_cargo_zero_or_one_with_arg),
    CARGO_ADD_TEST(TEST_cargo_zero_or_one_without_arg),
    CARGO_ADD_TEST(TEST_cargo_zero_or_one_without_arg2),
    CARGO_ADD_TEST(TEST_late_unknown_options),
    CARGO_ADD_TEST(TEST_early_unknown_options),
    CARGO_ADD_TEST(TEST_late_unknown_options_no_fail),
    CARGO_ADD_TEST(TEST_late_unknown_options_no_fail_stop),
    CARGO_ADD_TEST(TEST_cargo_set_error),
    CARGO_ADD_TEST(TEST_cargo_set_error_append),
    CARGO_ADD_TEST(TEST_cargo_set_error_append2),
    CARGO_ADD_TEST(TEST_cargo_set_memfunctions),
    CARGO_ADD_TEST(TEST_cargo_malloc_zero_bytes),
    CARGO_ADD_TEST(TEST_test_hidden_option),
    CARGO_ADD_TEST(TEST_test_hidden_short_option),
    CARGO_ADD_TEST(TEST_override_short_usage),
    CARGO_ADD_TEST(TEST_duplicate_alias),
    CARGO_ADD_TEST(TEST_cargo_get_option_type),
    CARGO_ADD_TEST(TEST_cargo_large_list_and_usage),
    CARGO_ADD_TEST(TEST_cargo_large_list_and_usage2),
    CARGO_ADD_TEST(TEST_int_range_validation),
    CARGO_ADD_TEST(TEST_float_range_validation),
    CARGO_ADD_TEST(TEST_double_range_validation),
    CARGO_ADD_TEST(TEST_uint_range_validation),
    CARGO_ADD_TEST(TEST_longlong_range_validation),
    CARGO_ADD_TEST(TEST_ulonglong_range_validation),
    CARGO_ADD_TEST(TEST_choices_validation),
    CARGO_ADD_TEST(TEST_choices_validation_case_sensitive),
    CARGO_ADD_TEST(TEST_choices_validation_int),
    CARGO_ADD_TEST(TEST_choices_validation_uint),
    CARGO_ADD_TEST(TEST_choices_validation_float),
    CARGO_ADD_TEST(TEST_choices_validation_double),
    CARGO_ADD_TEST(TEST_choices_validation_longlong),
    CARGO_ADD_TEST(TEST_choices_validation_ulonglong),
    CARGO_ADD_TEST(TEST_double_parse),
    CARGO_ADD_TEST(TEST_double_parse_clear),
    CARGO_ADD_TEST(TEST_default_str),
    CARGO_ADD_TEST(TEST_default_str2),
    CARGO_ADD_TEST(TEST_default_str3),
    CARGO_ADD_TEST(TEST_default_str_add_fail),
    CARGO_ADD_TEST(TEST_default_str_add_fail2),
    CARGO_ADD_TEST(TEST_nearly_equal),
    CARGO_ADD_TEST(TEST_cargo_strdup_invalid_arg),
    CARGO_ADD_TEST(TEST_cargo_static_list_alloced_items)
};

#define CARGO_NUM_TESTS (sizeof(tests) / sizeof(tests[0]))

static void _test_print_names()
{
    int i;

    for (i = 0; i < (int)CARGO_NUM_TESTS; i++)
    {
        printf("%2d: %s\n", (i + 1), tests[i].name);
    }
}

static int _test_find_test_index(const char *name)
{
    int i;

    for (i = 0; i < (int)CARGO_NUM_TESTS; i++)
    {
        if (!strcmp(name, tests[i].name))
        {
            return i + 1;
        }
    }

    return -1;
}

int main(int argc, char **argv)
{
    cargo_t cargo;
    size_t i;
    int was_error = 0;
    int tests_to_run[CARGO_NUM_TESTS];
    size_t num_tests = 0;
    size_t success_count = 0;
    int all = 0;
    int quiet = 0;
    int test_index = 0;
    int shortlist = 0;
    int start = 1;
    int ret = 0;
    char **test_names = NULL;
    size_t test_name_count = 0;
    cargo_test_t *t;

    memset(tests_to_run, 0, sizeof(tests_to_run));

    cargo_suppress_debug = 1;

    if (cargo_init(&cargo, CARGO_AUTOCLEAN | CARGO_NOERR_OUTPUT, argv[0]))
    {
        fprintf(stderr, "Failed to init command line parsing\n");
        return -1;
    }

    cargo_set_description(cargo,
        "You can add additional debug output by compiling with CARGO_DEBUG=<level>");

    ret |= cargo_add_option(cargo, 0, "--shortlist --short -s",
            "Only show the tests that have been run, don't include the entire list.",
            "b", &shortlist);

    ret |= cargo_add_option(cargo, 0, "--all -a",
            "Run all tests.",
            "b", &all);

    ret |= cargo_add_option(cargo, 0, "--quiet -q",
            "Suppress CARGO_DEBUG output.",
            "b", &quiet);

    ret |= cargo_add_option(cargo, 0, "tests",
            "Either a test number or name. "
            "If -1 is specified all tests will be run.",
            "[s]*", &test_names, &test_name_count);

    cargo_set_epilog(cargo,
            "Return code for this usage message equals "
            "the number of available tests.");

    assert(ret == 0);

    if (cargo_parse(cargo, 0, 1, argc, argv))
    {
        _test_print_names();
        cargo_print_usage(cargo, 0);
        fprintf(stderr, "%s\n", cargo_get_error(cargo));
        return CARGO_NUM_TESTS;
    }

    if ((test_name_count == 0) && !all)
    {
        _test_print_names();
        cargo_print_usage(cargo, 0);
        fprintf(stderr, "No test specified!\n");
        return CARGO_NUM_TESTS;
    }

    if (!quiet) cargo_suppress_debug = 0;

    for (i = 0; i < test_name_count; i++)
    {
        // First check if we were given a function name.
        if (!strncmp(test_names[i], "TEST_", 5))
        {
            test_index = _test_find_test_index(test_names[i]);

            if (test_index <= 0)
            {
                fprintf(stderr, "Unknown test specified: \"%s\"\n", test_names[i]);
                return CARGO_NUM_TESTS;
            }
        }
        else
        {
            test_index = atoi(test_names[i]);
            fprintf(stderr, "%d\n", test_index);

            if ((test_index == 0) || (test_index > (int)CARGO_NUM_TESTS))
            {
                fprintf(stderr, "Invalid test number %s\n", test_names[i]);
                return CARGO_NUM_TESTS;
            }
            else if (test_index < 0)
            {
                printf("Run ALL tests!\n");
                all = 1;
                break;
            }
        }

        tests_to_run[num_tests] = test_index;
        num_tests++;
    }

    // Run all tests.
    if (all)
    {
        num_tests = CARGO_NUM_TESTS;

        for (i = 0; i < num_tests; i++)
        {
            tests_to_run[i] = i + 1;
        }
    }

    // Run the tests.
    for (i = 0; i < num_tests; i++)
    {
        test_index = tests_to_run[i] - 1;
        t = &tests[test_index];
        t->ran = 1;

        fprintf(stderr, "\n%sStart Test %3d:%s - %s\n",
            CARGO_COLOR_CYAN, test_index + 1, CARGO_COLOR_RESET, t->name);

        //fprintf(stderr, "%s", CARGO_COLOR_DARK_GRAY);
        t->error = t->f();
        fprintf(stderr, "%s", CARGO_COLOR_RESET);

        fprintf(stderr, "%sEnd Test %3d:%s ",
            CARGO_COLOR_CYAN, test_index + 1, CARGO_COLOR_RESET);

        if (t->error)
        {
            fprintf(stderr, "[%sFAIL%s] %s\n",
                CARGO_COLOR_RED, CARGO_COLOR_RESET, t->error);
            was_error++;
        }
        else
        {
            fprintf(stderr, "[%sSUCCESS%s]\n",
                CARGO_COLOR_GREEN, CARGO_COLOR_RESET);

            success_count++;
        }

        tests[test_index].success = (t->error == NULL);
    }

    printf("---------------------------------------------------------------\n");
    printf("Test report:\n");
    printf("---------------------------------------------------------------\n");

    for (i = 0; i < CARGO_NUM_TESTS; i++)
    {
        if (!tests[i].ran)
        {
            if (!shortlist)
            {
                printf(" [%sNOT RUN%s] %3lu: %s\n",
                    CARGO_COLOR_DARK_GRAY, CARGO_COLOR_RESET, (i + 1), tests[i].name);
            }

            continue;
        }

        if (tests[i].success)
        {
            printf(" [%sSUCCESS%s] %3lu: %s\n",
                CARGO_COLOR_GREEN, CARGO_COLOR_RESET, (i + 1), tests[i].name);
        }
        else
        {
            fprintf(stderr, " [%sFAILED%s]  %3lu: %s - %s\n",
                CARGO_COLOR_RED, CARGO_COLOR_RESET,
                (i + 1), tests[i].name, tests[i].error);
        }
    }

    if (was_error)
    {
        fprintf(stderr, "\n[[%sFAIL%s]] ", CARGO_COLOR_RED, CARGO_COLOR_RESET);
    }
    else
    {
        printf("\n[[%sSUCCESS%s]] ", CARGO_COLOR_GREEN, CARGO_COLOR_RESET);
    }

    printf("%lu of %lu tests run were successful (%lu of %lu tests ran)\n",
            success_count, num_tests, num_tests, CARGO_NUM_TESTS);

    cargo_suppress_debug = 1;
    cargo_destroy(&cargo);
    if (!quiet) cargo_suppress_debug = 0;

    return (num_tests - success_count);
}

#elif defined(CARGO_HELPER)

int main(int argc, char **argv)
{
    int ret = 0;
    char *fmt = NULL;
    char *end = NULL;
    char arrsize[1024];
    char lenstr[1024];
    char varname[1024];
    cargo_type_t types[3];
    size_t type_count = 0;
    int array = 0;
    int alloc = 0;
    int static_str = 0;
    size_t example_count = 1;
    size_t i;
    size_t j;
    memset(types, 0, sizeof(types));

    if (argc < 2)
    {
        fprintf(stderr, "%s: <variable declaration>\n", argv[0]);
        return -1;
    }

    fmt = argv[1];

    printf("%s;\n", fmt);

    while (isspace(*fmt)) fmt++;

    // Get type.
    if (!strncmp(fmt, "int", 3))
    {
        types[type_count++] = CARGO_INT;
        types[type_count++] = CARGO_BOOL;
        fmt += 3;
    }
    else if (!strncmp(fmt, "long long", 9))
    {
        types[type_count++] = CARGO_LONGLONG;
        fmt += 9;
    }
    else if (!strncmp(fmt, "unsigned long long", 18))
    {
        types[type_count++] = CARGO_ULONGLONG;
        fmt += 18;
    }
    else if (!strncmp(fmt, "char", 4))
    {
        types[type_count++] = CARGO_STRING;
        fmt += 4;
    }
    else if (!strncmp(fmt, "float", 5))
    {
        types[type_count++] = CARGO_FLOAT;
        fmt += 5;
    }
    else if (!strncmp(fmt, "double", 6))
    {
        types[type_count++] = CARGO_DOUBLE;
        fmt += 6;
    }
    else if (!strncmp(fmt, "unsigned int", 12))
    {
        types[type_count++] = CARGO_UINT;
        fmt += 12;
    }

    while (isspace(*fmt)) fmt++;

    // Set array and alloc status.
    if (types[0] == CARGO_STRING)
    {
        if (!strncmp(fmt, "**", 2))
        {
            alloc = 1;
            array = 1;
            fmt += 2;
        }
        else if (*fmt == '*')
        {
            alloc = 1;
            fmt++;
        }
    }
    else
    {
        if (*fmt == '*')
        {
            alloc = 1;
            array = 1;
            // We can't have CARGO_BOOL arrays.
            if (types[0] == CARGO_INT)
                type_count--;

            fmt++;
        }
    }

    // Get var name.
    if ((end = strchr(fmt, '[')) != NULL)
    {
        cargo_snprintf(varname, sizeof(varname), "%.*s", (end - fmt), fmt);
    }
    else
    {
        cargo_snprintf(varname, sizeof(varname), "%s", fmt);
    }

    // Get variable name.
    if ((end = strchr(fmt, '[')) != NULL)
    {
        char *start = end + 1;
        array = 1;

        // We can't have CARGO_BOOL arrays.
        if (types[0] == CARGO_INT)
            type_count--;

        if ((end = strchr(fmt, ']')) == NULL)
        {
            fprintf(stderr, "Missing ']'\n");
            ret = -1; goto fail;
        }

        cargo_snprintf(arrsize, sizeof(arrsize), "%.*s", (end - start), start);

        if (types[0] == CARGO_STRING)
        {
            if (alloc)
            {
                fprintf(stderr, "You cannot use string arrays of this format\n");
                ret = -1; goto fail;
            }

            if ((end = strchr(start, '[')) != NULL)
            {
                // We have an array of strings.
                // char bla[COUNT][LENSTR];
                start = end + 1;

                if ((end = strchr(start, ']')) == NULL)
                {
                    fprintf(stderr, "Missing ']'\n");
                    ret = -1; goto fail;
                }

                static_str = 1;
                cargo_snprintf(lenstr, sizeof(lenstr), "%.*s", (end - start), start);
            }
            else
            {
                // This is not an array, simply a static string.
                // char bla[123];
                array = 0;
                static_str = 1;
                cargo_snprintf(lenstr, sizeof(lenstr), "%s", arrsize);
            }
        }

        if (strlen(arrsize) == 0)
        {
            cargo_snprintf(arrsize, sizeof(arrsize), "sizeof(%s) / sizeof(%s[0])",
                            varname, varname);
        }
    }

    if ((types[0] == CARGO_STRING) && !array && !alloc && !static_str)
    {
        fprintf(stderr, "\"char\" is not a valid variable type by itself, did you mean \"char *\"?\n");
        return -1;
    }

    if (alloc) example_count++;
    if (array) printf("size_t %s_count;\n", varname);

    // TODO: Create extra examples for [s#]#
    //if ((types[0] == CARGO_STRING)) example_count += 2;

    #define IS_EXTRA_EXAMPLE(num) ((j % 2) == 0)

    for (j = 0; j < example_count; j++)
    {
        for (i = 0; i < type_count; i++)
        {
            printf("cargo_add_option(cargo, 0, \"--%s", varname);
            if (strlen(varname) > 1) printf(" -%c", varname[0]);
            printf("\", \"Description of %s\", \"", varname);

            if (!alloc)
            {
                if (static_str || array)
                    printf(".");
            }

            if (array) printf("[");

            printf("%c", _cargo_type_to_str(types[i])[0]);

            if (array && static_str) printf("#");
            if (array) printf("]");

            if (array && IS_EXTRA_EXAMPLE(1))
            {
                printf("#");
            }
            else
            {
                if (array && alloc) printf("+");
            }

            printf("\", &%s", varname);

            if (static_str) printf(", %s", lenstr);

            if (array)
            {
                printf(", &%s_count", varname);

                if (!alloc)
                {
                    printf(", %s", arrsize);
                }
            }

            if (array && alloc && IS_EXTRA_EXAMPLE(1))
            {
                printf(", 128");
            }

            printf(");");

            if (array && alloc && IS_EXTRA_EXAMPLE(1))
            {
                printf(" // Allocated with max length 128.");
            }
            else if (array && alloc)
            {
                printf(" // Allocated unlimited length.");
            }

            printf("\n");
        }
    }

fail:
    return ret;
}

#elif defined(CARGO_EXAMPLE)

typedef struct args_s
{
    int a;
} args_t;

int main(int argc, char **argv)
{
    int ret = 0;
    cargo_t cargo;

    cargo_init(&cargo, 0, argv[0]);

    // TODO: Make a real example.

    cargo_print_usage(cargo, 0);
    cargo_destroy(&cargo);
    return ret;
}

#endif // CARGO_EXAMPLE

// LCOV_EXCL_END
