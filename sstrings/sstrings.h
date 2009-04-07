/**
 * Filename: sstrings/sstrings.h
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
 * Synopsis: This is small collection of C function that handle with
 * C string most know as char*
 * Leading correctly with C strings is not simple and you should understand
 * how malloc/ralloc/etc work before you use it. If you don't want to have
 * unexpected buffer overflows then you should use this library.
 *
 * This code is explained well if you want to change some places of the code.
 * If you want to use the library, you should read the manual page sstrings(2)
 *
 **/

#ifndef SSTRINGS_H
#define SSTRINGS_H

/* Standard C99 header files that don't need to be included by the user */

#include <stdlib.h>	/* malloc/realloc/free stuff */
#include <string.h>	/* strlen, memmove stuff */
#include <ctype.h>	/* isspace */


/* Defining error constans */

/* DON'T DECLARE THIS VARIABLE */

extern short ss_errno;

#define SS_NO_ERROR 0
#define SS_NO_MEM 1
#define SS_NO_EXPAND 2
#define SS_NULL_BUFFER 3
#define SS_NEEDLE_NOT_FOUND 4
#define SS_NULL_TRIM 5
#define SS_EMPTY_STR 6

/**
 * You have always to set ss_errno to 0 or SS_NO_ERROR by yourself,
 * when a fucntion may set ss_errno to SS_NO_ERROR.
 * if you want to use this variable. If a function ends successfully
 * then ss_errno won't be SS_NO_ERROR; this variable is set only when
 * an error is generated.
 *
 * List of ERRORS
 *
 * Name              || value || Description
 * =========================================
 * SS_NO_ERROR          0        describes the state of no error
 * SS_NO_MEM            1        cannot execute malloc because of the memory
 * SS_NO_EXPAND         2        cannot execute realloc and expand the buffer
 * SS_NULL_BUFFER       3        you passed a NULL buffer, when a string is required
 *                               or a not NULL pointer is required
 * SS_NEEDLE_NOT_FOUND  4	 when the needle is not found (reasons: empty needle or
 * 				 the needle is really not found)
 * SS_NULL_TRIM		5	 *buffer in trim sstr*trim function is NULL. Cannot trim 
 * 				 anything
 * SS_EMPTY_STR		6	 When a buffer contains an empty string and it shouldn't
 * 				 be empty.
 * 
 **/

/**
 * NOTE: Don't forget to use free when you use one of this functions.
 * All functions need *buffer==NULL or a memory space that was
 * allocated with malloc. This won't work:
 *
 * 	char* x = "hel";
 * 	appendstring(&x,"lo");
 *
 * 	char x[] = "hel";
 * 	appendstring(&x,"lo");
 *
 * but this works fine
 * 
 * 	char* x = malloc(4);
 * 	if(!x) return ???
 *
 * 	strcpy(x, "hel");
 * 	x = appendstring(&x, "lo");
 * 	...
 * 	free(x);
 *
 *
 * NOTE: If the pointer adress of *buffer is changed, those functions call free
 * by themselve so that you don't need to free old content that are changed while
 * using realloc.
 *
 * NOTE: Using those functions means that the content of *buffer will be changed,
 * if you want to keep the original, you have to make a copy. You can do it with str2sstr,
 * since str2sstr doesn't check whether the memeroy block of `string` was 
 * manually allocated (with malloc/realloc) or not. This function makes only a copy!
 *
 **/

char* addchar(char** buffer, int c);

/**
 * addchar inserts a new character passed with c in buffer and returns the new
 * string, the content of buffer may be changes, addchar returns *buffer.
 * The output of this function is a 0-terminated string, NULL if the functions
 * fails. To determinate which error was produce, use the ss_errno variable.
 * Don't pass 0 (or '\0') with char. The function does not do anything in this case.
 *
 * If the content of buffer is NULL (*buffer==NULL) then the function will allocate
 * the required memory. Example:
 * 
 * 	char* strHello = NULL;
 * 	strHello = addchar(&strHello, 'H');
 *
 * In this case strHallo will have the "H" string, if strHallo is NULL then addchar have failed.
 * If the function fails, *buffer won't be changed
 **/

char* appendstring(char** buffer, const char* string);

/**
 * If *buffer != NULL then appendstring will append the whole string at the end of buffer.
 * if *buffer == NULL then string will be copied into buffer after having allocated the memory
 * for *buffer. The output of this function is a 0-terminated string, NULL if the functions
 * fails. string must be a 0-terminated string, if it is not then the output of the function is
 * unexpected. If string is an empty string, appendstring won't do anything, bit when *buffer is NULL
 * you will have the return of addchar(..., 0).
 *
 * *buffer and string my overlap.
 *
 **/

