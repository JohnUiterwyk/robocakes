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

/**
 * Struct for managing a shared memory buffer.
 */
typedef struct
{
  /** Buffer of shared memory */
  char * buffer;
  /** Update flag for buffer. */
  int is_changed;
  /** Mutex lock for buffer. */
  pthread_mutex_t mutex;

}shared_buffer_t;


/**
 * Creates a new shared buffer.
 */
shared_buffer_t * thread_new_shared_buffer();
/**
 * Frees shared buffer and it's contents.
 */
void thread_free_shared_buffer(shared_buffer_t * shared);
/**
 * Copies data to the shared buffer, waiting until the mutex is
 * released.
 */
void thread_copy_to_buffer(shared_buffer_t * shared, const char * source);
/**
 * Copies data from the shared buffer, waiting until the mutex is
 * released.
 */
int thread_copy_from_buffer(shared_buffer_t * buffer, char * destination);

#endif
