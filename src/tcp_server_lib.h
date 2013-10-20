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

typedef struct
{
  char * port;
  int socket_fd;

  fd_set clients;
  int max_client_fd;

  shared_buffer_t * send_buffer;
  shared_buffer_t * recv_buffer;
  int alive;

  pthread_t accept_thread;
  pthread_t recv_thread;
  pthread_t send_thread;


} tcp_server_data_t;


tcp_server_data_t * tcp_server_data_new();
void tcp_server_data_free(tcp_server_data_t * server);
void tcp_server_start(tcp_server_data_t * server);
void tcp_server_stop(tcp_server_data_t * server);
void tcp_server_create_socket(tcp_server_data_t * server);
void * tcp_server_accept_loop(void * data);
int tcp_server_accept_connection(int listener);
void * tcp_server_recv_loop(void * data);
void * tcp_server_send_loop(void * data);


#endif
