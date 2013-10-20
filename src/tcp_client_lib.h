/**
 * @file tcp_client_lib.h
 */
#ifndef _TCP_CLIENT_LIB_
#define _TCP_CLIENT_LIB_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#include "thread_lib.h"
#include "tcp_lib.h"


/**
 * TCP client data struct
 */
typedef struct
{
  /** IP address for server. */
  char * dest_ip_address;
  /** Server port. */
  char * port;
  /** Network socket */
  int socket_fd;
  /** Socket type */
  int socket_type;
  /** Buffer for messages to be sent to server. */
  shared_buffer_t * send_buffer;
  /** Buffer for incoming messages from server. */
  shared_buffer_t * recv_buffer;
  /** Control variable to stop connection. */
  int alive;

  /** Thread for sending messages to server. */
  pthread_t send_thread;
  /** Thread for receiving messages from server. */
  pthread_t recv_thread;

} tcp_client_data_t;


/**
 * Creates a new txp_client_data_t and initializes to safe defaults.
 */
tcp_client_data_t * tcp_client_data_new();
/**
 * Frees tcp_client_data_t and it's contents.
 */
void tcp_client_data_free(tcp_client_data_t * data);
/**
 * Starts TCP connection.
 */
void tcp_client_start(tcp_client_data_t * client);
/**
 * Stops TCP connection.
 */
void tcp_client_stop(tcp_client_data_t * client);
/** Connects and binds to a server.
 */
void tcp_client_connect(tcp_client_data_t * client);
/**
 * Loop for sending information to the server.
 */
void * tcp_client_send_loop(void * data);
/**
 * Loop for receiving information from the server.
 */
void * tcp_client_recv_loop(void * data);


#endif
