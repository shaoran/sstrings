/**
 * Filename: sstrings/sstrings.c
 * Author: Pablo Yanez Trujillo <rex@supertux.homelinux.org>
 *
 * This is free software. Please read the file ../COPYING if you
 * want to use/edit/distribuite this source file.
 * This source file is protected by the GNU GPL-2
 *
 * NOTE: There is NO  warranty; not even for MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.
**/


/**
 * Version: 1.0.3
 * Please read the TODO, NEWS, BUGS files
 *
 * Last change: 24.03.2005 by Pablo, alias Supertux
 *
 **/

#include <sstrings.h>

short ss_errno; /* This variable must be rest to SS_NO_ERROR manually by the user */



char* addchar(char** buffer, int c)
{
	size_t len;
	char* tmp;
	
	if(!buffer)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}


	if(!*buffer)
	{
		/* allocate 2 Bytes of memory */
		tmp = malloc(!c ? 1 : 2);

		if(!tmp)
		{
			ss_errno = SS_NO_MEM;
			return NULL;
		}

		*tmp = c;
		*(tmp + (!c ? 0 : 1))=0;

		*buffer = tmp;
		
	} else {

		if(!c) return *buffer;
		
		/* read all size and allocate old size+2 new characters */
		len = strlen(*buffer) + 2;

		tmp = realloc(*buffer,len);

		if(!tmp)
		{
			ss_errno = SS_NO_EXPAND;
			return NULL;
		}

		if(tmp != *buffer)
			*buffer = tmp;
		
		*(tmp + len - 2) = c;
	        *(tmp + len - 1) = 0;
	}

	return *buffer;
}




char* appendnstring(char** buffer, const char* string, size_t len)
{
	size_t buf_len, str_len;
	char* tmp, *new_tmp;

	
	if(!buffer || !string)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	/* if len > strlen(string) then set len to strlen(string) */

	str_len = strlen(string); /* hope string is 0-terminated */

	if(len > str_len)
		len = str_len;

	/* if len == 0 then do nothing and return *buffer.
	 * string may be an empty string, in this case len is 0, too.
	 * In this case, allocate only one byte of memory and set it
	 * to 0 and return it. If string is empty then len will be 0
	 */

	if(!*buffer && !len) /* ok, *buffer is NULL and len==0 and string may be empty (if not then do as if) 
				if *buffer is NULL and string is empty (the case *buffer==NULL and !*string) is 
				also there! */
	{
		tmp = NULL;
		tmp = addchar(&tmp, 0);

		if(!tmp) return NULL;

		return *buffer = tmp;

	} else if(!len) /* len==0, *buffer is not NULL, then do nothing */
		return *buffer;

	/* ok, *string != 0 and len != 0, let's rock :) */
	
	if(!*buffer)
	{
		/* allocate len+1 bytes */
		
		tmp = malloc(len + 1);

		if(!tmp)
		{
			ss_errno = SS_NO_MEM;
			return NULL;
		}

		/* what if *string==0 ? */
		if(!*string)
		{
			**buffer = *string;
			return *buffer;
		}

		/* be sure, avoid using memcpy, strcpy, etc. Set \0 by yourself */
		memmove(tmp, string, len);
		*(tmp + len) = 0; /* set \0 by yourself */

		*buffer = tmp;
		
	} else {
		/* a little bit harder. Try to reallocate the space */

		buf_len = strlen(*buffer);


		tmp = realloc(*buffer, buf_len+len+1);


		if(!tmp)
		{
			ss_errno = SS_NO_EXPAND;
			return NULL;
		}

		if(tmp != *buffer)
		{
			/* printf("\t***DEBUG: memory changed from %x to %x**\n", *buffer, tmp); */
			*buffer = tmp;
		}

		/* go to the \0 of *buffer */
		new_tmp = tmp + buf_len;

		memmove(new_tmp, string, len); /* again, set \0 by yourself */

		new_tmp += len;

		*new_tmp = 0;
	}


	return *buffer;
}




char* appendstring(char** buffer, const char* string)
{
	return appendnstring(buffer, string, string ? strlen(string) : 0);
}




