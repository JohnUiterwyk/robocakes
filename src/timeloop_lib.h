/**
 * @file timeloop_lib.h
 */
#ifndef _TIME_LOOP_LIB_H_
#define _TIME_LOOP_LIB_H_
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * Stores the passage of time between ticks.
 */
typedef struct{
  /** Current tick count */
  int tickCount;

  /** Timestamp for struct creation */
  struct timeval start;
  /** Timestamp for previous tick  */
  struct timeval previous;
  /** Timestamp for current tick */
  struct timeval current;
  /** Minimum time allowable between ticks */
  struct timeval offset;

  /** Total elapsed time since start. */
  struct timeval total;
  /** usleep factor */
  float interval;
  /** Interrupt to stop time loop */
  int interrupt;
} timeloop_data_t;

/**
 * Creates a timeloop data struct with default values.
 */
timeloop_data_t *
timeloop_new();

/**
 * Loop to perform tick_func per tick until interrupt received.
 */
void
timeloop_start(timeloop_data_t * timeloop_data,void * (* tick_func)(void*),void * tick_func_data);

/**
 * Debug function - prints timeloop data to stdout.
 */
void
timeloop_print_time(timeloop_data_t * data);

long int
timeloop_time_to_sleep(timeloop_data_t * data);

/**
 * Controller method to determine if enough time has elapsed to perform
 * another tick.
 */
int
timeloop_compare(struct timeval *result, struct timeval *t1, struct timeval *t2);

#endif
