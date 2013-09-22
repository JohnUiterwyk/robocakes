//
//  main.c
//  RoboMessage
//
//  Created by John Uiterwyk on 9/15/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include "UdpLib.h"
#include "InputUtil.h"

int main(int argc, char *argv[])
{
    UDP_ConnectionData connData;
    char message[MAX_BUFFER_LEN];
    
    struct timespec sleepTime, sleepTimeResult;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = .001 *1000000000;
    
    if (argc > 4 || argc < 3) {
        fprintf(stderr,"usage: RoboMessage server hostname port \n");
        fprintf(stderr,"or\n");
        fprintf(stderr,"usage: RoboMessage client port \n");
        exit(1);
    }
    if(strcasecmp(argv[1], "server") == 0)
    {
        //server
        connData.socketType = SOCKET_TYPE_BROADCAST;
        connData.ipAddress = argv[2];
        connData.port = argv[3];
        UDP_CreateSocket(&connData);
        while(strcmp(message,"exit") != 0)
        {
            getString(message,MAX_BUFFER_LEN,":::");
            UDP_SendMessage(&connData, message);
            
        }
    }else if(strcasecmp(argv[1], "client") == 0)
    {
        connData.socketType = SOCKET_TYPE_LISTEN;
        connData.port = argv[2];
        UDP_CreateSocket(&connData);
        while(strcmp(message,"exit") != 0)
        {
            UDP_RecieveMessage(&connData, message);
            nanosleep(&sleepTime,&sleepTimeResult);
        }
    }
    
    return 0;
}


