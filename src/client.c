/**
 * @file client.c
 * Hydra client main.
 *
 * On most architectures, it is a dumb client that simply listens to the
 * network over TCP.
 * On a pi, with HAVE_GLES defined, the network information is drawn to
 * the screen using OpenGL
 */

/*
 * Copyright (C) 2013 John Uiterwyk, Virginia King, Stewart Johnston
 */
#include "client.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void client_start(struct configuration * conf)
{
        client_data_t * data;
        pthread_t draw_thread;

        data = client_data_init();
#ifdef HAVE_GLES
        /* swap order of paa */
        bcm_egl_openvg_init(data->display_state);
        display_init(data->display_state, conf->position);
#endif
        data->time_data->interval = (float)(conf->interval)/1000;
        strcpy(data->tcp_client->dest_ip_address, conf->ipaddress);
        strcpy(data->tcp_client->port, conf->port);
        tcp_client_start(data->tcp_client);

        draw_thread_func(data);
        //pthread_create(&draw_thread,NULL,&draw_thread_func,data);
        //pthread_join(draw_thread,NULL);

}

client_data_t * client_data_init()
{
        client_data_t * client_data;
        client_data = calloc(1, sizeof(client_data_t));
        if(client_data == NULL)
        {
                perror("Could not create client data struct!");
                exit(EXIT_FAILURE);
        }

        client_data->tcp_client = tcp_client_data_new();
        client_data->time_data = timeloop_new();
        client_data->draw_message = calloc(MAX_BUFFER_LEN, sizeof(char));
        client_data->sim_data = sim_new();
#ifdef HAVE_GLES
        client_data->display_state = safe_calloc(1,sizeof(display_state_t));
#endif
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


#ifdef HAVE_GLES
                display_draw(client_data->display_state, client_data->sim_data);
#endif
        }

        return NULL;
}