char* stringnappend(char** buffer, const char* string, size_t len)
{
	size_t buf_len, str_len;
	char* tmp, *new_tmp;

	if(!buffer || !string)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	/* same cases as appendnstring */

	str_len = strlen(string); /* hope string is 0-terminated */

	if(len > str_len)
		len = str_len;

	if(!*buffer && !len)
	{
		tmp = NULL;
                tmp = addchar(&tmp, 0);
                
		if(!tmp) return NULL;
 
		return *buffer = tmp;
	} else if(!len)
		return *buffer;
	
	if(!*buffer)
	{
		*buffer = appendnstring(buffer, string, str_len);
		return *buffer;
	}

	/* don't need at this point the else anymore */
	
	/* first of all increment the allocated space and move the old
	 * content, including the \0 */

	buf_len = strlen(*buffer);

	tmp = realloc(*buffer, buf_len + len + 1);

	if(!tmp)
	{
		ss_errno = SS_NO_EXPAND;
		return NULL;
	}

	if(tmp != *buffer)
		*buffer = tmp;

	new_tmp = tmp + len;
	memmove(new_tmp, *buffer, buf_len);

	/* set \0 by yourself. *buffer is 0-terminated, but be sure */
	*(tmp + buf_len + len) = 0;

	memmove(tmp, string, len);

	return *buffer;
}




char* stringappend(char** buffer, const char* string)
{
	return stringnappend(buffer, string, string ? strlen(string) : 0);
}




char* str2sstr(const char* string)
{
	char* tmp = NULL;

	if(!string)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	tmp = appendnstring(&tmp, string, strlen(string));

	return tmp;
}




char* subsstr(char** buffer, size_t pos, size_t len)
{
	size_t buf_len;
	char* tmp;

	if(!buffer)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	} else if(!*buffer && !len)
	{
		ss_errno = SS_NO_ERROR;
		return NULL;
	} else if(*buffer && !len)
		return NULL;

	/* if len > strlen(*buffer) ==> len = strlen(*buffer) */

	buf_len = strlen(*buffer);

	if(!buf_len) 
	{
		ss_errno = SS_EMPTY_STR;
		return NULL;
	}

	if(pos > buf_len - 1) pos = buf_len - 1; /* we can make here so
						    that *buffer is returned instead of
						    changing pos */

	if(len > buf_len - pos) len = buf_len - pos;

	tmp = *buffer + pos;

	memmove(*buffer, tmp, len);

	tmp = *buffer;
	*(tmp + len) = 0;

	/* for this function and for all other functions.
	 * if realloc fails (what it may be in a very very
	 * strange case) then return the old buffer.
	 *
	 */

	tmp = realloc(*buffer, strlen(*buffer) + 1);

	if(tmp && (tmp != *buffer))
		*buffer = tmp;

	return *buffer;
	
}




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




char* substr(const char* haystack, const char* needle)
{
	char* tmp;
	
	if(!haystack || !needle)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!*needle)
		return str2sstr(haystack);

	tmp = strstr(haystack, needle);

	if(!tmp)
	{
		ss_errno = SS_NEEDLE_NOT_FOUND;
		return NULL;
	}

	
	return str2sstr(tmp);
}




char* subrstr(const char* haystack, const char* needle)
{
	char* tmp;

	if(!haystack || !needle)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!*haystack || !*needle)
		return str2sstr(haystack);

	tmp = strrstr(haystack, needle);

	if(!tmp)
	{
		ss_errno = SS_NEEDLE_NOT_FOUND;
		return NULL;
	}

	return str2sstr(tmp);
}




char* delsstr(char** buffer, size_t pos, size_t len)
{
	char* tmp, *tmp2;
	size_t str_len;
	
	if(!buffer)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!*buffer) return *buffer;

	str_len = strlen(*buffer);

	if(pos > str_len - 1) pos = str_len - 1; /* we can make so
						    that *buffer is returned instead of
						    changing pos */

	if(len > str_len - pos) len = str_len - pos;

	if(!len) return *buffer;

	tmp = *buffer + pos;
	tmp2 = tmp + len;

	str_len = strlen(tmp2);
	
	if(!str_len)
	{
		/* cut the 'len' last characters. */
		tmp = *buffer;
		tmp[strlen(tmp) - len] = 0;
		return *buffer;
	}

	memmove(tmp, tmp2, str_len);

	/* set 0 */
	tmp = *buffer;

	tmp[pos + str_len] = 0;
	
	str_len = strlen(tmp);

	tmp = realloc(*buffer, str_len + 1);

	if(tmp && (tmp != *buffer))
		*buffer = tmp;

	return *buffer;
}




