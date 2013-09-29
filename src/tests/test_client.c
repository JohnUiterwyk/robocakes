#include <stdio.h>
#include <pthread.h>
#include "../SimLib.h"
#include "../TimeLoopLib.h"
#include "../udp_lib.h"

typedef struct {
    TimeLoop_Data * time_data;
    udp_conn_data_t * conn_data;
    Sim_Data * draw_sim_data;
    Sim_Data * recv_sim_data;
    
    char * recv_buffer;
    char * thread_buffer;
    char * draw_message;
    int thread_buffer_updated;
    
} client_data_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void * time_loop_tick(void *);
void * listen_thread_func(void * data);
void * draw_thread_func(void * data);

int main(int argc, const char * argv[])
{
    pthread_t network_thread;
    pthread_t draw_thread;
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
    client_data->conn_data->port = "8000";
    udp_create_socket(client_data->conn_data);
    
    
    client_data->time_data = TimeLoop_Init();
    client_data->time_data->interval = .016;
    
    pthread_create(&network_thread,NULL,&listen_thread_func,client_data);
    pthread_create(&draw_thread,NULL,&draw_thread_func,client_data);
    pthread_join(network_thread,NULL);
    
    return 1;
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
    TimeLoop_Start(client_data->time_data,&time_loop_tick,client_data);
    return NULL;
}



void * time_loop_tick(void * data)
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
    
    /* Sim_UnserializeData(simData, message); */
    if(redraw)
    printf("%s\n",client_data->draw_message);
    //draw();
    return NULL;
}