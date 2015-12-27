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
#ifndef __CARGO_H__
#define __CARGO_H__

#include <stdio.h>

// Please override any defaults in this header.
#ifdef CARGO_CONFIG
#include "cargo_config.h"
#endif

//
// Defaults.
//

#ifndef CARGO_NAME_COUNT
#define CARGO_NAME_COUNT 4
#endif

#ifndef CARGO_DEFAULT_PREFIX
#define CARGO_DEFAULT_PREFIX "-"
#endif

#ifndef CARGO_DEFAULT_MAX_OPTS
#define CARGO_DEFAULT_MAX_OPTS 32
#endif

#ifndef CARGO_DEFAULT_MAX_WIDTH
#define CARGO_DEFAULT_MAX_WIDTH 80
#endif

#ifndef CARGO_AUTO_MAX_WIDTH
#define CARGO_AUTO_MAX_WIDTH 0
#endif

#ifndef CARGO_MAX_MAX_WIDTH
#define CARGO_MAX_MAX_WIDTH 1024
#endif

#ifndef CARGO_MAX_OPT_MUTEX_GROUP
#define CARGO_MAX_OPT_MUTEX_GROUP 4
#endif

//
// Colors.
//

#define CARGO_COLOR_BLACK           "\x1b[22;30m"
#define CARGO_COLOR_RED             "\x1b[22;31m"
#define CARGO_COLOR_GREEN           "\x1b[22;32m"
#define CARGO_COLOR_YELLOW          "\x1b[22;33m"
#define CARGO_COLOR_BLUE            "\x1b[22;34m"
#define CARGO_COLOR_MAGENTA         "\x1b[22;35m"
#define CARGO_COLOR_CYAN            "\x1b[22;36m"
#define CARGO_COLOR_GRAY            "\x1b[22;37m"
#define CARGO_COLOR_DARK_GRAY       "\x1b[01;30m"
#define CARGO_COLOR_LIGHT_RED       "\x1b[01;31m"
#define CARGO_COLOR_LIGHT_GREEN     "\x1b[01;32m"
#define CARGO_COLOR_LIGHT_BLUE      "\x1b[01;34m"
#define CARGO_COLOR_LIGHT_MAGNETA   "\x1b[01;35m"
#define CARGO_COLOR_LIGHT_CYAN      "\x1b[01;36m"
#define CARGO_COLOR_WHITE           "\x1b[01;37m"
#define CARGO_COLOR_RESET           "\x1b[0m"

//
// Version.
//

#define CARGO_XSTRINGIFY(s) #s
#define CARGO_STRINGIFY(s) CARGO_XSTRINGIFY(s)

#define CARGO_MAJOR_VERSION 0
#define CARGO_MINOR_VERSION 1
#define CARGO_PATCH_VERSION 0
#define CARGO_RELEASE       1 // 0 = Release, 1 = Alpha.

#define CARGO_VERSION                        \
    ((CARGO_MAJOR_VERSION << 24) |           \
    (CARGO_MINOR_VERSION << 16)  |           \
    (CARGO_PATCH_VERSION << 8)   |           \
    (CARGO_RELEASE))

#if CARGO_RELEASE == 0
#define CARGO_VERSION_STR                    \
    CARGO_STRINGIFY(CARGO_MAJOR_VERSION) "." \
    CARGO_STRINGIFY(CARGO_MINOR_VERSION) "." \
    CARGO_STRINGIFY(CARGO_PATCH_VERSION)
#else
#define CARGO_VERSION_STR                    \
    CARGO_STRINGIFY(CARGO_MAJOR_VERSION) "." \
    CARGO_STRINGIFY(CARGO_MINOR_VERSION) "." \
    CARGO_STRINGIFY(CARGO_PATCH_VERSION)     \
    "-alpha"
#endif

const char *cargo_get_version();


//
// Types.
//

typedef struct cargo_s *cargo_t;

typedef enum cargo_type_e
{
    CARGO_BOOL                          = (1 << 0),
    CARGO_INT                           = (1 << 1),
    CARGO_UINT                          = (1 << 2),
    CARGO_FLOAT                         = (1 << 3),
    CARGO_DOUBLE                        = (1 << 4),
    CARGO_STRING                        = (1 << 5),
    CARGO_LONGLONG                      = (1 << 6),
    CARGO_ULONGLONG                     = (1 << 7)
} cargo_type_t;

//
// Flags.
//

