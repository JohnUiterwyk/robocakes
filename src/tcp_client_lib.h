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

#include "thread_lib.h"
#include "tcp_lib.h"


typedef struct
{
    char * dest_ip_address;
    char * port;
    int socket_fd;
    int socket_type;
    shared_buffer_t * send_buffer;
    shared_buffer_t * recv_buffer;
    int alive;
    
    pthread_t send_thread;
    pthread_t recv_thread;
    
} tcp_client_data_t;


tcp_client_data_t * tcp_client_data_new();
void tcp_client_data_free(tcp_client_data_t * data);
void tcp_client_start(tcp_client_data_t * client);
void tcp_client_stop(tcp_client_data_t * client);
void tcp_client_connect(tcp_client_data_t * client);
void * tcp_client_send_loop(void * data);
void * tcp_client_recv_loop(void * data);


#endif