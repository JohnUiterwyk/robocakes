//
//  main.c
//  SocketTest
//
//  Created by John Uiterwyk on 8/18/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct timeval timev;
typedef struct{
    int tick_count;
    timev time_start;
    timev time_prev;
    timev time_offset;
    timev time_total;
}GameData;


/* Return 1 if the difference is negative, otherwise 0.  */
int calc_time_diff(timev *result, timev *t2, timev *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    
    return (diff<0);
}

void print_time(timev *tv)
{
    char buffer[30];
    time_t time_to_print;
    time_to_print = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&time_to_print));
    printf("%s.%06d\n", buffer, tv->tv_usec);
}

int min(int a, int b)
{
    if(a <= b) return a;
    return b;
}
int max(int a, int b)
{
    if(a >= b) return a;
    return b;
}


int game_tick(GameData * data)
{
    
    timev time_current;
    int i;
    gettimeofday(&time_current, NULL);
    calc_time_diff(&data->time_offset, &time_current, &data->time_prev);
    calc_time_diff(&data->time_total, &time_current, &data->time_start);
    data->time_prev = time_current;
    
    if(data->tick_count%1 ==0)
    {
        printf("tick:%d offset: %ld.%06d, total: %ld.%06d \n",
               data->tick_count,
               data->time_offset.tv_sec,
               data->time_offset.tv_usec,
               data->time_total.tv_sec,
               data->time_total.tv_usec);
    }
    
    /*
     * Do the physics or math here.
     * Then dispatch a udp broadcast of current state
     * clients will only listen to start
     */
    for(i =0;i<10000000;i++)
    {
        sqrt(10*10+30*30+2);
    }
    data->tick_count++;
    return 1;
}

int (*tick_func)(GameData *);

int time_loop(int (*tick_func)(GameData *), GameData * data, int fps)
{
    
    do {
        game_tick(data);
        //usleep(1000000/fps);
    } while (data->time_total.tv_sec < 1);
    return 1;
}


int main()
{
    GameData data;
    data.tick_count = 0;
    gettimeofday(&data.time_start, NULL);
    gettimeofday(&data.time_prev, NULL);
    
    time_loop(&game_tick, &data, 60);

    return 0;
}
