//
//  UdpSendMessage.h
//  RoboMessage
//
//  Created by John Uiterwyk on 9/15/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#ifndef RoboMessage_UdpSendMessage_h
#define RoboMessage_UdpSendMessage_h

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
#define MAX_BUFFER_LEN 10000
#define SOCKET_TYPE_LISTEN 1001
#define SOCKET_TYPE_BROADCAST 1002

typedef struct
{
    const char * ipAddress;
    const char * port;
    int socketFileDesc;
    struct addrinfo * socketAddressInfo;
    int socketType;

} UDP_ConnectionData;

void UDP_CreateSocket(UDP_ConnectionData * connData);
void UDP_SendMessage(UDP_ConnectionData * connData, char * message);
void UDP_ReceiveMessage(UDP_ConnectionData * connData, char * message);
void * getIpAsString(struct sockaddr *socketAddr);
void printAddressInfo(struct addrinfo * addrInfo);
#endif
