//
//  main.c
//  simtest
//
//  Created by John Uiterwyk on 9/22/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include <stdio.h>
#include "SimLib.h"
#include "UdpLib.h"
#include "InputUtil.h"

int main(int argc, char *argv[])
{
    UDP_ConnectionData connData;
    int numOfObjects,i;
    Sim_Data * simData;
    char message[MAX_BUFFER_LEN];
    
    numOfObjects = 50;
    simData = Sim_New();
    Sim_Init(simData,numOfObjects, 1000, 1000);
    
    
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
            if(strcmp(message,"start")==0)
            {
                
                for(i = 0; i< 1000;i++)
                {
                    Sim_Tick(simData);
                    Sim_SerializeState(simData,message,MAX_BUFFER_LEN);
                    UDP_SendMessage(&connData, message);
                    nanosleep(&sleepTime,&sleepTimeResult);
                }
                printf("sent 1000 messages\n");
            }
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

