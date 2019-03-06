#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gsh], [1.0.2], [contact@hguandl.com])
AC_CONFIG_SRCDIR([src/gsh.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99

# Checks for libraries.
AC_CHECK_LIB([readline], [readline])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getcwd gethostname setenv strstr])

# Automake
AM_INIT_AUTOMAKE([foreign -Wno-gnu -Werror])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
