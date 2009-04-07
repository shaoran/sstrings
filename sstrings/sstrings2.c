/**
 * Filename: sstrings/sstrings2.c
 * Author: Pablo Yanez Trujillo <yanezp@informatik.uni-freiburg.de>
 *
 * This is free software. Please read the file ../COPYING if you
 * want to use/edit/distribuite this source file.
 * This source file is protected by the GNU GPL-2
 *
 * NOTE: There is NO  warranty; not even for MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.
**/



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sstrings2.h>
#include <ss_gc.h>
#include <stdio.h> /* for fprintf */


/* error variable */
short ss_errno;

/* reallocs memory for string->str if needed.
 * len is the new length of the string that has to be saved in string->str
 * if the new length is greater than the actual amount of
 * allocated bytes then we have to expand the memory.
 *
 * if the new length is less than 1/4 of the actual amount of allocated
 * bytes then the memory will be halved */

static int ss_string_alloc(ss_string *string, size_t len)
{
	char *tmp;

	if(!string)
		return 0;

	if(len + 1 > string->bytes)
	{
		/* we need more memory */

		tmp = ss_realloc(string->str, len + 1);

		if(!tmp)
			return 0;

		if(string->str != tmp)
			string->str = tmp;

		string->bytes = len + 1;

	} else if( 4 * (len + 1) < string->bytes ) {
		/* memory will be halved */

		tmp = ss_realloc(string->str, (string->bytes + string->bytes % 2)/2);

		if(!tmp) /* memory could not be halved, this is not a serious error */
			return 1;

		if(string->str != tmp)
                        string->str = tmp;

		string->bytes = (string->bytes + string->bytes % 2)/2;
	}

	return 1;
}

/* this function is a generic crop function which searches the pattern
 * using search */
static int ss_string_crop_c_string_l_choose(ss_string *string, const char *pattern, size_t patt_len, char* (*search)(const char*, const char*))
{
	char *tmp;

	if(!string || !pattern)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!string->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	} 

	tmp = search(string->str, pattern);

	if(!tmp)
		return -1;

	return ss_string_delete(string, tmp - string->str, patt_len);
}

/* this function is a generic replace function which searches the pattern
 * using search */

static int ss_string_replace_c_string_l_choose(ss_string *string, const char *pattern, size_t patt_len, const char *replace, size_t repl_len, char* (*search)(const char*, const char*))
{
	int resize_flag; /* ss_string_alloc hast to be called at the right moment, that
			    means when any part of string->str is freed */
	
	char *tmp, *move;
	int tmp_pos;

	if(!string || !pattern || !replace)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!string->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	tmp = search(string->str, pattern);

	if(!tmp)
		return -1;

	/* pattern was found. Now let's see whether we need more or less space 
	 * set resize_flag to 1 if we need to expand memory
	 * set resize_flag to 0 if we need to decrease memory */

	tmp_pos = tmp - string->str;

	resize_flag = (repl_len >= patt_len);

	if(resize_flag && !ss_string_alloc(string, string->len - patt_len + repl_len))
	{
		ss_errno = SS_NOMEM;
		return 0;
	}

	/* it could be that ss_realloc changed the pointer string->str */


	tmp = string->str + tmp_pos;

	move = tmp + patt_len; /* the first position after pattern in string->str */

	memmove(tmp + repl_len, move, string->str + string->len - move + 1); /* \0 must be copied */

	memmove(tmp, replace, repl_len); /* copy the replace string */

	string->len = string->len - patt_len + repl_len;

	/* if !resize_flag, then we should decrease the memory. If it fails, no problem with that */

	if(!resize_flag)
		ss_string_alloc(string, string->len);
	
	return 1;
}

ss_string* ss_string_new(const char *init)
{
	ss_string *tmp;
	size_t len;

	if(!init)
	{
		ss_errno = SS_NULL;
		return NULL;
	}

	tmp = ss_malloc(sizeof(ss_string));

	if(!tmp)
	{
		ss_errno = SS_NOMEM;
		return NULL;
	}
	
	
	len = strlen(init);

	tmp->len = 0;
	tmp->str = NULL;
	tmp->bytes = 0;

	if(!ss_string_alloc(tmp, len))
	{
		ss_errno = SS_NOMEM;
		ss_free(tmp);
		return NULL;
	}

	strcpy(tmp->str, init);

	tmp->len = len;

	return tmp;
}

char* ss_string_free(ss_string *string, int free_cstring)
{
	char *tmp = NULL;

	if(!string)
		return NULL;

	if(free_cstring)
	{
		ss_free(string->str);
		string->str = NULL;
	}

	tmp = string->str;

	string->str = NULL;
	string->len = string->bytes = 0;

	ss_free(string);

	return tmp;
}

int ss_string_enable_gc(void)
{
	return ss_use_gc();
}


void ss_string_gc_free(const char *str)
{
	ss_free((void*) str);
}


