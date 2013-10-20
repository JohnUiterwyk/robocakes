/**
 * @file udl_lib.h
 */
/*  Created by John Uiterwyk on 9/15/13.
 *  Copyright (c) 2013 John Uiterwyk. All rights reserved.
 */

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

/**
 * Connection information for the udp connection
 */
typedef struct
{
  /** IP address to broadcast to or listen on. */
  const char * dest_ip_address;
  /** Port number to broadcast to or listen on. */
  const char * port;
  /** Socket file */
  int socket_fd;
  /** Addrinfo for socket */
  struct addrinfo * socket_addr_info;
  /** Addrinfo for server */
  struct addrinfo * server_addr_info;
  /** Socket type - listen or broadcast */
  int socket_type;

} udp_conn_data_t;

/**
 * Creates a new UDP connection data struct.
 */
udp_conn_data_t * udp_new_conn_data();
/**
 * Creates a new socket to listen on or broadcast to.
 */
void udp_create_socket(udp_conn_data_t * conn_data);
/**
 * Sends message over the network
 */
void udp_send_message(udp_conn_data_t * conn_data, char * message);
/**
 * Listen on socket and copy received message into message pointer.
 */
void udp_receive_message(udp_conn_data_t * conn_data, char * message);
/**
 * Returns the IPv4 or IPv6 address as a string, as appropriate.
 */
void * udp_get_ip_as_string(struct sockaddr *socket_address);
/**
 * Helper function - prints address information to stdout.
 */
void udp_print_address_info(struct addrinfo * address_info);
#endif
