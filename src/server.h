#ifndef _SERVER_H_
#define _SERVER_H_
#include <stdio.h>
#include "udp_lib.h"
#include "sim_lib.h"
#include "util.h"
#include "timeloop_lib.h"


typedef struct {
    timeloop_data_t * time_data;
    udp_conn_data_t * conn_data;
    sim_data_t * sim_data;
    char message[MAX_BUFFER_LEN];
    
} server_data_t;

void server_start(void);
void * server_timer_tick(void *);
#endif
