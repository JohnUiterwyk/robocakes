/**
 * @file tcp_lib.h
 */
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

#define TCP_SERVER 101
#define TCP_CLIENT 102

/**
 * Sends the message in buffer over the network, using the socket
 * provided.
 *
 * \param socketfd The file descripter for the socket to use.
 * \param buffer The message to send.
 * \param length The length of the message to send.
 * \return -1 on failure, 0 on success
 */
int tcp_send_full_message(int socketfd, char *buffer, int *length);

/**
 * Returns the ip address associated with the socket.
 *
 * \param sockaddr SockAddr Address information struct
 * \return Incoming address
 */
void * tcp_get_in_addr(struct sockaddr *sa);

/**
 * Debugging function - prints IP address information to stdout
 *
 * \param addrinfo Addrinfo struct for connection (see getaddrinfo (3)
 * man page)
 */
void tcp_print_addr_info(struct addrinfo * addrInfo);


#endif
