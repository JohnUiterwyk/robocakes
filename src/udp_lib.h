//
//  UdpSendMessage.h
//  RoboMessage
//
//  Created by John Uiterwyk on 9/15/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#ifndef _UDP_LIB_H_
#define _UDP_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>


#define DEFAULT_PORT "8675"
#define MAX_BUFFER_LEN 600
#define SOCKET_TYPE_LISTEN 1001
#define SOCKET_TYPE_BROADCAST 1002

typedef struct
{
    const char * dest_ip_address;
    const char * port;
    int socket_fd;
    struct addrinfo * socket_addr_info;
    struct addrinfo * server_addr_info;
    int socket_type;

} udp_conn_data_t;

udp_conn_data_t * udp_new_conn_data();
void udp_create_socket(udp_conn_data_t * conn_data);
void udp_send_message(udp_conn_data_t * conn_data, char * message);
void udp_receive_message(udp_conn_data_t * conn_data, char * message);
void * udp_get_ip_as_string(struct sockaddr *socket_address);
void udp_print_address_info(struct addrinfo * address_info);
#endif
