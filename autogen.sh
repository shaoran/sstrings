#!/bin/sh

dir=`echo "$0" | sed 's,[^/]*$,,'`
test "x${dir}" = "x" && dir='.'

if test "x`cd "${dir}" 2>/dev/null && pwd`" != "x`pwd`"
then
    echo "This script must be executed directly from the source directory."
    exit 1
fi

rm -f -r config.cache acconfig.h autom4te.cache

# for libtoolize
[ ! -f ./aclocal.m4 ] && aclocal-1.9	

echo -e "\033[1;32m *\033[0;0m Executing libtoolize..." 			&& \
libtoolize --force							&& \
echo -e "\033[1;32m *\033[0;0m Generating alocal.m4..." 			&& \
aclocal-1.9									&& \
echo -e "\033[1;32m *\033[0;0m Generating configure..."			&& \
autoconf								&& \
echo -e "\033[1;32m *\033[0;0m Generating config.h.in..."                  && \
autoheader                                                                && \
echo -e "\033[1;32m *\033[0;0m Generating Makefile, src/Makefile"	&& \
automake-1.9 --add-missing --gnu  && exit 0

exit 1
