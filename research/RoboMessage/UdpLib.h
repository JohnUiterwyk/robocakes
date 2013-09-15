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
#define MAX_BUFFER_LEN 100
#define SOCKET_TYPE_LISTEN 1001
#define SOCKET_TYPE_BROADCAST 1002

typedef struct
{
    char ipAddress[INET6_ADDRSTRLEN];
    char port[5];
    int socketFileDesc;
    int socketAddressInfo;
    int socketType;
    
} UDP_ConnectionData;

void UDP_CreateListenSocket(int socketFileDesc, char * address, int port, int bind);
void UDP_EnableBroadcast(int socketFileDesc);
void UDP_SendMessage();
void UDP_RecieveMessage(int socketFileDesc, char * message);
void * getIpAsString(struct sockaddr *socketAddr);
#endif
