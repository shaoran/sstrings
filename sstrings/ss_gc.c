/**
 * Filename: sstrings/ss_gc.c
 * Author: Pablo Yanez Trujillo <yanezp@informatik.uni-freiburg.de>
 *
 * This is free software. Please read the file ../COPYING if you
 * want to use/edit/distribuite this source file.
 * This source file is protected by the GNU GPL-2
 *
 * NOTE: There is NO  warranty; not even for MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.
**/

#include <ss_gc.h>
#include <string.h> /* memmove */

static int __ss_gc_flag = 0;
static void **__ss_gc_pointers = NULL; /* the first position tells how many
				  __ss_gc_pointers are saved in the list */


static int add_to_list(const void *ptr);
static int remove_from_list(const void *ptr);
static void gc_cleanup(void);


int add_to_list(const void *ptr)
{
	void **tmp;
	size_t count = 0;
	if(!ptr)
		return 1;

	if(__ss_gc_pointers)
		count = (int) __ss_gc_pointers[0];

	count += 2;

	tmp = realloc(__ss_gc_pointers, sizeof(void*) * count);

	if(!tmp)
		return 0;

	if(tmp != __ss_gc_pointers)
		__ss_gc_pointers = tmp;

	__ss_gc_pointers[count - 1] = (void*) ptr;
	__ss_gc_pointers[0] = (void*) (count - 1);

	return 1;
}


int remove_from_list(const void *ptr)
{
	int found = 0;
	size_t i = 1, count;

	if(!ptr)
		return 1;

	if(!__ss_gc_pointers)
		return 0;

	while( (i <= (int) __ss_gc_pointers[0]) )
	{
		if(__ss_gc_pointers[i] == ptr)
		{
			found = 1;
			break;
		}

		i++;
	}

	if(!found)
		return 0;

	count = (int) __ss_gc_pointers[0];

	/* if i is the last, do not memmove */

	if(i == (int) __ss_gc_pointers[0])
	{
		__ss_gc_pointers[i] = NULL;
		__ss_gc_pointers[0] = (void*) ( count - 1);
		return 1;
	}

	memmove(&__ss_gc_pointers[i], &__ss_gc_pointers[i + 1], (count - i) * sizeof(void*));

	__ss_gc_pointers[count] = NULL;

	__ss_gc_pointers[0] = (void*) (count - 1);

	return 1;
}

static void gc_cleanup(void)
{
	int i;
	if(!__ss_gc_pointers)
		return;

	for(i=1; i<= (int) __ss_gc_pointers[0]; ++i)
	{
		free(__ss_gc_pointers[i]);
		__ss_gc_pointers[i] = NULL;
	}

	free(__ss_gc_pointers);
}

int ss_use_gc(void)
{
	if(__ss_gc_flag)
		return 1;

	if(atexit(gc_cleanup))
		return 0;

	__ss_gc_flag = 1;

	return 1;
}


void* ss_malloc(size_t size)
{
	void *tmp;

	tmp = malloc(size);

	if(!tmp)
		return NULL;

	if(__ss_gc_flag && !add_to_list(tmp))
	{
		free(tmp);
		return NULL;
	}

	return tmp;
}

void* ss_realloc(void* ptr, size_t size)
{
	int found = 0;
	size_t i = 1;
	void *tmp;

	if(!__ss_gc_flag)
		return realloc(ptr, size);

	if(!ptr)
		return ss_malloc(size);
	
	while( (i <= (int) __ss_gc_pointers[0]) )
        {
                if(__ss_gc_pointers[i] == ptr)
                {
                        found = 1;
                        break;
                }

                i++;
        }

        if(!found)
                return NULL; /* not a pointer created by ss_malloc/ss_realloc */

	tmp = realloc(ptr, size);

	if(!tmp)
		return NULL; /* do not change original pointer */

	if(tmp != __ss_gc_pointers[i])
		__ss_gc_pointers[i] = tmp; /* adress of ptr changed */
	
	return tmp;
}

void ss_free(void* ptr)
{
	if(!ptr)
		return;

	if(__ss_gc_flag && !remove_from_list(ptr))
		return;

	free(ptr);	
}
