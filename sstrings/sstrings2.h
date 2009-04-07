/**
 * Filename: sstrings/sstrings2.h
 * Author: Pablo Yanez Trujillo <yanezp@informatik.uni-freiburg.de>
 *
 * This is free software. Please read the file ../COPYING if you
 * want to use/edit/distribuite this source file.
 * This source file is protected by the GNU GPL-2
 *
 * NOTE: There is NO  warranty; not even for MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.
**/

#ifndef SSTRINGS2_H
#define SSTRINGS2_H

/* Standard C99 header files that don't need to be included by the user */

#include <string.h>	/* strlen, memmove stuff */


/* DON'T DECLARE THIS VARIABLE IN YOUR SOURCE CODE */
extern short ss_errno;

/* error model */
enum {
	SS_NOERR = 0,
	SS_NOMEM,
	SS_NULL,
	SS_EINVAL,
	SS_NUM	/* number of errors */
};

/* ss_string structure: saves information about the length
 * of the saved c-string (str) and how may bytes are 
 * allocated for the c-string */

typedef struct {
	char *str; /* pointer to allocated string */
	size_t len; /* length of the string */
	size_t bytes; /* how many bytes are allocated */
	
} ss_string;


ss_string *ss_string_new(const char *init);

/**
 * creates a new ss_string Object with init as the initial string
 * init cannot be NULL
 *
 * return: pointer to ss_string object on success
 * 	   NULL otherwise
 *
 * ss_errno = SS_NOMEM on failure,
 * ss_errno = SS_NULL if init is NULL
 **/


char *ss_string_free(ss_string *string, int free_cstring);

/**
 * frees the allocates memory for the ss_string object and its string
 *
 * if free_cstring is 0 then string->str is returned and not freed.
 * You must free it using ss_string_gc_free 
 *
 * return: string->str if free_cstring is 0
 * 	   NULL otherwise
 **/

int ss_string_enable_gc(void);

/**
 * starts the garbage collector, then you don't need to free
 * memory anymore
 *
 * return: 0 on failure
 * 	   != 0 otherwise
 *
 * Notice: You will need more memory for your programs since
 * the garbage collector has to remeber all allocated pointers
 **/


void ss_string_gc_free(const char *str);

/* ss_malloc wrapper */


void ss_string_perror(const char *msg);

/* works like perror */



int ss_string_append_c_string_l(ss_string *string, const char *src, size_t src_len);
int ss_string_append_c_string(ss_string *string, const char *src);
int ss_string_append(ss_string *string, const ss_string *src);

/** 
 * appends src into string
 *
 * return: 0 on error
 * 	   1 on success
 * 
 * ss_errno = SS_NULL if one parameter is NULL
 * ss_errno = SS_EINVAL if string->str is NULL / string not a valid ss_string object
 * ss_errno = SS_NOMEM if string->str could not be expaned
 *
 * NOTE: ss_string_append_c_string_l does *not* proof that src_len is the right length of src!!!
 **/


int ss_string_delete(ss_string *string, size_t pos, size_t len);

/**
 * deletes the substring at position pos with length len.
 *
 * return: 0 on error
 * 	   1 on success
 *
 * ss_errno = SS_NULL if one parameter is NULL
 * ss_errno = SS_EINVAL if string->str is NULL / string not a valid ss_string object
 * ss_errno = SS_EINVAL if pos > string->len - 1
 *
 * NOTE: len may be greater than the total number of characters to be deleted!
 *
 **/

char* strrstr(const char *haystack, const char *needle);

/* "extension" of the glibc, the same as strstr, but it searches the last occurence
 * of needle in haystack */


int ss_string_copy_c_string_l(ss_string *dest, const char *src, size_t len);
int ss_string_copy_c_string(ss_string *dest, const char *src);
int ss_string_copy(ss_string *dest, const ss_string *src);

/* copies src into a dest object.
 *
 * return: 0 on error
 * 	   1 in success
 *
 * ss_errno = SS_NULL if one parameter is NULL
 * ss_errno = SS_EINVAL if dest->str  is NULL / dest not a valid ss_string object
 *
 * NOTE: ss_string_copy_c_string_l does *not* proof that src_len is the right length of src!!!
 * 	 dest must be a valid ss_string object
 **/


