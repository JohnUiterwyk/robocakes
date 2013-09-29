#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "util.h"
#include "sim_lib.h"
#include "timeloop_lib.h"
#include "udp_lib.h"

#ifndef __APPLE__
#include "display.h"
#endif

typedef struct {
    timeloop_data_t * time_data;
    udp_conn_data_t * conn_data;
    sim_data_t * sim_data;
    
    char * recv_buffer;
    char * thread_buffer;
    char * draw_message;
    int thread_buffer_updated;
    
} client_data_t;

void client_start(void);
client_data_t * client_data_init();
void * listen_thread_func(void * data);
void * draw_thread_func(void * data);
void * client_timer_tick(void *);

#endif
