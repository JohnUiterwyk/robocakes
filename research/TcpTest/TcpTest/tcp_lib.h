#ifndef _TCP_LIB_
#define _TCP_LIB_

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

#include "thread_lib.h"

#define PORT "3490" // the port client will be connecting to
#define MAX_CLIENTS 10
#define TCP_SERVER 101
#define TCP_CLIENT 102


typedef struct
{
    int socket_fd;
    pthread_t recv_thread;
    
}tcp_client_data_t;

typedef struct
{
    char * dest_ip_address;
    char * port;
    int socket_fd;
    struct addrinfo * socket_addr_info;
    struct addrinfo * server_addr_info;
    int socket_type;
    
    fd_set clients;
    int max_client_fd;
    
    
    shared_buffer_t * send_buffer;
    shared_buffer_t * recv_buffer;
    
} tcp_conn_data_t;


// get sockaddr, IPv4 or IPv6:
tcp_conn_data_t * tcp_new_conn_data();
void tcp_create_server(tcp_conn_data_t * conn_data);
void tcp_start_server(tcp_conn_data_t * conn_data);
int tcp_accept_connection(int listener);
void tcp_create_client(tcp_conn_data_t * conn_data);
void tcp_start_client_listener(tcp_conn_data_t * conn_data);
void tcp_start_client_sender(tcp_conn_data_t * conn_data);
int tcp_send_full_message(int socketfd, char *buffer, int *length);
void * tcp_get_in_addr(struct sockaddr *sa);
void tcp_print_addr_info(struct addrinfo * addrInfo);


#endif