char* cutsstr(char** buffer, const char* needle)
{
	char* tmp;
	size_t len, pos;
	
	if(!buffer || !needle)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!*needle) return *buffer;

	tmp = strstr(*buffer, needle);

	if(!tmp)
	{
		ss_errno = SS_NEEDLE_NOT_FOUND;
		return *buffer;
	}

	pos = tmp - *buffer;
	len = strlen(needle);

	tmp = delsstr(buffer, pos, len);

	if(!tmp) return NULL;

	if(tmp != *buffer)
		*buffer = tmp;

	return *buffer;
}




char* cutrsstr(char** buffer, const char* needle)
{
        char* tmp;
        size_t len, pos;

        if(!buffer || !needle)
        {
                ss_errno = SS_NULL_BUFFER;
                return NULL;
        }

        if(!*needle) return *buffer;

        tmp = strrstr(*buffer, needle);

        if(!tmp)
        {
                ss_errno = SS_NEEDLE_NOT_FOUND;
                return *buffer;
        }

        pos = tmp - *buffer;
        len = strlen(needle);

        tmp = delsstr(buffer, pos, len);

        if(!tmp) return NULL;

        if(tmp != *buffer)
                *buffer = tmp;

        return *buffer;
}




char* ltrim(char* buffer)
{
	if(!buffer)
		return NULL;

	while(isspace(*buffer))
		buffer++;

	return buffer;
}




char* rtrim(char* buffer)
{
	size_t len;
	char* tmp;
	if(!buffer)
		return NULL;

	len = strlen(buffer);

	if(!len) return buffer;

	tmp = buffer + len - 1;
	
	while(isspace(*tmp) && (tmp != buffer))
	{
		*tmp = 0;
		tmp--;
	}

	*tmp = isspace(*tmp) ? 0 : *tmp;

	return buffer;
}




char* trim(char* buffer)
{
	return rtrim(ltrim(buffer));
}




char* sstrltrim(char** buffer)
{
	char* tmp;
	size_t len;
	
	if(!buffer)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!*buffer)
	{
		ss_errno = SS_NULL_TRIM;
		return NULL;
	}

	tmp = ltrim(*buffer);

	len = strlen(tmp);

	memmove(*buffer, tmp, len);

	/* set \0 by yourself */
	*(*buffer + len) = 0;

	tmp = realloc(*buffer, len + 1);

	if(tmp && (tmp != *buffer))
		*buffer = tmp;

	return *buffer;
}




char* sstrrtrim(char** buffer)
{
	char* tmp;
	size_t len;

	if(!buffer)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!*buffer)
	{
		ss_errno = SS_NULL_TRIM;
		return NULL;
	}

	rtrim(*buffer);

	len = strlen(*buffer);

	tmp = realloc(*buffer, len + 1);

	if(tmp && (tmp != *buffer))
		*buffer = tmp;

	return *buffer;
}




char* sstrtrim(char** buffer)
{
	char* tmp;

	if(!buffer)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!*buffer)
	{
		ss_errno = SS_NULL_TRIM;
		return NULL;
	}

	/* important, right trim before of ltrim */

	tmp = sstrrtrim(buffer);

	if(!tmp)
	{
		/* ss_errno is set by sstrrtrim */
		return NULL;
	}

	if(tmp != *buffer)
		*buffer = tmp;

	tmp = sstrltrim(buffer);

	if(!tmp)
	{
		/* ss_errno is set by sstrrtrim */
		return NULL;
	}

	if(tmp != *buffer)
		*buffer = tmp;

	return *buffer;
}





/* Functions added in the 1.0.3 version */

static char* __replacestr(char** buffer, const char* haystack, const char* needle, size_t len, size_t search)
{
	size_t buff_len[5] = {0,0,0,0,0}; /* 0 => haystack, 1 => needle, 
					 2 => move, 3=> new len,
					 4 => new found pos */

	char* found, *move, *tmp;

	
	if(!buffer || !haystack || !needle)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!len || !*buffer)
	{
		ss_errno = SS_NO_ERROR;
		return *buffer;
	}

	buff_len[0] = strlen(haystack);
	buff_len[1] = strlen(needle);

	if(len > buff_len[1])
		len = buff_len[1];

	found = search ? strstr(*buffer, haystack) : strrstr(*buffer, haystack);

	if(!found)
	{
		ss_errno = SS_NEEDLE_NOT_FOUND;
		return NULL;
	}

	move = found + buff_len[0];

	/* set move to NULL if *move==0, because nothing
	 * must be copied. */

	if(*move)
	{
		move = str2sstr(move);

		if(!move) return NULL;
	} else move = NULL;

	buff_len[2] = move ? strlen(move) : 0;
	buff_len[4] = found - *buffer;

	buff_len[3] = buff_len[4] + len + buff_len[2];

	/* resize buffer */

	tmp = realloc(*buffer, buff_len[3] + 1);

	if(!tmp)
	{
		ss_errno = SS_NO_EXPAND;
		if(move) free(move);
		return NULL;
	}

	if(tmp != *buffer)
		*buffer = tmp;

	/* set new found (after realloc *buffer may have been changed) */

	found = *buffer + buff_len[4];

	memmove(found, needle, len);

	found = *buffer + buff_len[4] + len;

	if(move)
	{
		memmove(found, move, buff_len[2]);

		found = *buffer + buff_len[3];
	}
	
	*found = 0;

	if(move) free(move);
	
	return *buffer;
}


