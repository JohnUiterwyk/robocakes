#include "server.h"

void
server_start(struct configuration * conf)
{
    server_data_t * data;

    data = server_int();

    strcpy(data->tcp_server->port, conf->port);
    sim_init(data->sim_data, conf->clients,conf->balls,conf->width,conf->height);
    data->time_data->interval = (float)(conf->interval)/1000;
    tcp_server_start(data->tcp_server);
    timeloop_start(data->time_data,&server_timer_tick,data);
}

server_data_t* server_int()
{
    server_data_t * server_data;
    server_data = calloc(1, sizeof(server_data_t));
    if(server_data == NULL)
    {
        perror("piss off!");
        exit(EXIT_FAILURE);
    }

    server_data->sim_data = sim_new();
    server_data->tcp_server = tcp_server_data_new();
    server_data->time_data = timeloop_new();
    return server_data;
}

void * server_timer_tick(void * data)
{
    server_data_t * server;
    server = (server_data_t *) data;
    sim_tick(server->sim_data);
    sim_serialize_state(server->sim_data, server->message, MAX_BUFFER_LEN-1);
    //send msg
    thread_copy_to_buffer(server->tcp_server->send_buffer, server->message);

    return NULL;
}
