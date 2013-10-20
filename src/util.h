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


/**
 * Configuration storage
 */
struct configuration {
  /** Enable frames per second output. */
  bool frames;
  /** Integer position (zero indexed, from left) */
  uint32_t position;
  /** Number of clients */
  uint32_t clients;
  /** Number of balls to draw */
  uint32_t balls;
  /** Timer interval in ms */
  uint32_t interval;
  /** Role: Client or Server */
  char * role;
  /** IP address to use for communication.*/
  char * ipaddress;
  /** Port to use for communication. */
  char * port;
  /** Width of game map. */
  uint32_t width;
  /** Height of game map. */
  uint32_t height;
};

/**
 * Creates a new configuration store.
 */
struct configuration *
new_config(void);

/**
 * Initializes communication store to safe defaults.
 */
void
init_config(struct configuration * conf);

/**
 * Prints program usage to the screen
 */
void print_usage();

/** Checks whether a string array contains the specified string. */
bool
string_array_contains(const char *const* haystack, const char *needle);

/** Strips whitespace from a string and moves string in memory to
 * recover room.
 */
char *
strip_whitespace(char * string);

/**
 * Callocs count * size and checks for a safe result.
 */
void *
safe_calloc(int count,int size);

/**
 * Clears buffer on input error.\
 * \copyright Paul Miller
 */
void
read_rest_of_line();

/**
 * Get string input from user and insist that it be non-empty.
 */
int
get_non_empty_string(char* string, unsigned length, char * prompt);

/**
 * Print prompt, then get response from user.
 */
int
get_string_auto_prompt(char* string, unsigned length, char * contentType);

/**
 * Gets string from the stdin buffer.
 */
int
get_string_from_stdin(char* string, unsigned length, char* prompt);

#endif
