#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([guile-pkgconf], [0.0.1], [873216071@qq.com])
AC_CONFIG_SRCDIR([pkgconf.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([color-tests subdir-objects gnu])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([pre-inst-env], [chmod +x pre-inst-env])
# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([GUILE], [guile-3.0])
PKG_CHECK_MODULES([PKGCONF], [libpkgconf])
# Checks for header files.
AC_CHECK_HEADERS([libguile.h])
LT_INIT([dlopen disable-static])
GUILE_PKG([3.0])

GUILE_PROGS
GUILE_SITE_DIR
AC_OUTPUT
