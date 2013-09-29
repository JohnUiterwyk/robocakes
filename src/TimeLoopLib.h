#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct timeval timev;
typedef struct{
    int tickCount;
    
    timev start;
    timev previous;
    timev current;
    timev offset;
    timev total;
    float interval;
    int interrupt;
}TimeLoop_Data;

TimeLoop_Data *
TimeLoop_Init();

void
TimeLoop_Start(TimeLoop_Data * timeLoopData,void * (* tickFunc)(void*),void * tickFuncData);

void
TimeLoop_PrintTime(TimeLoop_Data * data);

long int
TimeLoop_TimeToSleep(TimeLoop_Data * data);

int 
TimeLoop_Compare(timev *result, timev *t1, timev *t2);
