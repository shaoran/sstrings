AC_PREREQ(2.59)
dnl Process this file with autoconf to produce a configure script.
AC_INIT(sstrings/sstrings.c)
dnl Setting Package name for automake
AM_INIT_AUTOMAKE(sstrings,1.0.3)

dnl Package name
GENERIC_LIBRARY_NAME=sstrings

dnl Setting C as the language
AC_GNU_SOURCE
AC_LANG(C)

dnl Checks for programs.

dnl Checks for libraries.
AC_PROG_LIBTOOL

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(ctype.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for library functions.

AC_PROG_INSTALL
dnl AC_SUBST(RANLIB)

AC_OUTPUT(Makefile sstrings/Makefile docs/Makefile)

cat << EOF

configure successfully executed.
Type in 'make' and 'make install' to install

Bugs reports @ pabloy@pcpool.mathematik.uni-freiburg.de
EOF
