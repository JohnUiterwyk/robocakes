#include <stdio.h>
#include "../SimLib.h"
#include "../TimeLoopLib.h"
#include "../UdpLib.h"

typedef struct {
    TimeLoop_Data * time_data;
    UDP_ConnectionData * conn_data;
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
    
    
    server_data->conn_data = UDP_NewConnData();
    server_data->conn_data->socketType = SOCKET_TYPE_BROADCAST;
    server_data->conn_data->ipAddress = "192.168.1.255";
    server_data->conn_data->port = "8000";
    UDP_CreateSocket(server_data->conn_data);
    
    
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
    UDP_SendMessage(server_data->conn_data, server_data->message);
    
    
}
