//
//  UdpSendMessage.c
//  RoboMessage
//
//  Created by John Uiterwyk on 9/15/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include "UdpLib.h"

void UDP_CreateSocket(UDP_ConnectionData * connData)
{
    struct addrinfo hints, *serverInfo, *aiPtr;
    int getInfoResult, bindResult;
    
    //set to zero
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
    //get address info passing in some helpful hints
    //null as the first value with AI_PASSIVE means use my ip
    //stores result in serverInfo
    if(connData->socketType == SOCKET_TYPE_LISTEN)
    {
        hints.ai_flags = AI_PASSIVE; //use my ip
        getInfoResult = getaddrinfo(NULL, connData->port, &hints, &serverInfo);
    }else
    {
        getInfoResult = getaddrinfo(connData->ipAddress, connData->port, &hints, &serverInfo);
    }
    

    if(getInfoResult != 0)
    {
        fprintf(stderr, "RoboClient Error: getaddrinfo: %s\n", gai_strerror(getInfoResult));
        exit(EXIT_FAILURE);
    }
    //now we look through results and bind
    for(aiPtr = serverInfo; aiPtr != NULL ; aiPtr = aiPtr->ai_next)
    {
        bindResult = -1;
        //try to get the socket fd
        socketFileDesc = socket(aiPtr->ai_family,
                                aiPtr->ai_socktype,
                                aiPtr->ai_protocol);
        if(socketFileDesc != -1)
        {
            //error getting socket
            perror("RoboClient: socket error");
            continue; //try next address info
        }
        
        if(socketType == SOCKET_TYPE_LISTEN)
        {
            //if no error, bind
            bindResult = bind(socketFileDesc,aiPtr->ai_addr,aiPtr->ai_addrlen);
            if(bindResult == -1)
            {
                close(socketFileDesc);
                perror("RoboClient: bind error.");
                continue; //try next address info
            }
        }
        //if we reach here, we have valid socket (and bind result if listening)
        //so break the for loop
        break;
    }
    
    if(aiPtr == NULL)
    {
        //we didnt manage to find a socker to use to anything
		fprintf(stderr, "RoboClient: failed to bind socket\n");
        exit(EXIT_FAILURE);
    }
    
    //dump servinfo since we are done using it
    freeaddrinfo(serverInfo);

}
void UDP_EnableBroadcast(int socketFileDesc)
{
    int broadcast;
    broadcast =1;
	if (setsockopt(socketFileDesc, SOL_SOCKET, SO_BROADCAST, &broadcast,
                   sizeof broadcast) == -1) {
		perror("setsockopt (SO_BROADCAST)");
		exit(1);
	}
}
void UDP_SendMessage(int socketFileDesc, char * message)
{
    long numBytes;
    numBytes = sendto(socketFileDesc,
                      message,
                      strlen(message),
                      0,
                      aiPtr->ai_addr,
                      aiPtr->ai_addrlen);
}
void UDP_RecieveMessage(int socketFileDesc, char * message)
{
    
    struct sockaddr_storage senderAddr;
    socklen_t addressLength;
    long numBytes;
    
    addressLength = sizeof(senderAddr);
    numBytes = recvfrom(socketFileDesc,
                        message,
                        MAX_BUFFER_LEN-1,
                        0,
                        (struct sockaddr *)&senderAddr,
                        &addressLength);
    if(numBytes == -1)
    {
        perror("RoboClient: recvfrom problem");
        exit(EXIT_FAILURE);
    }
    
    //put the null terminator in the right place
    message[numBytes] = '\0';

}

void * getIpAsString(struct sockaddr *socketAddr)
{
    struct sockaddr_in * socketAddrIPv4;
    struct sockaddr_in6 * socketAddrIPv6;
    void * result = NULL;
    //this gets the ip address cast to the IPv4 or IPv6 structure
    if(socketAddr->sa_family == AF_INET)
    {
        //its IPv4
        socketAddrIPv4 = (struct sockaddr_in*)socketAddr;
        result = &(socketAddrIPv4->sin_addr);
        
    }else
    {
        socketAddrIPv6 = (struct sockaddr_in6*)socketAddr;
        result = &(socketAddrIPv6->sin6_addr);
    }
    return result;
}