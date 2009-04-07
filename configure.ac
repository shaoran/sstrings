AC_PREREQ(2.59)
dnl Process this file with autoconf to produce a configure script.
AC_INIT(sstrings/sstrings2.c)
dnl Setting Package name for automake
AM_INIT_AUTOMAKE(sstrings,2.0.2)
AM_CONFIG_HEADER(config.h)

dnl Package name
GENERIC_LIBRARY_NAME=sstrings

dnl Setting C as the language
AC_GNU_SOURCE

dnl Checks for programs.

AC_MSG_CHECKING([whether valgrind is installed])

valgrind_path=`which valgrind 2>/dev/null`

if test -z "${valgrind_path}" ; then
	AC_MSG_RESULT([no])
else
	AC_MSG_RESULT([yes, ${valgrind_path}])
fi

AC_SUBST(valgrind_path)

dnl Checks for libraries.
AC_PROG_LIBTOOL

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for library functions.

AC_PROG_INSTALL
dnl AC_SUBST(RANLIB)

AC_OUTPUT(Makefile 
	  sstrings/Makefile 
	  docs/Makefile 
	  docs/man/Makefile 
	  docs/html/Makefile
	  docs/html/images/Makefile
	  docs/html/style/Makefile)

cat << EOF

configure successfully executed.
Type in 'make' and 'make install' to install

Bugs reports to yanezp@informatik.uni-freiburg.de 
EOF
