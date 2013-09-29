//
//  main.c
//  SocketTest
//
//  Created by John Uiterwyk on 8/18/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//
#include "TimeLoopLib.h"

TimeLoop_Data *
TimeLoop_Init()
{
    TimeLoop_Data *data;
    data = calloc(1, sizeof(TimeLoop_Data));
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
TimeLoop_Start(TimeLoop_Data * timeLoopData,void (* tickFunc)(void*),void * tickFuncData)
{
    do {
        /*get current time */
        gettimeofday(&timeLoopData->current, NULL);
        
        
        TimeLoop_Compare(&timeLoopData->offset, &timeLoopData->current, &timeLoopData->previous);
        TimeLoop_Compare(&timeLoopData->total, &timeLoopData->current, &timeLoopData->start);
        
        tickFunc(tickFuncData);
        
        timeLoopData->tickCount++;
        timeLoopData->previous = timeLoopData->current;
        TimeLoop_PrintTime(timeLoopData);
        
        usleep(timeLoopData->interval * 1000000);
        
    } while (timeLoopData->interrupt != 1);
}

void
TimeLoop_PrintTime(TimeLoop_Data * data)
{
    printf("tick:%d offset: %ld.%06d, total: %ld.%06d \n",
           data->tickCount,
           data->offset.tv_sec,
           data->offset.tv_usec,
           data->total.tv_sec,
           data->total.tv_usec);
}
long int TimeLoop_TimeToSleep(TimeLoop_Data * data)
{
    long int result = data->interval * 2 * 1000000
    - (data->offset.tv_usec + 1000000 * data->offset.tv_sec);
    return result;
    
    
}
int TimeLoop_Compare(timev *result, timev *t1, timev *t2)
{
    long int diff = (t1->tv_usec + 1000000 * t1->tv_sec)
    - (t2->tv_usec + 1000000 * t2->tv_sec);
    
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    
    if(diff>0)return 1;
    if(diff<0)return -1;
    if(diff==0)return 0;
}
