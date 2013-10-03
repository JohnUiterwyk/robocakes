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

int tcp_send_full_message(int socketfd, char *buffer, int *length);
void * tcp_get_in_addr(struct sockaddr *sa);
void tcp_print_addr_info(struct addrinfo * addrInfo);


#endif