/**
 * @file thread_lib.h
 */
#ifndef _THREAD_LIB_
#define _THREAD_LIB_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_BUFFER_LEN 1000

typedef struct
{
  char * buffer;
  int is_changed;
  pthread_mutex_t mutex;

}shared_buffer_t;


shared_buffer_t * thread_new_shared_buffer();
void thread_free_shared_buffer(shared_buffer_t * shared);
void thread_copy_to_buffer(shared_buffer_t * shared, const char * source);
int thread_copy_from_buffer(shared_buffer_t * buffer, char * destination);

#endif
