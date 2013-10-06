#ifndef _UTIL_H_
#define _UTIL_H_
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "conf.h"

#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE -1
#endif

#define TRUE 1
#define FALSE 0

#define DEBUG 1

#define REQUIRED_ARGS 1

#define GETOPT_FINISHED -1

#define MAX_STRING_SIZE 255

/* Prints program usage to the screen
 */
void print_usage();

/* Checks whether a string array contains the specified string. */
bool
string_array_contains(const char *const* haystack, const char *needle);

char *
strip_leading_whitespace(char * string);

void *
safe_calloc(int count,int size);

void
read_rest_of_line();

int
get_non_empty_string(char* string, unsigned length, char * prompt);

int
get_string_auto_prompt(char* string, unsigned length, char * contentType);

int
get_string_from_stdin(char* string, unsigned length, char* prompt);

#endif
