#include <stdio.h>
#include "../SimLib.h"
#include "../TimeLoopLib.h"
#include "../udp_lib.h"


typedef struct {
    TimeLoop_Data * time_data;
    udp_conn_data_t * conn_data;
    Sim_Data * sim_data;
    char message[MAX_BUFFER_LEN];
    
} server_data_t;

void * time_loop_tick(void *);

int main(int argc, const char * argv[])
{
    server_data_t * server_data;
    server_data = calloc(1, sizeof(server_data_t));
    if(server_data == NULL)
    {
        perror("piss off!");
        exit(EXIT_FAILURE);
    }
    
    server_data->sim_data = Sim_New();
    Sim_Init(server_data->sim_data, 10,1200,1200);
    
    
    server_data->conn_data = udp_new_conn_data();
    server_data->conn_data->socket_type = SOCKET_TYPE_BROADCAST;
    server_data->conn_data->dest_ip_address = "192.168.1.255";
    server_data->conn_data->port = "8000";
    udp_create_socket(server_data->conn_data);
    
    
    server_data->time_data = TimeLoop_Init();
    server_data->time_data->interval = .033;
    TimeLoop_Start(server_data->time_data,&time_loop_tick,server_data);
    return 1;
}


void * time_loop_tick(void * data)
{
    server_data_t * server_data;
    server_data = (server_data_t *) data;
    Sim_Tick(server_data->sim_data);
    Sim_SerializeState(server_data->sim_data, server_data->message, MAX_BUFFER_LEN-1);
    udp_send_message(server_data->conn_data, server_data->message);
    
    return NULL;
}