typedef enum cargo_flags_e
{
    CARGO_AUTOCLEAN                     = (1 << 0),
    CARGO_NOCOLOR                       = (1 << 1),
    CARGO_NOERR_OUTPUT                  = (1 << 2),
    CARGO_NOERR_USAGE                   = (1 << 3),
    CARGO_STDOUT_ERR                    = (1 << 4),
    CARGO_NO_AUTOHELP                   = (1 << 5),
    CARGO_NO_FAIL_UNKNOWN               = (1 << 6),
    CARGO_UNIQUE_OPTS                   = (1 << 7),
    CARGO_NOWARN                        = (1 << 8),
    CARGO_UNKNOWN_EARLY                 = (1 << 9),
    CARGO_DEFAULT_LITERALS              = (1 << 10)
} cargo_flags_t;

typedef enum cargo_format_e
{
    CARGO_USAGE_FULL                    = (0 << 0),
    CARGO_USAGE_SHORT                   = (1 << 0),
    CARGO_USAGE_RAW_DESCRIPTION         = (1 << 1),
    CARGO_USAGE_RAW_OPT_DESCRIPTIONS    = (1 << 2),
    CARGO_USAGE_RAW_EPILOG              = (1 << 3),
    CARGO_USAGE_HIDE_DESCRIPTION        = (1 << 4),
    CARGO_USAGE_HIDE_EPILOG             = (1 << 5),
    CARGO_USAGE_HIDE_SHORT              = (1 << 6),
    CARGO_USAGE_OVERRIDE_SHORT          = (1 << 7),
    CARGO_USAGE_NO_STRIP_PROGNAME       = (1 << 8)
} cargo_usage_t;

typedef enum cargo_option_flags_e
{
    CARGO_OPT_UNIQUE                    = (1 << 0),
    CARGO_OPT_REQUIRED                  = (1 << 1),
    CARGO_OPT_NOT_REQUIRED              = (1 << 2),
    CARGO_OPT_RAW_DESCRIPTION           = (1 << 3),
    CARGO_OPT_STOP                      = (1 << 4),
    CARGO_OPT_HIDE                      = (1 << 5),
    CARGO_OPT_HIDE_SHORT                = (1 << 6),
    CARGO_OPT_STOP_HARD                 = (1 << 7),
    CARGO_OPT_DEFAULT_LITERAL           = (1 << 8)
} cargo_option_flags_t;

typedef enum cargo_mutex_group_flags_e
{
    CARGO_MUTEXGRP_ONE_REQUIRED         = (1 << 0),
    CARGO_MUTEXGRP_GROUP_USAGE          = (1 << 1),
    CARGO_MUTEXGRP_NO_GROUP_SHORT_USAGE = (1 << 2),
    CARGO_MUTEXGRP_RAW_DESCRIPTION      = (1 << 3),
    CARGO_MUTEXGRP_ORDER_BEFORE         = (1 << 4),
    CARGO_MUTEXGRP_ORDER_AFTER          = (1 << 5)
} cargo_mutex_group_flags_t;

typedef enum cargo_group_flags_e
{
    CARGO_GROUP_HIDE                    = (1 << 0),
    CARGO_GROUP_RAW_DESCRIPTION         = (1 << 1)
} cargo_group_flags_t;

typedef enum cargo_parse_result_e
{
    CARGO_PARSE_CALLBACK_ERR            = -8,
    CARGO_PARSE_OPT_ALREADY_PARSED      = -7,
    CARGO_PARSE_MUTEX_CONFLICT_ORDER    = -6,
    CARGO_PARSE_MUTEX_CONFLICT          = -5,
    CARGO_PARSE_MISS_REQUIRED           = -4,
    CARGO_PARSE_FAIL_OPT                = -3,
    CARGO_PARSE_NOMEM                   = -2,
    CARGO_PARSE_UNKNOWN_OPTS            = -1,
    CARGO_PARSE_OK                      = 0,
    CARGO_PARSE_SHOW_HELP               = 1
} cargo_parse_result_t;

typedef enum cargo_err_flags_e
{
    CARGO_ERR_DEFAULT                   = (0 << 0),
    CARGO_ERR_APPEND                    = (1 << 0)
} cargo_err_flags_t;

typedef enum cargo_width_flags_e
{
    CARGO_WIDTH_USED                    = (0 << 0),
    CARGO_WIDTH_RAW                     = (1 << 0)
} cargo_width_flags_t;

typedef enum cargo_validation_flags_e
{
    CARGO_VALIDATION_NONE               = (0 << 0),
} cargo_validation_flags_t;

