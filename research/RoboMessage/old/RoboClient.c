//
//  RoboClient
//  RoboMessage
//
//  UDP Listener
//
//  Based on the Beej's Guide to Network Programming listener.c
//  http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//
//  Created by John Uiterwyk on 9/10/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

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

void *getIpAsString(struct sockaddr *socketAddr)
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
int main(int argc, char *argv[])
{
    int socketFileDesc;
    struct addrinfo hints, *serverInfo, *aiPtr;
    int getInfoResult, bindResult;
    long numBytes;
    struct sockaddr_storage senderAddr;
    char message[MAX_BUFFER_LEN];
    socklen_t addressLength;
    char ipString[INET6_ADDRSTRLEN];
    
    struct timespec sleepTime, sleepTimeResult;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = .001 *1000000000;
    
    //set to zero
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; //use my ip
    
    //get address info passing in some helpful hints
    //null as the first value with AI_PASSIVE means use my ip
    //stores result in serverInfo
    getInfoResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &serverInfo);
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
        if(socketFileDesc == -1)
        {
            //error getting socket
            perror("RoboClient: socket error");
        }else
        {
            //if no error, bind
            bindResult = bind(socketFileDesc,aiPtr->ai_addr,aiPtr->ai_addrlen);
            if(bindResult == -1)
            {
                close(socketFileDesc);
                perror("RoboClient: bind error.");
            }
        }
        
        if(socketFileDesc != -1 && bindResult != -1)
        {
            //we are done, so jump out of for loop!
            break;
        }
    }
    
    if(aiPtr == NULL)
    {
        //we didnt manage to bind to anything
		fprintf(stderr, "RoboClient: failed to bind socket\n");
        exit(EXIT_FAILURE);
    }
    
    //dump servinfo since we are done using it
    freeaddrinfo(serverInfo);
    
    printf("Starting listen loop. Waiting to 'recvfrom'...\n");
    
    //here we start a listen loop
    addressLength = sizeof(senderAddr);
    while(strcmp(message,"exit") != 0)
    {
        
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
        
        //get the senders ip as a string
        inet_ntop(senderAddr.ss_family,
                  getIpAsString((struct sockaddr *)&senderAddr),
                  ipString,
                  sizeof(ipString));
        //WOOT! print the message!
        printf("Got %d bytes from %s: %s\n",(int)numBytes,ipString,message);
        nanosleep(&sleepTime,&sleepTimeResult);
    }
    close(socketFileDesc);
    
    //YAY!
    return EXIT_SUCCESS;
}

