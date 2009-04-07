/* test.c
 * Author: Pablo Yanez Trujillo <yanezp@informatik.uni-freiburg.de>
 * Licence: GPL-2
 * This file is the target file for make test */

#include <stdio.h>
#include <sstrings2.h>
#include <stdlib.h>
#include <ctype.h>

#define YES printf("yes\n");
#define NO printf("no\n"); 

void print_message(const char *msg)
{
	printf("checking %s... ", msg);
}

int myblank(int c)
{
	return isblank(c) || c == '\n';
}

int main(void)
{
	int res;
	char *tmp;
	ss_string *str1, *str2, *str3;

	/* TEST 1 */

	ss_errno = SS_NOERR;

	#ifdef USEGC
		if(!ss_string_enable_gc())
		{
			printf("ERROR: could not start garbage collector.\n");
			#undef USEGC
		}
	#endif

	print_message("whether a ss_string object can be generated");

	str1 = ss_string_new("Hello");

	if(!str1)
	{
		ss_string_perror("ss_string_new");
		NO
		exit(1);
	}
	
	YES

	/* TEST 2 */
	
	ss_errno = SS_NOERR;

	str2 = ss_string_new("Hello");

	if(!str2)
	{
		ss_string_perror("ss_string_new");
		ss_string_free(str1, 1);
		exit(1);
	}

	print_message("whether ss_string_free works properly");

	tmp = ss_string_free(str2, 0);

	if(!tmp)
	{
		NO
		ss_string_free(str1, 1);
		exit(1);
	}

	ss_string_gc_free(tmp);

	YES

	/* TEST 3 */

	ss_errno = SS_NOERR;

        str2 = ss_string_new(" World");

        if(!str2)
        {
                ss_string_perror("ss_string_new");
                ss_string_free(str1, 1);
                exit(1);
        }

	print_message("for ss_string_append family");
	
	if(!ss_string_append(str1, str2))
	{
		NO
		ss_string_perror("ss_string_append");
		ss_string_free(str1, 1);
		ss_string_free(str2, 1);
		exit(1);
	}

	if(strcmp(str1->str, "Hello World"))
	{
		NO
		fprintf(stderr, "The resulting string was not expected\n");
		ss_string_free(str1, 1);
		ss_string_free(str2, 1);
		exit(1);
	}

	YES

	/* TEST 4 */

	ss_errno = SS_NOERR;

	print_message("for ss_string_delete");

	if(!ss_string_delete(str1,4, 5)) /* It should be Hellld */
	{
		NO
		ss_string_perror("ss_string_delete");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	}

	if(strcmp(str1->str, "Hellld"))
	{
		NO
                fprintf(stderr, "The resulting string was not expected\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	}

	YES;

	/* TEST 5 */

	ss_errno = SS_NOERR;
	
	print_message("for ss_string_copy family");

	if(!ss_string_copy_c_string(str1, "abcdefghij"))
	{
		NO
		ss_string_perror("ss_string_copy_c_string");
		ss_string_free(str1, 1);
		ss_string_free(str2, 1);
		exit(1);
	}

	if(strcmp(str1->str, "abcdefghij"))
	{
		NO
                fprintf(stderr, "The resulting string was not expected\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	}

	YES

	/* TEST 6 */

	ss_errno = SS_NOERR;
	
	if(!ss_string_copy_c_string(str1, "abcOPQ123OPQxyzOPQ122"))
	{
		fprintf(stderr, "Could not copy string, test aborted\n");
		ss_string_perror("ss_string_copy_c_string");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	}

	print_message("for ss_string_crop family");

	res = ss_string_crop_c_string(str1, "+++");

	if(!res)
	{
		NO
		ss_string_perror("ss_string_crop_c_string");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	} else if(res != -1) {
		NO
		fprintf(stderr, "The string '+++' should not be found, ss_string_crop_c_string thinks it has found it\n");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	}

	ss_errno = SS_NOERR;

	res = ss_string_crop_c_string(str1, "OPQ");

	if(!res)
	{
		NO
                ss_string_perror("ss_string_crop_c_string");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	} else if(res == -1) {
		NO
                fprintf(stderr, "The string '+++' should be found, ss_string_crop_c_string didn't\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	}

	if(strcmp(str1->str, "abc123OPQxyzOPQ122"))
	{
		NO
                fprintf(stderr, "The resulting string was not expected\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
	}

	YES


	ss_errno = SS_NOERR;


	print_message("for ss_string_rcrop family");

        res = ss_string_rcrop_c_string(str1, "+++");

        if(!res)
        {
                NO
                ss_string_perror("ss_string_rcrop_c_string");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
        } else if(res != -1) {
                NO
                fprintf(stderr, "The string '+++' should not be found, ss_string_crop_c_string thinks it has found it\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
        }

        ss_errno = SS_NOERR;

        res = ss_string_rcrop_c_string(str1, "OPQ");

        if(!res)
        {
                NO
                ss_string_perror("ss_string_rcrop_c_string");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
        } else if(res == -1) {
                NO
                fprintf(stderr, "The string '+++' should be found, ss_string_crop_c_string didn't\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
        }

        if(strcmp(str1->str, "abc123OPQxyz122"))
        {
                NO
                fprintf(stderr, "The resulting string was not expected\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                exit(1);
        }

        YES

	/* TEST 7 */

	ss_errno = SS_NOERR;

	if(!ss_string_copy_c_string(str1, "This World Is not a World where you can say it is a World."))
	{
		fprintf(stderr, "Unexpected error while copying. Test aborted\n");
		ss_string_free(str1, 1);
	        ss_string_free(str2, 1);
		exit(1);
	}

	/* cut the first ' ' of str2 */

	if(!ss_string_delete(str2, 0, 1))
	{
		fprintf(stderr, "Could not delete the first blank character. Test aborted\n");
		ss_string_free(str1, 1);
		ss_string_free(str2, 1);
		exit(1);
	}

	str3 = ss_string_new("Sea");

	if(!str3)
	{
		fprintf(stderr, "No memory left. Test aborted\n");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
		exit(1);
	}

	print_message("for ss_string_replace family");

	res = ss_string_replace(str1, str3, str2);

	if(!res)
	{
		NO
		ss_string_perror("ss_string_replace");
		ss_string_free(str1, 1);
		ss_string_free(str2, 1);
		ss_string_free(str3, 1);
		exit(1);
	} else if(res != -1) {
		NO
		fprintf(stderr, "Sea should not be found, ss_string_replace did it!\n");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	res = ss_string_replace(str1, str2, str3);

	if(!res)
	{
		NO
                ss_string_perror("ss_string_replace");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	} else if(res == -1) {
		NO
		fprintf(stderr, "World should be found, ss_string_replace didn't!\n");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	if(strcmp(str1->str, "This Sea Is not a World where you can say it is a World."))
	{
		NO
		fprintf(stderr, "The resulting string was not expected\n");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	YES




	print_message("for ss_string_rreplace family");

        res = ss_string_replace_c_string(str1, "sssea", str2->str);

        if(!res)
        {
                NO
                ss_string_perror("ss_string_rreplace_c_string");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
        } else if(res != -1) {
                NO
                fprintf(stderr, "sssea should not be found, ss_string_rreplace_c_string did it!\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
        }

        res = ss_string_rreplace(str1, str2, str3);

        if(!res)
        {
                NO
                ss_string_perror("ss_string_rreplace");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
        } else if(res == -1) {
                NO
                fprintf(stderr, "World should be found, ss_string_rreplace didn't!\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
        }

        if(strcmp(str1->str, "This Sea Is not a World where you can say it is a Sea."))
        {
                NO
                fprintf(stderr, "The resulting string was not expected\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
        }

        YES

	/* TEST 8 */

	ss_errno = SS_NOERR;

	print_message("for ss_string_insert family");

	if(!ss_string_insert(str1, str2, 8))
	{
		NO
		ss_string_perror("ss_string_insert");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	if(strcmp(str1->str, "This SeaWorld Is not a World where you can say it is a Sea."))
	{
		NO
                fprintf(stderr, "The resulting string was not expected\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	YES

	/* TEST 9 */

	ss_errno = SS_NOERR;

	if(!ss_string_copy_c_string(str1, "    \n\n\n      \t\t\tHello, World                              \t\n "))
	{
		fprintf(stderr, "Could not copy string, test aborted\n");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	print_message("for ss_string_trim family");

	if(!ss_string_trim(str1, myblank))
	{
		NO
		ss_string_perror("ss_string_trim");
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	if(strcmp(str1->str, "Hello, World"))
	{
		NO
                fprintf(stderr, "The resulting string was not expected\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	YES

	/* TEST 10 */

	ss_errno = SS_NOERR;

	if(!ss_string_copy_c_string(str2, "Hello, World"))
	{
		fprintf(stderr, "Could not copy string, test aborted\n");
                ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	print_message("for ss_string_strcmp");

	if(ss_string_strcmp(str1, str2))
	{
		NO
		ss_string_free(str1, 1);
                ss_string_free(str2, 1);
                ss_string_free(str3, 1);
                exit(1);
	}

	YES

	
	#ifndef USEGC

	ss_string_free(str1, 1);
	ss_string_free(str2, 1);
	ss_string_free(str3, 1);

	#endif

	return 0;
}
