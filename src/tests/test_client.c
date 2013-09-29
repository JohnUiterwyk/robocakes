#include <stdio.h>
#include <pthread.h>
#include "../SimLib.h"
#include "../TimeLoopLib.h"
#include "../UdpLib.h"

typedef struct {
    TimeLoop_Data * time_data;
    UDP_ConnectionData * conn_data;
    Sim_Data * draw_sim_data;
    Sim_Data * recv_sim_data;
    
    char * recv_buffer;
    char * thread_buffer;
    char * draw_message;
    int thread_buffer_updated;
    
} client_data_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void time_loop_tick(void *);
void * listen_thread_func(void * data);

int main(int argc, const char * argv[])
{
    pthread_t network_thread;
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
    
    client_data->conn_data = UDP_NewConnData();
    client_data->conn_data->socketType = SOCKET_TYPE_LISTEN;
    client_data->conn_data->port = "8000";
    UDP_CreateSocket(client_data->conn_data);
    
    
    
    pthread_create(&network_thread,NULL,consumer,NULL);
    
    client_data->time_data = TimeLoop_Init();
    client_data->time_data->interval = .016;
    TimeLoop_Start(client_data->time_data,&time_loop_tick,client_data);
    return 1;
}


void time_loop_tick(void * data)
{
    client_data_t * client_data;
    client_data = (client_data_t *) data;
    

    printf("%s\n",client_data->draw_message);
    //draw();
}

void * listen_thread_func(void * data)
{
    struct timespec sleepTime, sleepTimeResult;
    client_data_t * client_data = (client_data_t *) data;
    
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 0.001 * 1000000000;
    
    while(1)
    {
        UDP_ReceiveMessage(client_data->conn_data, client_data->recv_buffer);
        /* lock */
        /* copy buffer to thread_buffer */
        /* mark the msg as updated */
        /* unlock */
        /* Sim_UnserializeData(simData, message); */
        
        nanosleep(&sleepTime, &sleepTimeResult);
    }
    
}