typedef enum cargo_validate_choices_flags_e
{
    CARGO_VALIDATE_CHOICES_NONE             = (0 << 0),
    CARGO_VALIDATE_CHOICES_CASE_SENSITIVE   = (1 << 0),
    CARGO_VALIDATE_CHOICES_SET_EPSILON      = (1 << 1)
} cargo_validate_choices_flags_t;

//
// Callback types.
//

// Custom option parser callback function.
typedef int (*cargo_custom_f)(cargo_t ctx, void *user, const char *optname,
                                int argc, char **argv);

typedef void *(*cargo_malloc_f)(size_t bytes);
typedef void (*cargo_free_f)(void *ptr);
typedef void *(*cargo_realloc_f)(void *ptr, size_t bytes);

//
// Functions.
//

int cargo_init(cargo_t *ctx, cargo_flags_t flags, const char *progname_fmt, ...);

void cargo_destroy(cargo_t *ctx);

void cargo_set_flags(cargo_t ctx, cargo_flags_t flags);

cargo_flags_t cargo_get_flags(cargo_t ctx);

int cargo_add_optionv(cargo_t ctx, cargo_option_flags_t flags,
                      const char *optnames,
                      const char *description,
                      const char *fmt, va_list ap);

int cargo_add_option(cargo_t ctx, cargo_option_flags_t flags,
                    const char *optnames, const char *description,
                    const char *fmt, ...);

int cargo_add_alias(cargo_t ctx, const char *optname, const char *alias);

int cargo_set_metavarv(cargo_t ctx,
                    const char *optname,
                    const char *fmt, va_list ap);

int cargo_set_metavar(cargo_t ctx,
                    const char *optname,
                    const char *fmt, ...);

int cargo_set_option_descriptionv(cargo_t ctx,
                                  const char *optname,
                                  const char *fmt, va_list ap);

int cargo_set_option_description(cargo_t ctx,
                                 const char *optname,
                                 const char *fmt, ...);

int cargo_add_group(cargo_t ctx, cargo_group_flags_t flags, const char *name,
                    const char *title, const char *description, ...);

int cargo_group_add_option(cargo_t ctx, const char *group, const char *opt);

int cargo_group_set_flags(cargo_t ctx, const char *group,
                          cargo_group_flags_t flags);

int cargo_add_mutex_group(cargo_t ctx,
                        cargo_mutex_group_flags_t flags,
                        const char *name,
                        const char *title,
                        const char *description, ...);

int cargo_mutex_group_add_option(cargo_t ctx,
                                const char *group,
                                const char *opt);

int cargo_mutex_group_set_metavarv(cargo_t ctx,
                                   const char *mutex_group,
                                   const char *fmt, va_list ap);

int cargo_mutex_group_set_metavar(cargo_t ctx,
                                  const char *mutex_group,
                                  const char *fmt, ...);

void cargo_set_internal_usage_flags(cargo_t ctx, cargo_usage_t flags);

cargo_parse_result_t cargo_parse(cargo_t ctx, cargo_flags_t flags,
                                int start_index, int argc, char **argv);

void cargo_set_prefix(cargo_t ctx, const char *prefix_chars);

void cargo_set_max_width(cargo_t ctx, size_t max_width);

int cargo_get_width(cargo_t ctx, cargo_width_flags_t flags);

void cargo_set_prognamev(cargo_t ctx, const char *fmt, va_list ap);

void cargo_set_progname(cargo_t ctx, const char *fmt, ...);

void cargo_set_descriptionv(cargo_t ctx, const char *fmt, va_list ap);

void cargo_set_description(cargo_t ctx, const char *fmt, ...);

void cargo_set_epilogv(cargo_t ctx, const char *fmt, va_list ap);

void cargo_set_epilog(cargo_t ctx, const char *fmt, ...);

int cargo_fprint_usage(cargo_t ctx, FILE *f, cargo_usage_t flags);

int cargo_print_usage(cargo_t ctx, cargo_usage_t flags);

const char *cargo_get_usage(cargo_t ctx, cargo_usage_t flags);

const char *cargo_get_error(cargo_t ctx);

void cargo_set_errorv(cargo_t ctx, cargo_err_flags_t flags,
                    const char *fmt, va_list ap);

void cargo_set_error(cargo_t ctx,
                    cargo_err_flags_t flags, const char *fmt, ...);

const char **cargo_get_unknown(cargo_t ctx, size_t *unknown_count);

char **cargo_get_unknown_copy(cargo_t ctx, size_t *unknown_count);

const char **cargo_get_args(cargo_t ctx, size_t *argc);