int ss_string_crop_c_string_l(ss_string *string, const char *pattern, size_t patt_len);
int ss_string_crop_c_string(ss_string *string, const char *pattern);
int ss_string_crop(ss_string *string, const ss_string *pattern);

int ss_string_rcrop_c_string_l(ss_string *string, const char *pattern, size_t patt_len);
int ss_string_rcrop_c_string(ss_string *string, const char *pattern);
int ss_string_rcrop(ss_string *string, const ss_string *pattern);

/**
 * deletes pattern from string
 *
 * ss_string_crop deletes the *first* occurrence of pattern in string
 * ss_string_rcrop deletes the *last* occurence of pattern in string
 *
 * return: 0 on error
 * 	   1 in success
 * 	  -1 if pattern was not found (ss_errno ist not set to any value)
 *
 * ss_errno = SS_NULL if one parameter is NULL
 * ss_errno = SS_EINVAL if string,pattern->str is NULL / string not a valid ss_string object
 *
 * NOTE: either ss_string_crop_c_string_l nor ss_string_rcrop_c_string_l do *not*
 * proof that patt_len is the right length of pattern!!!
 *
 * TODO: (next versions?) same functions with max length of bytes to be croped of pattern
 **/


int ss_string_replace_c_string_l(ss_string *string, const char *pattern, size_t patt_len, const char *replace, size_t repl_len);
int ss_string_replace_c_string(ss_string *string, const char *pattern, const char *replace);
int ss_string_replace(ss_string *string, const ss_string *pattern, const ss_string *replace);

int ss_string_rreplace_c_string_l(ss_string *string, const char *pattern, size_t patt_len, const char *replace, size_t repl_len);
int ss_string_rreplace_c_string(ss_string *string, const char *pattern, const char *replace);
int ss_string_rreplace(ss_string *string, const ss_string *pattern, const ss_string *replace);

/**
 * replaces pattern with replace
 *
 * ss_string_replace replaces the *first* occurrence of pattern in string
 * ss_string_rreplace replaces the *last* occurrence of pattern in string
 *
 * return: 0 on error
 * 	   1 in success
 * 	  -1 if pattern was not found (ss_errno ist not set to any value)
 *
 * ss_errno = SS_NULL if one parameter is NULL
 * ss_errno = SS_EINVAL if string,pattern,replace->str is NULL / string not a valid ss_string object
 *
 * NOTE: either ss_string_replace_c_string_l nor ss_string_rreplace_c_string_l do *not*
 * proof that patt_len is the right length of pattern (same with repl_len)!!!
 *
 **/

int ss_string_insert_c_string_l(ss_string *string, const char *insert, size_t ins_len, size_t pos);
int ss_string_insert_c_string(ss_string *string, const char *insert, size_t pos);
int ss_string_insert(ss_string *string, const ss_string *insert, size_t pos);

/**
 * inserts the substring at position pos
 *
 * return: 0 on error
 * 	   1 on success
 *
 * ss_errno = SS_NULL if one parameter is NULL
 * ss_errno = SS_EINVAL if string->str is NULL / string not a valid ss_string object (same with insert)
 *
 * NOTE: if pos is greater than string->len - 1 then it the substring will be concatenated!
 *
 * TODO: (next versions?) same functions with max length of bytes to be inserted
 **/


int ss_string_ltrim(ss_string *string, int (*blank)(int));
int ss_string_rtrim(ss_string *string, int (*blank)(int));
int ss_string_trim(ss_string *string, int (*blank)(int));

/**
 * deletes the leading blank characters from the beginning and end of the string
 * 
 * return: 0 on error
 * 	   1 on success
 *
 * ss_errno = SS_NULL if one parameter is NULL
 * ss_errno = SS_EINVAL if string->str is NULL / string not a valid ss_string object
 *
 * NOTE: the second parameter is a function pointer and the function hast to be declared as
 * 	int func(int c)
 **/


int ss_string_strcmp(const ss_string *s1, const ss_string *s2);

#endif
