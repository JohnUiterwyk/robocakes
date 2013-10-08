#ifndef _SERVER_H_
#define _SERVER_H_
#include <stdio.h>
#include "tcp_server_lib.h"
#include "sim_lib.h"
#include "util.h"
#include "timeloop_lib.h"


typedef struct {
    timeloop_data_t * time_data;
    tcp_server_data_t * tcp_server;
    sim_data_t * sim_data;
    char message[MAX_BUFFER_LEN];
    
} server_data_t;

void server_start(struct configuration * conf);
server_data_t* server_int();
void * server_timer_tick(void *);
#endif