char* appendnstring(char** buffer, const char* string, size_t len);

/**
 * This function works like appendstring, in fact appendstring is a wrapper for
 * appendnstring. len determinates how many characters for string must be copied.
 * if len==0 nothing will be done and *buffer is returned, if len is greater than the length of string
 * then the length of the string will be assumed to be len.
 *
 * *buffer and string my overlap.
 *
 **/

char* stringappend(char** buffer, const char* string);

/**
 * Works the same like apendstring. The string is copied in buffer and the old content
 * of buffer is appended to string in *buffer.
 * Exp:
 *
 * 	char* strHello = NULL;
 * 	strHello = appendstring(&strHello, "llo"); // strHello points to "llo"
 * 	strHello = stringappend(&strHello, "He");  // strHello points to "Hallo"
 *
 * *buffer and string my overlap.
 *
 **/

char* stringnappend(char** buffer, const char* string, size_t len);

/**
 * Works the same like stringappend, in fact stringappend is a wrapper fort stringnappend.
 * len determinates how many characters for string must be copied. if len==0 nothing will be done
 * and *buffer will be returnded.
 * if len is greater than the length of string
 * then the length of the string will be assumed to be len.
 *
 * *buffer and string my overlap.
 * 
 **/

char* str2sstr(const char* string);

/**
 * wrapper of appendstring when *buffer == NULL
 *
 **/

char* subsstr(char** buffer, size_t pos, size_t len);

/**
 * returns the the string between position `pos` and
 * strlen(*buffer)-len.
 * Exp:
 *
 * 	char* text = NULL;
 * 	text = appendstring(&text, "abcdefgh");
 * 	text = substr(&text, 2, 5);
 * 	printf("%s\n", text);
 * 	...
 * 	free(text);
 *
 * The output is `cdefg`
 *
 * NOTE: If len==0 and *buffer NULL will be returned and ss_errno is set
 * to SS_NO_ERROR. If *buffer != NULL *buffer is returned
 *
 **/

char* strrstr(const char *haystack, const char *needle);

/**
 * see strstr(3). This function returns a pointer to the last
 * occurrence of needle in haystack. NULL is returnded, if needle is
 * not found on haystack.
 *
 * Try to make this functions safer than the sister function of th GNU GLIBC.
 * when haystack or needle are NULL pointers, return NULL.
 * strstr generates in those cases a segmentation fault.
 * 
 **/

char* substr(const char* haystack, const char* needle);
char* subrstr(const char* haystack, const char* needle);

/**
 * substr searches the first occurrence of needle in haystack and returns the string at that point.
 * subrstr searches the last occurrence of needle in haystack and returns the string at that point.
 *
 * Both function return a pointer to a new allocated memory block
 * so you can work with any other function of this library (I think
 * a function like the second one should have to be implemented in the ANSI C Standard)
 *
 * if needle==NULL then NULL is returned and ss_errno is set to SS_NULL_BUFFER.
 *
 * if *needle==0 then haystack is returned [1]
 *
 * if haystack==NULL then NULL is returned and ss_errno is set to SS_NULL_BUFFER.
 *
 * if *haystack==0 then haystack is returned [1]
 *
 * [1] This pointer is a pointer to memeory that was allocated with malloc 
 *
 **/

char* delsstr(char** buffer, size_t pos, size_t len);

/**
 * delete `len` characters of *buffer from position `pos`
 * NOTE: the first character is at the position 0, the last
 * at strlen(*buffer)-1. If `len` is greater than the characters
 * left, then len will be set to the next length, that matches
 * with the characters.
 * Exp:
 *
 *      char* text = NULL;
 *      text = appendstring(&text, "abcdefgh");
 *      text = delstr(&text, 2, 5);
 *      printf("%s\n", text);
 *      ...
 *      free(text);
 *
 * The output of printf is `ach`
 *
 *
 **/

char* cutsstr(char** buffer, const char* needle);
char* cutrsstr(char** buffer, const char* needle);

/**
 * cutsstr searches the first occurrence of needle in *buffer and delete this substring.
 * 
 * cutrsstr searches the last occurrence of needle in *buffer and delete this substring.
 * 
 * If needle is NULL then NULL is returned and
 * ss_errno is set to SS_NULL_BUFFER. *buffer won't be
 * changed.
 *
 * If needle is an empty string then NULL is returned and is set to SS_NEEDLE_NOT_FOUND
 *
 * If needle is not found NULL is returned,
 * ss_errno is set to SS_NEEDLE_NOT_FOUND and *buffer
 * won't be changed.
 *
 **/