static char* __replacelaststr(char** buffer, const char* haystack, const char* needle, size_t len, int search)
{
        int need_len;

        if(!needle)
        {
                ss_errno = SS_NULL_BUFFER;
                return NULL;
        }

        need_len = strlen(needle);

        if(need_len < len)
                len = need_len;

        if(need_len != len)
                needle += need_len - len;

        return __replacestr(buffer, haystack, needle, len, search);
}




char* insertnstring(char** buffer, const char* src, size_t pos, size_t len)
{
	char* tmp = NULL, *move;
	size_t src_len, buf_len;
	
	if(!buffer || !src)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!len)
	{
		ss_errno = SS_NO_ERROR;
		return *buffer;
	}

	src_len = strlen(src);

	if(len > src_len) len = src_len;

	if(!*buffer)
		return appendnstring(&tmp, src, len);

	buf_len = strlen(*buffer);
	
	if(pos > buf_len) pos = buf_len + 1;

	if(pos == buf_len + 1)
		return appendnstring(buffer, src, len);

	if(!pos)
		return stringnappend(buffer, src, len);

	
	tmp = realloc(*buffer, buf_len + len + 1);

	if(!tmp)
	{
		ss_errno = SS_NO_EXPAND;
		return NULL;
	}

	if(tmp != *buffer)
		*buffer = tmp;

	tmp += pos; /* set tmp to the position where it must be inserted */
	move = tmp + len; /* set move to the position where the string must be moved */

	memmove(move, tmp, buf_len - pos + 1); /* +1 to copy \0 */

	/* 2nd block */

	memmove(tmp, src, len);

	return *buffer;
}




char* insertstring(char** buffer, const char* src, size_t pos)
{
	return insertnstring(buffer, src, pos, src ? strlen(src) : 0);
}




char* insertrnstring(char** buffer, const char* src, size_t pos, size_t len)
{
	size_t src_len;
	
	if(!src)
	{
		ss_errno = SS_NULL_BUFFER;
		return NULL;
	}

	if(!len)
	{
		ss_errno = SS_NO_ERROR;
		return *buffer;
	}

	src_len = strlen(src);

	if(len > src_len) len = src_len;

	src += src_len - len;

	return insertstring(buffer, src, pos);
}


char* replacestring(char** buffer, const char* haystack, const char* needle)
{
	/* wrapper of replacenstring 1*/
	return replacenstring(buffer, haystack, needle, needle ? strlen(needle) : 0);
}



char* replacerstring(char** buffer, const char* haystack, const char* needle)
{
	return replacernstring(buffer, haystack, needle, needle ? strlen(needle) : 0);
}




char* replacenstring(char** buffer, const char* haystack, const char* needle, size_t len) /* 1 */
{
	return __replacestr(buffer, haystack, needle, len, 1);
}




char* replacenlaststring(char** buffer, const char* haystack, const char* needle, size_t len)
{
	return __replacelaststr(buffer, haystack, needle, len, 1);
}




char* replacernstring(char** buffer, const char* haystack, const char* needle, size_t len)
{
	return __replacestr(buffer, haystack, needle, len, 0);
}




char* replacernlaststring(char** buffer, const char* haystack, const char* needle, size_t len)
{
	return __replacelaststr(buffer, haystack, needle, len, 0);
}



int emptystring(const char* src)
{
	size_t len, i;
	if(!src)
		return -1;

	if(!*src)
		return 1;

	len = strlen(src);

	for(i=0; i<len; ++i)
	{
		if(!isspace(src[i]))
			return 0;
	}

	return 1;
}
