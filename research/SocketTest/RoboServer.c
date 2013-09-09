/*
 ** broadcaster.c -- a datagram "client" like talker.c, except
 **                  this one can broadcast
 */

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

#include "InputUtil.c"

#define DEFAULT_PORT "8675"	// the port users will be connecting to
#define MAX_MESSAGE_LEN 100

int main(int argc, char *argv[])
{
    int socketFileDesc;
    struct addrinfo hints, *serverInfo, *aiPtr;
    int getInfoResult;
    int numBytes;
    int broadcast;
    char message[MAX_MESSAGE_LEN];
    
    if (argc != 2) {
        fprintf(stderr,"usage: talker hostname\n");
        exit(1);
    }
    
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    getInfoResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &serverInfo);
    if(getInfoResult != 0)
    {
        fprintf(stderr, "RoboServer: getaddrinfo: %s\n", gai_strerror(getInfoResult));
        exit(EXIT_FAILURE);
    }
    
    // loop through all the results and make a socket
    for(aiPtr = serverInfo; aiPtr != NULL ; aiPtr = aiPtr->ai_next)
    {
        
        //try to get the socket fd
        socketFileDesc = socket(aiPtr->ai_family,
                                aiPtr->ai_socktype,
                                aiPtr->ai_protocol);
        if(socketFileDesc == -1)
        {
            //error getting socket
            perror("RoboServer: socket error");
        }else
        {
            break;
        }
        
    }
    
    if(aiPtr == NULL)
    {
        //we didnt manage to bind to anything
		fprintf(stderr, "RoboServer: failed to bind socket\n");
        exit(EXIT_FAILURE);
    }
    
    freeaddrinfo(serverInfo);
    // this call is what allows broadcast packets to be sent:
    broadcast = 1;
	if (setsockopt(socketFileDesc, SOL_SOCKET, SO_BROADCAST, &broadcast,
                   sizeof broadcast) == -1) {
		perror("setsockopt (SO_BROADCAST)");
		exit(1);
	}
    
    while(strcmp(message,"exit") != 0)
    {
        
        getString(message,MAX_MESSAGE_LEN,":::");
        numBytes = sendto(socketFileDesc,
                          message,
                          strlen(message),
                          0,
                          aiPtr->ai_addr,
                          aiPtr->ai_addrlen);
        if(numBytes == -1)
        {
            perror("RoboServer: sendto problem");
            exit(EXIT_FAILURE);
        }
        printf("RoboServer: sent %d bytes to %s\n", numBytes, argv[1]);
    }
    close(socketFileDesc);
    return 0;
}