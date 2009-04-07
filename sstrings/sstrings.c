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
 * Version: 0.0.1
 * This code is the first release of this tiny library.
 *
 * Last change: 23.02.2005 by Pablo, alias Supertux
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
		*(tmp+ (!c ? 0 : 1))=0;

		*buffer = tmp;
		
	} else {

		if(!c) return *buffer;
		
		/* read all size and allocate old size+2 new characters */
		len = strlen(*buffer)+2;

		tmp = realloc(*buffer,len);

		if(!tmp)
		{
			ss_errno = SS_NO_EXPAND;
			return NULL;
		}

		if(tmp != *buffer)
			*buffer = tmp;
		
		*(tmp+len-2) = c;
	        *(tmp+len-1) = 0;
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
		
		tmp = malloc(len+1);

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
		*(tmp+len) = 0; /* set \0 by yourself */

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

		if(tmp!=*buffer)
			*buffer = tmp;

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
	return appendnstring(buffer, string, strlen(string));
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
	return stringnappend(buffer, string, strlen(string));
}

char* str2sstr(const char* string)
{
	char* tmp = NULL;

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
		tmp[strlen(tmp) - 1 - len] = 0;
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

	/* later version should call realloc to make the buffer smaller */
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
