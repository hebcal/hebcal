#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(hebcal,3.4,hebcal-bugs@sadinoff.com)
AC_CONFIG_SRCDIR([common.c])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([sqrt strchr strdup])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([doc/Makefile])

AC_OUTPUT
