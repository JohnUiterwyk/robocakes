#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_lib.h"

#define MAX_BUFFER_LEN 10000

int
main (int argc, char **argv)
{
    sim_data_t * server_sim;
    sim_data_t * client_sim;
    char * buffer;
    int i;
    
    buffer = calloc(MAX_BUFFER_LEN, sizeof(char));
    server_sim = sim_new();
    client_sim = sim_new();
    sim_init(server_sim, 10,1200,1200);
    sim_init(client_sim, 5,1200,1200);
    sim_serialize_state(server_sim, buffer, MAX_BUFFER_LEN);
    printf("%s\n",buffer);
    
    sim_deserialize_state(client_sim, buffer);
    
    printf(" size:%d\n width:%d\n height:%d\n",
           client_sim->size,
           client_sim->width,
           client_sim->height);
    for(i = 0; i < client_sim->size; i++)
    {
        printf("object %d: %.2f %.2f\n",
               i,
               client_sim->objects[i]->x,
               client_sim->objects[i]->y);
    }
    
    
    
    return 0;
}
