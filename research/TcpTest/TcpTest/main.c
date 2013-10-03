//
//  main.c
//  TcpTest
//
//  Created by John Uiterwyk on 10/3/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//
#include "main.h"

void * output_thread_func (void * data)
{
    shared_buffer_t * shared;
    shared = (shared_buffer_t *) data;
    struct timespec sleepTime, sleepTimeResult;
    char message[MAX_BUFFER_LEN];
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 0.001 * 1000000000;
    while(1)
    {
        if(thread_copy_from_buffer(shared, message)==1)
        {
            printf("recvied: %s\n",message);
        }
        nanosleep(&sleepTime, &sleepTimeResult);
    }
}

int main(int argc, const char * argv[])
{
    
    char message[MAX_BUFFER_LEN];
    tcp_server_data_t * server;
    tcp_client_data_t * client;
    pthread_t output_thread;
    
    if(argc>1 && strcmp(argv[1],"server") == 0)
    {
        
        server = tcp_server_data_new();
        strcpy(server->port,"8675");
        pthread_create(&output_thread,NULL,&output_thread_func,server->recv_buffer);
        tcp_server_start(server);
        while(strcmp(message,"exit") != 0)
        {
            getString(message,MAX_BUFFER_LEN,":::");
            thread_copy_to_buffer(server->send_buffer, message);
        }
        tcp_server_stop(server);
    }else
    {
        client = tcp_client_data_new();
        strcpy(client->dest_ip_address,"localhost");
        strcpy(client->port,"8675");

        pthread_create(&output_thread,NULL,&output_thread_func,client->recv_buffer);
        
        tcp_client_start(client);
        while(strcmp(message,"exit") != 0)
        {
            getString(message,MAX_BUFFER_LEN,":::");
            thread_copy_to_buffer(client->send_buffer, message);
        }
        tcp_client_stop(client);

    }
    pthread_join(output_thread,NULL);
    return 0;
}