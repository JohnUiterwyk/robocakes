/**
 * @file tcp_server_lib.h
 */
#ifndef _TCP_SERVER_LIB_
#define _TCP_SERVER_LIB_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#include "thread_lib.h"
#include "tcp_lib.h"

#define MAX_CLIENTS 10

/**
 * Struct to store tcp communication for server.*/
typedef struct
{
  /** Port to send messages on */
  char * port;
  /** Socket to send messages to */
  int socket_fd;

  /** Set of client fds to communicate to */
  fd_set clients;
  /** Maximum allowable clients */
  int max_client_fd;

  /** Shared buffer for outgoing messages. */
  shared_buffer_t * send_buffer;
  /** Shared buffer for incoming messages. */
  shared_buffer_t * recv_buffer;
  /** Control variable - set to zero to stop server. */
  int alive;

  /** Thread for accepting new clients. */
  pthread_t accept_thread;
  /** Thread for receiving new messages. */
  pthread_t recv_thread;
  /** Thread for sending messages. */
  pthread_t send_thread;


} tcp_server_data_t;


/**
 * Creates a new tcp_server_data_t struct and initializes values to safe
 * defaults.
 */
tcp_server_data_t * tcp_server_data_new();
/**
 * Frees tcp_server_data_t and it's contents.
 */
void tcp_server_data_free(tcp_server_data_t * server);
/**
 * Starts the TCP server.
 */
void tcp_server_start(tcp_server_data_t * server);
/**
 * Stops the TCP server.
 */
void tcp_server_stop(tcp_server_data_t * server);
/**
 * Creates a new socket to bind the TCP connection to.
 */
void tcp_server_create_socket(tcp_server_data_t * server);
/**
 * Listen for connecting clients function.
 */
void * tcp_server_accept_loop(void * data);
/**
 * Adds a new client to the service.
 */
int tcp_server_accept_connection(int listener);
/**
 * Listen for incoming messages.
 */
void * tcp_server_recv_loop(void * data);
/**
 * Send outgoing messages.
 */
void * tcp_server_send_loop(void * data);


#endif