void ss_string_perror(const char *msg)
{
	short err;
	const char *errors[] = {
		"Success",
		"Not enough memory",
		"Input is a null pointer",
		"Invalid argument",
		"Unknown error" };

	if(ss_errno >= SS_NUM)
		err = SS_NUM;
	else
		err = ss_errno;

	if(msg && *msg)
		fprintf(stderr, "%s: %s\n", msg, errors[err]);
	else
		fprintf(stderr, "%s\n", errors[err]);
}


int ss_string_append_c_string_l(ss_string *string, const char *src, size_t src_len)
{
	char *tmp;

	if(!string || !src)
        {
                ss_errno = SS_NULL;
                return 0;
        }

        if(!string->str)
        {
                ss_errno = SS_EINVAL;
                return 0;
        }

	if(!ss_string_alloc(string, string->len + src_len))
	{
		ss_errno = SS_NOMEM;
		return 0;
	}

	tmp = string->str + string->len;

	memcpy(tmp, src, src_len);

	tmp[src_len] = 0;

	string->len += src_len;

	return 1;
}

int ss_string_append_c_string(ss_string *string, const char *src)
{
	if(!src)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	return ss_string_append_c_string_l(string, src, strlen(src));
}

int ss_string_append(ss_string *string, const ss_string *src)
{
	if(!src)
        {
                ss_errno = SS_NULL;
                return 0;
        }

	if(!src->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	return ss_string_append_c_string_l(string, src->str, src->len);
}

int ss_string_delete(ss_string *string, size_t pos, size_t len)
{
	char *tmp;

	if(!string)
	{
		ss_errno = SS_NULL;
		return 0;
	}
	
	if(!string->str || pos > string->len - 1)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	/* recalculate len if len is greater than number of characters to be deleted */

	len = pos + len > string->len ? string->len - pos : len;

	tmp = string->str + pos;

	memmove(tmp, tmp + len, string->len - (tmp - string->str) - len + 1); /* +1 for \0 */
	/* tmp - string->str is the lenthg of the characters that were not deleted */

	string->len -= len;

	/* it is not so bad if realloc cannot decrement the amount of allocated bytes */

	ss_string_alloc(string, string->len);

	return 1;
}




/* we should work on the inefficiency of this function, copied from sstrings-1.0.5 */
char* strrstr(const char *haystack, const char *needle)
{
        char* tmp, *tmp_old;
        size_t len;
        int counter;


        if(!haystack || !needle)
                return NULL;

        if(!*haystack && !*needle)
                return (char*) &haystack[0]; /* gcc generates a warning */
                                     /* other compiler may not compile this */
        else if(!*haystack)
                return NULL;
        else if(!*needle)
                return (char*) &haystack[0]; /* the same */

        tmp_old = tmp = (char*) &haystack[0]; /* the same */
        len = strlen(needle);

        counter = 0;

        while(1)
        {
                tmp = strstr(tmp, needle);

                if(!tmp)
                {
                        if(!counter) return NULL;
                        return tmp_old;
                }

                counter++;

                if(strlen(tmp)==len)
                        return tmp; /* the last strlen(needle)
                                           match with needle. */
                /* in this case, strlen(tmp_new > strlen(needle) */
                tmp_old = tmp;
                tmp += len; /* point to the nex character after needle */

        }

        return tmp_old;
}


int ss_string_copy_c_string_l(ss_string *dest, const char *src, size_t len)
{
	int flag; /* if new length is less than actual length and ss_realloc cannot decrease memory
		     then don't interpret this a an error */

	if(!dest || !src)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!dest->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	flag = len < dest->len ? 1 : 0;

	if(!ss_string_alloc(dest, len) && !flag) /* not enough memory, new length cannot be allocated */
	{
		ss_errno = SS_NOMEM;
		return 0;
	}

	strcpy(dest->str, src);

	dest->len = len;

	return 1;
}

int ss_string_copy_c_string(ss_string *dest, const char *src)
{
	if(!src)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	return ss_string_copy_c_string_l(dest, src, strlen(src));
}


int ss_string_copy(ss_string *dest, const ss_string *src)
{
	if(!src)
        {
                ss_errno = SS_NULL;
                return 0;
        }

	if(!src->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	return ss_string_copy_c_string_l(dest, src->str, src->len);
}




int ss_string_crop_c_string_l(ss_string *string, const char *pattern, size_t patt_len)
{
	return ss_string_crop_c_string_l_choose(string, pattern, patt_len, strstr);
}

int ss_string_crop_c_string(ss_string *string, const char *pattern)
{
	if(!pattern)
	{
		ss_errno = SS_NULL;
		return 0;
	}


	return ss_string_crop_c_string_l(string, pattern, strlen(pattern));
}

int ss_string_crop(ss_string *string, const ss_string *pattern)
{
	if(!pattern)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!pattern->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	return ss_string_crop_c_string_l(string, pattern->str, pattern->len);
}

int ss_string_rcrop_c_string_l(ss_string *string, const char *pattern, size_t patt_len)
{
	 return ss_string_crop_c_string_l_choose(string, pattern, patt_len, strrstr);
}

int ss_string_rcrop_c_string(ss_string *string, const char *pattern)
{
	if(!pattern)
        {
                ss_errno = SS_NULL;
                return 0;
        }

	return ss_string_rcrop_c_string_l(string, pattern, strlen(pattern));
}

int ss_string_rcrop(ss_string *string, const ss_string *pattern)
{
	if(!pattern)
        {
                ss_errno = SS_NULL;
                return 0;
        }
        
        if(!pattern->str)
        {
                ss_errno = SS_EINVAL;
                return 0;
        }

	return ss_string_rcrop_c_string_l(string, pattern->str, pattern->len);
}



int ss_string_replace_c_string_l(ss_string *string, const char *pattern, size_t patt_len, const char *replace, size_t repl_len)
{
	return ss_string_replace_c_string_l_choose(string, pattern, patt_len, replace, repl_len, strstr);
}

int ss_string_replace_c_string(ss_string *string, const char *pattern, const char *replace)
{
	if(!pattern || !replace)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	return ss_string_replace_c_string_l(string, pattern, strlen(pattern), replace, strlen(replace));
}

int ss_string_replace(ss_string *string, const ss_string *pattern, const ss_string *replace)
{
	if(!pattern || !replace)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!pattern->str || !replace->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	return ss_string_replace_c_string_l(string, pattern->str, pattern->len, replace->str, replace->len);
}


int ss_string_rreplace_c_string_l(ss_string *string, const char *pattern, size_t patt_len, const char *replace, size_t repl_len)
{
        return ss_string_replace_c_string_l_choose(string, pattern, patt_len, replace, repl_len, strrstr);
}

int ss_string_rreplace_c_string(ss_string *string, const char *pattern, const char *replace)
{
        if(!pattern || !replace)
        {
                ss_errno = SS_NULL;
                return 0;
        }

        return ss_string_rreplace_c_string_l(string, pattern, strlen(pattern), replace, strlen(replace));
}

int ss_string_rreplace(ss_string *string, const ss_string *pattern, const ss_string *replace)
{
        if(!pattern || !replace)
        {
                ss_errno = SS_NULL;
                return 0;
        }

        if(!pattern->str || !replace->str)
        {
                ss_errno = SS_EINVAL;
                return 0;
        }

        return ss_string_rreplace_c_string_l(string, pattern->str, pattern->len, replace->str, replace->len);
}



int ss_string_insert_c_string_l(ss_string *string, const char *insert, size_t ins_len, size_t pos)
{
	char *move;

	if(!string || !insert)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!string->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	if(pos > string->len - 1)
		return ss_string_append_c_string_l(string, insert, ins_len);

	
	if(!ss_string_alloc(string, string->len + ins_len))
	{
		ss_errno = SS_NOMEM;
		return 0;
	}

	move = string->str + pos;

	memmove(move + ins_len, move, string->str + string->len - move + 1); /* \o must be copied */

	memmove(move, insert, ins_len);


	string->len += ins_len;

	return 1;
}

int ss_string_insert_c_string(ss_string *string, const char *insert, size_t pos)
{
	if(!insert)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	return ss_string_insert_c_string_l(string, insert, strlen(insert), pos);
}

int ss_string_insert(ss_string *string, const ss_string *insert, size_t pos)
{
	if(!insert)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!insert->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	return ss_string_insert_c_string_l(string, insert->str, insert->len, pos);
}


int ss_string_ltrim(ss_string *string, int (*blank)(int))
{
	int i = 0;
	if(!string)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!string->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	while(string->str[i] && blank(string->str[i]))
		i++;

	memmove(string->str, string->str + i, string->len - i + 1);

	string->len -= i;
	
	/* it is not so bad if realloc cannot decrement the amount of allocated bytes */

	ss_string_alloc(string, string->len);

	return 1;
}

int ss_string_rtrim(ss_string *string, int (*blank)(int))
{
	int i;

	if(!string)
	{
		ss_errno = SS_NULL;
		return 0;
	}

	if(!string->str)
	{
		ss_errno = SS_EINVAL;
		return 0;
	}

	i = string->len - 1;

	while(i >= 0 && blank(string->str[i]))
		--i;

	string->str[i + 1] = 0;
	string->len = i + 1;

	/* it is not so bad if realloc cannot decrement the amount of allocated bytes */

	ss_string_alloc(string, string->len);
	
	return 1;
}

int ss_string_trim(ss_string *string, int (*blank)(int))
{
	if(!string)
        {
                ss_errno = SS_NULL;
                return 0;
        }

        if(!string->str)
        {
                ss_errno = SS_EINVAL;
                return 0;
        }

	if(ss_string_ltrim(string, blank))
		return ss_string_rtrim(string, blank);

	return 0;
}

int ss_string_strcmp(const ss_string *s1, const ss_string *s2)
{
	if(!s1 || !s2)
	{
		ss_errno = SS_NULL;
		return -1;
	}

	if(!s1->str || !s2->str)
	{
		ss_errno = SS_EINVAL;
		return -1;
	}

	return strcmp(s1->str, s2->str);
}
