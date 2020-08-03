#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([hebcal],[4.21],[hebcal-bugs@sadinoff.com])
AM_MAINTAINER_MODE([disable])
AM_INIT_AUTOMAKE([foreign no-dependencies no-installinfo -Wall])
AC_CONFIG_SRCDIR([hebcal.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
# Enable all the warnings.
AX_CFLAGS_WARN_ALL

AC_CHECK_HEADERS([getopt.h])

# Checks for libraries.
AC_SEARCH_LIBS([hypot], [m])
AC_SEARCH_LIBS([sin], [m])
AC_SEARCH_LIBS([cos], [m])
AC_SEARCH_LIBS([atan], [m])
AC_SEARCH_LIBS([sqrt], [m])
AC_SEARCH_LIBS([acos], [m])
AC_SEARCH_LIBS([fabs], [m])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strchr strdup])
AC_ARG_WITH([default-city],
  [AS_HELP_STRING([--with-default-city=CITYNAME],
     [make the default city CITYNAME.  If unspecified, the default city will be "New York"])],
     [AC_DEFINE_UNQUOTED(CITY,"$withval",[The Default City])],
     [AC_DEFINE(CITY,"New_York",[The Default City])])

AC_CONFIG_FILES([hebcal.1])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([tests/Makefile])


AC_OUTPUT
