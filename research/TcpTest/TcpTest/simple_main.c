#include "tcp_lib.h"
#include "thread_lib.h"
#include "InputUtil.h"

void * client_thread_func(void * data)
{
    tcp_conn_data_t * client_conn_data;
    client_conn_data = (tcp_conn_data_t *)data;
    strcpy(client_conn_data->dest_ip_address,"localhost");
    strcpy(client_conn_data->port,"8675");
    tcp_create_client(client_conn_data);
    tcp_start_client_sender(client_conn_data);
}

void * server_thread_func(void * data)
{
    tcp_conn_data_t * server_conn_data;
    
    server_conn_data = (tcp_conn_data_t *)data;
    strcpy(server_conn_data->dest_ip_address,"localhost");
    strcpy(server_conn_data->port,"8675");
    server_conn_data->socket_type = TCP_SERVER;
    tcp_create_server(server_conn_data);
    tcp_start_server(server_conn_data);
}
int main(void)
{
    
    tcp_conn_data_t * server_conn_data;
    tcp_conn_data_t * client_conn_data;
    int i;
    char message[MAX_BUFFER_LEN];
    
    i= 0;
    
    client_conn_data = tcp_new_conn_data();
    server_conn_data = tcp_new_conn_data();
    
    pthread_t client_thread,server_thread;
    pthread_create(&client_thread, NULL, client_thread_func, client_conn_data);
    pthread_create(&server_thread, NULL, server_thread_func, server_conn_data);
    while(1)
    {
        sleep(1);
        sprintf(message, "client sent hello %d\n",i);
        thread_copy_to_buffer(client_conn_data->send_buffer, message);
        
        sprintf(message, "server sent world %d\n",i);
        thread_copy_to_buffer(server_conn_data->send_buffer, message);
        i++;
    }
    
    
    
}

