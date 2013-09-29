//
//  main.c
//  SocketTest
//
//  Created by John Uiterwyk on 8/18/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//
#include "timeloop_lib.h"

timeloop_data_t *
timeloop_new()
{
    timeloop_data_t *data;
    data = calloc(1, sizeof(timeloop_data_t));
    if(data == NULL)
    {
        perror("Calloc error in TimeLoop_Init");
        exit(EXIT_FAILURE);
    }
    data->tickCount = 0;
    data->interval = 1;
    data->interrupt = 0;
    gettimeofday(&(data->start), NULL);
    gettimeofday(&(data->current), NULL);
    gettimeofday(&(data->previous), NULL);
    return data;
}

void
timeloop_start(timeloop_data_t * timeloop_data,void * (* tick_func)(void*),void * tick_func_data)
{
    do {
        /*get current time */
        gettimeofday(&timeloop_data->current, NULL);
        
        
        timeloop_compare(&timeloop_data->offset, &timeloop_data->current, &timeloop_data->previous);
        timeloop_compare(&timeloop_data->total, &timeloop_data->current, &timeloop_data->start);
        
        tick_func(tick_func_data);
        
        timeloop_data->tickCount++;
        timeloop_data->previous = timeloop_data->current;
        /* TimeLoop_PrintTime(timeLoopData); */
        
        usleep(timeloop_data->interval * 1000000);
        
    } while (timeloop_data->interrupt != 1);
}

void
timeloop_print_time(timeloop_data_t * data)
{
    printf("tick:%d offset: %ld.%06d, total: %ld.%06d \n",
           data->tickCount,
           data->offset.tv_sec,
           data->offset.tv_usec,
           data->total.tv_sec,
           data->total.tv_usec);
}
long int timeloop_time_to_sleep(timeloop_data_t * data)
{
    long int result = data->interval * 2 * 1000000
    - (data->offset.tv_usec + 1000000 * data->offset.tv_sec);
    return result;
    
    
}
int timeloop_compare(struct timeval *result, struct timeval *t1, struct timeval *t2)
{
    long int diff = (t1->tv_usec + 1000000 * t1->tv_sec)
    - (t2->tv_usec + 1000000 * t2->tv_sec);
    
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    
    if(diff>0)
    {
        return 1;   
    }
    else if(diff<0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
