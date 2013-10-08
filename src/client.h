#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "util.h"
#include "sim_lib.h"
#include "timeloop_lib.h"
#include "tcp_client_lib.h"

#ifndef __APPLE__
#ifdef HAVE_GLES
#include "display.h"
#endif
#endif

typedef struct {
    timeloop_data_t * time_data;
    tcp_client_data_t * tcp_client;
    sim_data_t * sim_data;
    char * draw_message;
#ifndef __APPLE__
#ifdef HAVE_GLES
    display_state_t * display_state;
#endif
#endif

} client_data_t;

void client_start(struct configuration * conf);
client_data_t * client_data_init();
void * draw_thread_func(void * data);
void * client_timer_tick(void *);

#endif
