/**
 * @file client.c
 */

/*
 * Copyright (C) 2013 John Uiterwyk, Virginia King, Stewart Johnston
 *
 * Display functions are only enabled on supported hardware with OpenGL
 * support.
 */
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

#ifdef HAVE_GLES
#include "display.h"
#endif

/**
 * The current state of a running client.
 */
typedef struct {
  /** Timing information */
  timeloop_data_t * time_data;
  /** TCP connection */
  tcp_client_data_t * tcp_client;
  /** Current simulation state */
  sim_data_t * sim_data;
  /** The message to draw next */
  char * draw_message;
#ifdef HAVE_GLES
  /** The OpenVG display state */
  display_state_t * display_state;
#endif

} client_data_t;

/**
 * Entry point to initialise the client.
 *
 * This function creates the required datastructures and connections
 * (display hardware and network) to run Hydra.
 *
 * @param conf A configuration structure, populated by main() from
 * configuration files or command line arguments.
 * */
void client_start(struct configuration * conf);

/**
 * Initializes client data struct to defaults.
 *
 * A client_data_t struct is the object that stores the top-level state
 * of a running client. It holds a reference to the tcp listener, tracks
 * time passing and stores the simulation data.
 *
 */
client_data_t * client_data_init();

/**
 * Draw callback to use in display thread.
 *
 * @param data The client data state struct.
 */
void * draw_thread_func(void * data);

/**
 * Tick callback for client.
 *
 * Note that two buffers are used for simdata - one is read from while
 * the other is written to, and then if an update is required the
 * pointers are switched.
 * This ensures that any read or write to shared memory is kept to an
 * absolute minimum number of operations - it's a simple pointer
 * assignment.
 */
void * client_timer_tick(void *);

#endif
