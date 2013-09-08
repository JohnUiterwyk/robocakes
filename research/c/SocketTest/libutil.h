/*** Miscelaneous Utility Functions ***/
#ifndef _LIBUTIL_H_
#define _LIBUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif /** cplusplus **/

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>

#define GETOPT_FINISHED -1
#define REALLOC_INCREMENT 256
#define MAX_WORD_LEN 64

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
    
#ifndef SUCCESS
#define SUCCESS 0
#endif
    
#ifndef FAILURE
#define FAILURE 0
#endif

/* Create a new block of memory and do vital error checking. 
 * The return value should be free'd by the calling function.
 */
void *safe_malloc (size_t size);

/* Resize a pre-existing block of memory and do vital error checking. 
 * The return value should be free'd by the calling function.
 */
void *safe_realloc (void *old_mem, size_t new_size);

/* Take a current string and create a new copy. The
 * return value should be free'd by the calling function.
 */
char *safe_strdup (const char *str);

/* Compare two integers. Return 0 if equal, negative if a < b, positive
 * if a > b. 
 */
int cmp_int (const void *a, const void *b);

    
/* This takes a string and returns an integer.
 */
int string_to_int(char * string, int * result);
    
/* Trims the new line character from the end of string
 */
void trimNewLine(char * string);
    
/* Takes a filename and file pointer. The program exits on invalid files.
 */
FILE* safe_open_file(char * filename);
    
/* takes an int value and returns a pointer to a new int
 */
int * alloc_int (int val);
    
void delete_int(void * val);

#ifdef __cplusplus
}
#endif /** cplusplus **/

#endif /** _LIBUTIL_H_ **/

