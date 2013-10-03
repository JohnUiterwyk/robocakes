//
//  main.c
//  TcpTest
//
//  Created by John Uiterwyk on 10/3/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//
#include "main.h"

int main(int argc, const char * argv[])
{
    
    pthread_t server_listen_thread;
    pthread_t server_input_thread;
    
    pthread_t client_read_thread;
    pthread_t client_write_thread;
    pthread_t client_input_thread;
    
    tcp_conn_data_t * conn_data;
    
    conn_data = tcp_new_conn_data();
    strcpy(conn_data->dest_ip_address,"localhost");
    strcpy(conn_data->port,"8675");
    
    if(argc>1 && strcmp(argv[1],"server") == 0)
    {
        conn_data->socket_type = TCP_SERVER;
        tcp_create_server(conn_data);
        pthread_create(&server_listen_thread, NULL, &server_listen_thread_func, conn_data);
        pthread_create(&server_input_thread, NULL, &input_thread_func, conn_data);
        pthread_join(server_input_thread,NULL);
        //now create threads to test,
        //maybe added threads to conn data (one for send one for receive);
    }else
    {
        conn_data->socket_type = TCP_CLIENT;
        tcp_create_client(conn_data);
        pthread_create(&client_read_thread, NULL, &client_read_thread_func, conn_data);
        pthread_create(&client_write_thread, NULL, &client_write_thread_func, conn_data);
        pthread_create(&client_input_thread, NULL, &input_thread_func, conn_data);
        
        pthread_join(client_input_thread,NULL);
        
    }
    return 0;
}

void * server_listen_thread_func(void * data)
{
    tcp_conn_data_t * conn_data;
    conn_data =  (tcp_conn_data_t *)data;
    tcp_start_server(conn_data);
    return NULL;
    
}
void * input_thread_func(void * data)
{
    tcp_conn_data_t * conn_data;
    conn_data =  (tcp_conn_data_t *)data;
    char message[MAX_BUFFER_LEN];
    
    while(strcmp(message,"exit") != 0)
    {
        getString(message,MAX_BUFFER_LEN,":::");
        thread_copy_to_buffer(conn_data->send_buffer, message);
    }
    printf("hmm");
    return NULL;
    
}
void * client_read_thread_func(void * data)
{
    tcp_conn_data_t * conn_data;
    conn_data =  (tcp_conn_data_t *)data;
    tcp_start_client_listener(conn_data);
    return NULL;
    
}
void * client_write_thread_func(void * data)
{
    tcp_conn_data_t * conn_data;
    conn_data =  (tcp_conn_data_t *)data;
    tcp_start_client_sender(conn_data);
    return NULL;
    
}