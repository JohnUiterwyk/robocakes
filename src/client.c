#include "client.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void client_start()
{
    pthread_t network_thread;
    pthread_t draw_thread;
    client_data_t * client_data;
    
    /* if not on os x, init display */
    /* in future change to ifdef egl */
    #ifndef __APPLE__
      display_state_t * display_state = safe_calloc(1,sizeof(display_state_t)); //swap order of paa
      bcm_egl_openvg_init(display_state);
      init(&display_state);
    #endif
    

    
    client_data = client_data_init();
    client_data->time_data->interval = .033;
    client_data->conn_data->dest_ip_address = config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP);
    client_data->conn_data->port = config_get_string(CONF_SERVER_PORT, DEFAULT_SERVER_PORT);
    
    udp_create_socket(client_data->conn_data);
    
    pthread_create(&network_thread,NULL,&listen_thread_func,client_data);
    pthread_create(&draw_thread,NULL,&draw_thread_func,client_data);
    pthread_join(network_thread,NULL);
    
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
    client_data->recv_buffer = calloc(MAX_BUFFER_LEN,sizeof(char));
    client_data->thread_buffer = calloc(MAX_BUFFER_LEN,sizeof(char));
    client_data->draw_message = calloc(MAX_BUFFER_LEN,sizeof(char));
    client_data->thread_buffer_updated = 0;
    
    client_data->conn_data = udp_new_conn_data();
    client_data->conn_data->socket_type = SOCKET_TYPE_LISTEN;
    
    client_data->time_data = timeloop_new();
    return client_data;
}

void * listen_thread_func(void * data)
{
    struct timespec sleepTime, sleepTimeResult;
    client_data_t * client_data = (client_data_t *) data;
    
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 0.001 * 1000000000;
    
    while(1)
    {
        udp_receive_message(client_data->conn_data, client_data->recv_buffer);
        /* lock */
        pthread_mutex_lock(&mutex);
        /* copy buffer to thread_buffer */
        memcpy(client_data->thread_buffer, client_data->recv_buffer, MAX_BUFFER_LEN * sizeof(char));
        /* mark the msg as updated */
        client_data->thread_buffer_updated = 1;
        /* unlock */
        pthread_mutex_unlock(&mutex);
        
        
        nanosleep(&sleepTime, &sleepTimeResult);
    }
    
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
    
    /* lock */
    pthread_mutex_lock(&mutex);
    /* check if msg modified */
    if(client_data->thread_buffer_updated)
    {
        redraw = 1;
        /* copy thread_buffer to draw_message */
        memcpy(client_data->draw_message, client_data->thread_buffer, MAX_BUFFER_LEN * sizeof(char));
        /* and unmark the msg as updated */
        client_data->thread_buffer_updated = 0;
    }
    /* unlock */
    pthread_mutex_unlock(&mutex);
    
    /* 
     Sim_UnserializeData(simData, message); 
     */
    
    if(redraw)
    {
        #ifdef __APPLE__
        printf("%s\n",client_data->draw_message);
        #endif
            
        #ifndef __APPLE__
        display_draw();
        #endif
    }
    
    return NULL;
}