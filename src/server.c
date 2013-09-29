#include "server.h"

void
server_start()
{
    server_data_t * server_data;
    server_data = calloc(1, sizeof(server_data_t));
    if(server_data == NULL)
    {
        perror("piss off!");
        exit(EXIT_FAILURE);
    }
    
    server_data->sim_data = sim_new();
    sim_init(server_data->sim_data, 10,1200,1200);
    
    
    server_data->conn_data = udp_new_conn_data();
    server_data->conn_data->socket_type = SOCKET_TYPE_BROADCAST;
    server_data->conn_data->dest_ip_address = config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP);
    server_data->conn_data->port = config_get_string(CONF_SERVER_PORT, DEFAULT_SERVER_PORT);
    
    udp_create_socket(server_data->conn_data);
    
    
    server_data->time_data = timeloop_new();
    server_data->time_data->interval = .033;
    timeloop_start(server_data->time_data,&server_timer_tick,server_data);
}


void * server_timer_tick(void * data)
{
    server_data_t * server_data;
    server_data = (server_data_t *) data;
    sim_tick(server_data->sim_data);
    sim_serialize_state(server_data->sim_data, server_data->message, MAX_BUFFER_LEN-1);
    udp_send_message(server_data->conn_data, server_data->message);
    
    return NULL;
}
