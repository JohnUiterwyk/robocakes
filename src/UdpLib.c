//
//  UdpSendMessage.c
//  RoboMessage
//
//  Created by John Uiterwyk on 9/15/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include "UdpLib.h"

UDP_ConnectionData * UDP_NewConnData()
{
    UDP_ConnectionData * result;
    result = calloc(1, sizeof(UDP_ConnectionData));
    if(result == NULL)
    {
        perror("Error ");
    }
    return result;
}

void UDP_CreateSocket(UDP_ConnectionData * connData)
{
    struct addrinfo hints, *aiPtr;
    int getInfoResult, bindResult, broadcastResult;
    int broadcast;

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
        getInfoResult = getaddrinfo(NULL,
                                    connData->port,
                                    &hints,
                                    &(connData->serverInfo));
    }else
    {
        getInfoResult = getaddrinfo(connData->ipAddress,
                                    connData->port,
                                    &hints,
                                    &(connData->serverInfo));
    }


    if(getInfoResult != 0)
    {
        fprintf(stderr,
                "RoboClient Error: getaddrinfo: %s\n",
                gai_strerror(getInfoResult));
        exit(EXIT_FAILURE);
    }
    //now we look through results and bind
    for(aiPtr = connData->serverInfo; aiPtr != NULL ; aiPtr = aiPtr->ai_next)
    {
        printAddressInfo(aiPtr);
        bindResult = -1;
        //try to get the socket fd
        connData->socketFileDesc = socket(aiPtr->ai_family,
                                aiPtr->ai_socktype,
                                aiPtr->ai_protocol);
        if(connData->socketFileDesc == -1)
        {
            //error getting socket
            perror("RoboClient: socket error");
            continue; //try next address info
        }

        if(connData->socketType == SOCKET_TYPE_LISTEN)
        {
            //if no error, bind
            bindResult = bind(connData->socketFileDesc,
                              aiPtr->ai_addr,
                              aiPtr->ai_addrlen);
            if(bindResult == -1)
            {
                close(connData->socketFileDesc);
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
    }else
    {
        connData->socketAddressInfo = aiPtr;
    }

    if (connData->socketType == SOCKET_TYPE_BROADCAST) {
        broadcast =1;
        broadcastResult = setsockopt(connData->socketFileDesc,
                                     SOL_SOCKET,
                                     SO_BROADCAST,
                                     &broadcast,
                                     sizeof broadcast);
        if (broadcastResult == -1) {
            perror("setsockopt (SO_BROADCAST)");
            exit(1);
        }
    }

}
void UDP_SendMessage(UDP_ConnectionData * connData, char * message)
{
    long numBytes;
    numBytes = sendto(connData->socketFileDesc,
                      message,
                      strlen(message),
                      0,
                      connData->socketAddressInfo->ai_addr,
                      connData->socketAddressInfo->ai_addrlen);
    printf("RoboServer: sent %d bytes to %s\n", (int)numBytes, connData->ipAddress);
}
void UDP_ReceiveMessage(UDP_ConnectionData * connData, char * message)
{

    struct sockaddr_storage senderAddr;
    socklen_t addressLength;
    long numBytes;
    char ipString[INET6_ADDRSTRLEN];

    addressLength = sizeof(senderAddr);
    numBytes = recvfrom(connData->socketFileDesc,
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

    //get the senders ip as a string
    inet_ntop(senderAddr.ss_family,
              getIpAsString((struct sockaddr *)&senderAddr),
              ipString,
              sizeof(ipString));

    // print the message!
    printf("Got %d bytes from %s: %s\n",(int)numBytes,ipString,message);

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

void printAddressInfo(struct addrinfo * addrInfo)
{
    char ipstr[INET6_ADDRSTRLEN];
    void *addr;
    char *ipver;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (addrInfo->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)addrInfo->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";
    } else { // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addrInfo->ai_addr;
        addr = &(ipv6->sin6_addr);
        ipver = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(addrInfo->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
}
