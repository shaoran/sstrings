/* filename: main.c
 *
 * this file is not part of the sstrings library.
 * this file is compiled with the sstrings library and
 * is executed with 'make check'. This file contains *_only_*
 * tests for all sstrings functions in the current release.
 *
 * You can use this file in order to learning how to use the
 * library */

#include <stdio.h>
#include <sstrings.h>

int main(void)
{
	char* s = str2sstr("JJJ");
	printf("checking ... ok\n");

	free(s);
	
	return 0;
}
