#ifndef _TIME_LOOP_LIB_H_
#define _TIME_LOOP_LIB_H_
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct{
    int tickCount;
    
    struct timeval start;
    struct timeval previous;
    struct timeval current;
    struct timeval offset;
    struct timeval total;
    float interval;
    int interrupt;
} timeloop_data_t;

timeloop_data_t *
timeloop_new();

void
timeloop_start(timeloop_data_t * timeloop_data,void * (* tick_func)(void*),void * tick_func_data);

void
timeloop_print_time(timeloop_data_t * data);

long int
timeloop_time_to_sleep(timeloop_data_t * data);

int 
timeloop_compare(struct timeval *result, struct timeval *t1, struct timeval *t2);

#endif