/**
 * @file util.h
 */
#ifndef _UTIL_H_
#define _UTIL_H_
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "config.h"

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

#define USER_CONFIG_FILE_LOCATION ".hydra.conf"

#define SERVER_ROLE "server"
#define CLIENT_ROLE "client"

#define DEFAULT_NET_METHOD "TCP"
/* #define DEFAULT_NET_METHOD "UDP" */
#define DEFAULT_SERVER_IP "localhost"
#define DEFAULT_SERVER_PORT "8000"
#define DEFAULT_MAP_WIDTH 1900
#define DEFAULT_MAP_HEIGHT 1200


struct configuration {
  bool frames;
  uint32_t position;
  uint32_t clients;
  uint32_t balls;
  uint32_t interval;
  char * role;
  char * ipaddress;
  char * port;
  uint32_t width;
  uint32_t height;
};

struct configuration *
new_config(void);

void
init_config(struct configuration * conf);

/* Prints program usage to the screen
*/
void print_usage();

/* Checks whether a string array contains the specified string. */
bool
string_array_contains(const char *const* haystack, const char *needle);

char *
strip_whitespace(char * string);

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
