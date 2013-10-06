#include "client.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void client_start()
{
    client_data_t * data;
    pthread_t draw_thread;

    /* if not on os x, init display */
    /* in future change to ifdef egl */
    #ifndef __APPLE__
    display_state_t * display_state = safe_calloc(1,sizeof(display_state_t));
    /* swap order of paa */
    bcm_egl_openvg_init(display_state);
    display_init(&display_state);
    #endif



    data = client_data_init();
    data->time_data->interval = .033;
    strcpy(data->tcp_client->dest_ip_address,config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP));
    strcpy(data->tcp_client->port,config_get_string(CONF_SERVER_PORT, DEFAULT_SERVER_PORT));
    tcp_client_start(data->tcp_client);

    pthread_create(&draw_thread,NULL,&draw_thread_func,data);
    pthread_join(draw_thread,NULL);

}
client_data_t * client_data_init()
{
    client_data_t * client_data;
    client_data = calloc(1, sizeof(client_data_t));
    if(client_data == NULL)
    {
        perror("piss off!");
        exit(EXIT_FAILURE);
    }

    client_data->tcp_client = tcp_client_data_new();
    client_data->time_data = timeloop_new();
    client_data->draw_message = calloc(MAX_BUFFER_LEN, sizeof(char));
    return client_data;
}

void * draw_thread_func(void * data)
{
    client_data_t * client_data = (client_data_t *) data;
    timeloop_start(client_data->time_data,&client_timer_tick,client_data);

    return NULL;
}


void * client_timer_tick(void * data)
{
    client_data_t * client_data;
    client_data = (client_data_t *) data;
    int redraw = 0;

    redraw = thread_copy_from_buffer(client_data->tcp_client->recv_buffer,
                                     client_data->draw_message);

    if(redraw)
    {
        sim_deserialize_state(client_data->sim_data, client_data->draw_message);
        printf("%s\n",client_data->draw_message);
//
        #ifndef __APPLE__
        display_draw();
        #endif
    }

    return NULL;
}