char **cargo_get_args_copy(cargo_t ctx, size_t *argc);

int cargo_get_stop_index(cargo_t ctx);

void cargo_set_context(cargo_t ctx, void *user);

void *cargo_get_context(cargo_t ctx);

int cargo_set_group_context(cargo_t ctx, const char *group, void *user);

void *cargo_get_group_context(cargo_t ctx, const char *group);

int cargo_set_mutex_group_context(cargo_t ctx,
                                const char *mutex_group,
                                void *user);

void *cargo_get_mutex_group_context(cargo_t ctx, const char *mutex_group);

const char *cargo_get_option_group(cargo_t ctx, const char *opt);

const char **cargo_get_option_mutex_groups(cargo_t ctx,
                                        const char *opt,
                                        size_t *count);

void cargo_set_memfunctions(cargo_malloc_f malloc_replacement,
                            cargo_realloc_f realloc_replacement,
                            cargo_free_f free_replacement);

cargo_type_t cargo_get_option_type(cargo_t ctx, const char *opt);


//
// Validation.
//

typedef struct cargo_validation_s cargo_validation_t;

typedef int (*cargo_validation_f)(cargo_t ctx,
                                cargo_validation_flags_t flags,
                                const char *opt, cargo_validation_t *vd,
                                void *value);

typedef void (*cargo_validation_destroy_f)(void *user);

#define CARGO_DEFAULT_EPSILON 0.000000000000000001

int cargo_add_validation(cargo_t ctx, cargo_validation_flags_t flags,
                        const char *opt, cargo_validation_t *vd);

cargo_validation_t *cargo_create_validator(const char *name,
                                           cargo_validation_f validator,
                                           cargo_validation_destroy_f destroy,
                                           cargo_type_t types,
                                           void *user);

void *cargo_validator_get_context(cargo_validation_t *validator);

//
// Validators.
//
cargo_validation_t *cargo_validate_int_range(int min, int max);
cargo_validation_t *cargo_validate_uint_range(unsigned int min,
                                              unsigned int max);
cargo_validation_t *cargo_validate_longlong_range(long long int min,
                                                  long long int max);
cargo_validation_t *cargo_validate_ulonglong_range(unsigned long long int min,
                                                   unsigned long long int max);
cargo_validation_t *cargo_validate_float_range(float min, float max,
                                                float epsilon);
cargo_validation_t *cargo_validate_double_range(double min, double max,
                                                double epsilon);
cargo_validation_t *cargo_validate_choices(
                                        cargo_validate_choices_flags_t flags,
                                        cargo_type_t type,
                                        size_t count, ...);


//
// Utility types.
//

typedef struct cargo_highlight_s
{
    int i;              // Index of highlight in argv.
    char *c;            // Highlight character (followed by color).
} cargo_highlight_t;

typedef enum cargo_fprint_flags_e
{
    CARGO_FPRINT_NOCOLOR                = (1 << 0),
    CARGO_FPRINT_NOARGS                 = (1 << 1),
    CARGO_FPRINT_NOHIGHLIGHT            = (1 << 2)
} cargo_fprint_flags_t;

typedef enum cargo_splitcmd_flags_e
{
    CARGO_SPLITCMD_DEFAULT = 0
} cargo_splitcmd_flags_t;

//
// Utility functions.
//

void cargo_fprintf(FILE *fd, const char *fmt, ...);

char *cargo_get_fprint_args(int argc, char **argv, int start,
                            cargo_fprint_flags_t flags,
                            size_t max_width,
                            size_t highlight_count, ...);

char *cargo_get_fprintl_args(int argc, char **argv, int start,
                            cargo_fprint_flags_t flags,
                            size_t highlight_count,
                            size_t max_width,
                            const cargo_highlight_t *highlights);

char *cargo_get_vfprint_args(int argc, char **argv, int start,
                            cargo_fprint_flags_t flags,
                            size_t max_width,
                            size_t highlight_count, va_list ap);

int cargo_fprint_args(FILE *f, int argc, char **argv, int start,
                      cargo_fprint_flags_t flags,
                      size_t max_width,
                      size_t highlight_count, ...);

int cargo_fprintl_args(FILE *f, int argc, char **argv, int start,
                       cargo_fprint_flags_t flags,
                       size_t max_width, size_t highlight_count,
                       const cargo_highlight_t *highlights);

char **cargo_split_commandline(cargo_splitcmd_flags_t flags,
                                const char *args, int *argc);

void cargo_free_commandline(char ***argv, int argc);

#endif // __CARGO_H__