/**
 * Trim functions!
 *
 * I think those functions should be implmented in the ANSI C Standard, but
 * they are not.
 *
 * I will implmente 3 function, ltrim, rtrim and trim in a C99 style and 3
 * function that are part of sstrings library. Those functions will call realloc to
 * make the buffer smaller.
 *
 **/

char* ltrim(char* buffer);

/**
 * cuts all left blank characters and returns a pointer to the first not blank
 * character.
 *
 * If buffer==NULL ltrim returns NULL
 *
 * NOTE: blank characters are proovef by isspace(3)
 **/

char* rtrim(char* buffer);

/**
 * The same, but only right blank characters. In thi case,
 * the returning pointers points to buffer, the \0 character is new replaced-
 *
 * NOTE: Make sure that buffer is not a const char*
 * 	char a[] = "but this      ";
 * 	rtrim("This won't work        ");
 * 	rtrim(a); 	
 *
 **/

char* trim(char* buffer);

/**
 * It's the same as rtrim(ltrim(buffer));
 **/


char* sstrltrim(char** buffer);
char* sstrrtrim(char** buffer);

/**
 * In fact, the same as ltrim/rtrim. The buffer size and the pointer may
 * change. You don't need to call free in that case
 * 
 **/

char* sstrtrim(char** buffer);

/**
 * In fact the same as trim (a little bit different)
 *
 * NOTE: if *buffer is a null pointer, then the functions will
 * return NULL and ss_errno will be set to SS_NULL_TRIM
 *
 **/











/* functions added in the 1.0.3 version */


char* insertstring(char** buffer, const char* src, size_t pos);

/**
 * This function inserts the string src into *buffer at position pos.
 * When src is NULL the function does anything. 
 *
 * The function returns a pointer to *buffer, that may change when the space is reallocated
 **/

char* insertnstring(char** buffer, const char* src, size_t pos, size_t len);

/**
 * This functions works like insertstring, but it inserts only the first len 
 * characters of src.
 *
 **/

char* insertrnstring(char** buffer, const char* src, size_t pos, size_t len);

/**
 * This functions works like insertstring, but it inserts only the last len 
 * characters of src.
 *  
 **/

char* replacestring(char** buffer, const char* haystack, const char* needle);

/**
 * searches for the first occurrence of haystack in *buffer and replaces with needle.
 * When haystack, needle of *buffer are NULL or empty strings, then nothing will be done
 *
 * The functions returns a pointer to *buffer.
 *
 **/

char* replacerstring(char** buffer, const char* haystack, const char* needle);

/**
 * searches for the last occurrence of haystack in *buffer and replaces with needle.
 * When haystack, needle of *buffer are NULL or empty strings, then nothing will be done
 *
 * The functions returns a pointer to *buffer.
 *
 **/

char* replacenstring(char** buffer, const char* haystack, const char* needle, size_t len);

/**
 * searches for the first occurrence of haystack in *buffer and replaces with the first len characters of needle.
 * When haystack, needle of *buffer are NULL or empty strings, then nothing will be done
 *
 * he functions returns a pointer to *buffer.
 *
 **/

char* replacenlaststring(char** buffer, const char* haystack, const char* needle, size_t len);

/**
 * searches for the first occurrence of haystack in *buffer and replaces with the last len characters of needle.
 * When haystack, needle of *buffer are NULL or empty strings, then nothing will be done
 *
 * he functions returns a pointer to *buffer.
 *
 **/

char* replacernstring(char** buffer, const char* haystack, const char* needle, size_t len);

/**
 * searches for the last occurrence of haystack in *buffer and replaces with the first len characters of needle.
 * When haystack, needle of *buffer are NULL or empty strings, then nothing will be done
 *
 * he functions returns a pointer to *buffer.
 *
 **/

char* replacernlaststring(char** buffer, const char* haystack, const char* needle, size_t len);

/**
 * searches for the last occurrence of haystack in *buffer and replaces with the last len characters of needle.
 * When haystack, needle of *buffer are NULL or empty strings, then nothing will be done
 *
 * he functions returns a pointer to *buffer.
 *
 **/

/* here some couple of functions that may be useful when reading config files and so on */

int emptystring(const char* src);
/**
 * This function checks whether src an empty string is or not. In the first case it returns 1, 0 otherwise
 *
 * An empty line is a line that matches to "" or any string that only has 
 * blank characters (they are prooved by isspace(3))
 *
 **/


#endif
