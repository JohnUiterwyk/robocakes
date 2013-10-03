#include "tcp_client_lib.h"
#include "tcp_server_lib.h"
#include "thread_lib.h"
#include "InputUtil.h"

int main(void)
{
    
    tcp_server_data_t * server;
    tcp_client_data_t * client;
    int i;
    char message[MAX_BUFFER_LEN];
    
    i= 0;
    
    server = tcp_server_data_new();
    strcpy(server->port,"8675");
    
    client = tcp_client_data_new();
    strcpy(client->dest_ip_address,"localhost");
    strcpy(client->port,"8675");
    
    tcp_server_start(server);
    tcp_client_start(client);

    
    while(i<20)
    {
        sleep(1);
        sprintf(message, "ack from client %d\n",i);
        thread_copy_to_buffer(client->send_buffer, message);
        
        sprintf(message, "boo form server %d\n",i);
        thread_copy_to_buffer(server->send_buffer, message);
        i++;
    }
    
    printf("shutting down! this isnt pretty yet, but its ok :) \n");
    tcp_client_stop(client);
    tcp_client_data_free(client);
    
    tcp_server_stop(server);
    tcp_server_data_free(server);
    
    printf("goodbye!\n");
    return 0;
    
}

