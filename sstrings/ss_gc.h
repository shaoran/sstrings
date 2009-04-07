/**
 * Filename: sstrings/ss_gc.h
 * Author: Pablo Yanez Trujillo <yanezp@informatik.uni-freiburg.de>
 *
 * This is free software. Please read the file ../COPYING if you
 * want to use/edit/distribuite this source file.
 * This source file is protected by the GNU GPL-2
 *
 * NOTE: There is NO  warranty; not even for MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.
**/


/*
 * This is a very small implementation of a garbage collector for
 * sstrings */

#ifndef SSTRINGS2_GC_H
#define SSTRINGS2_GC_H

#include <stdlib.h>

/* wrapper of stdlib::malloc  */
void* ss_malloc(size_t size);

/* wrapper of stdlib::realloc */
void* ss_realloc(void* ptr, size_t size);

/* wrapper of stdlib::free */
void ss_free(void* ptr);

/* use this function to run the garbage collector */
int ss_use_gc(void);

#endif
