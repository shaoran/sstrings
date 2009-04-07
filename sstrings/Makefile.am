EXTRA_DIST=test.c
installheaderfile=sstrings2.h
sourcefiles=sstrings2.c ss_gc.c
headerfiles=$(installheaderfile) ss_gc.h

lib_LTLIBRARIES=libsstrings2.la
libsstrings2_la_SOURCES=$(sourcefiles) $(headerfiles)

# before we generate a package, delete "-ansi --std=c99"
AM_CFLAGS=-ansi -std=c99 -Wall -Wwrite-strings

library_includedir=$(includedir)
library_include_HEADERS = $(installheaderfile)

test: all-am
	gcc test.c -otest -I. -lsstrings2 -L./.libs
	gcc test.c -otest-gc -I. -lsstrings2 -L./.libs -DUSEGC
	LD_LIBRARY_PATH=./.libs @valgrind_path@ ./test
	echo
	LD_LIBRARY_PATH=./.libs @valgrind_path@ ./test-gc 


clean: clean-am
	rm -rf test test-gc


distclean: distclean-am
	rm -rf test test-gc
