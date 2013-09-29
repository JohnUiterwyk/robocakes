//
//  SimLib.c
//  simtest
//
//  Created by John Uiterwyk on 9/22/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include "sim_lib.h"


sim_data_t * sim_new()
{
    sim_data_t * simData;
    simData = (sim_data_t*) calloc(1, sizeof(sim_data_t));
    if (simData == NULL)
	{
        perror("calloc encountered an error");
        exit(EXIT_FAILURE);
    }
    return simData;

}
void sim_init(sim_data_t * sim_data,int num_of_objects, int width, int height)
{
    sim_object_t * object;
    int i;

    sim_data->objects = (sim_object_t**) calloc(num_of_objects, sizeof(sim_object_t *));
    if (sim_data->objects == NULL)
	{
        perror("calloc encountered an error");
        exit(EXIT_FAILURE);
    }
    sim_data->size = num_of_objects;
    sim_data->width = width;
    sim_data->height = height;
    sim_data->tick = 0;

    srand((int)time(NULL));

    for(i = 0; i< sim_data->size;i++)
    {
        sim_data->objects[i] = calloc(1,sizeof(sim_object_t));
        object = sim_data->objects[i];
        object->x = sim_random_float(0, sim_data->width);
        object->y = sim_random_float(0, sim_data->height);
        object->dx = sim_random_float(-10, 10);
        object->dy = sim_random_float(-10, 10);
        object->uid = i;
    }
}
void sim_tick(sim_data_t * sim_data)
{
    int i;
    sim_object_t * object;
    for(i = 0; i< sim_data->size; i++)
    {

        object = sim_data->objects[i];
        sim_check_walls(object, sim_data->width, sim_data->height);
        //update position
        object->x += object->dx;
        object->y += object->dy;
    }
    sim_data->tick++;
}

void sim_dispose(sim_data_t * sim_data)
{
    int i;
    for (i = 0; i != sim_data->size ; i++)
    {
        free(sim_data->objects[i]);
    }
    free(sim_data->objects);
    free(sim_data);
}

void sim_check_walls(sim_object_t * object, int width, int height)
{
    // check walls
    if(object->x > width && object->dx > 0)
    {
        object->dx *= -1;
    }
    if(object->y > height && object->dy > 0)
    {
        object->dy *= -1;
    }
    if(object->x < 0 && object->dx < 0)
    {
        object->dx *= -1;
    }
    if(object->y < 0 && object->dy < 0)
    {
        object->dy *= -1;
    }
}


float sim_random_float(float lower, float upper)
{
    int range;
    float randomValue;

    range = upper - lower;

    randomValue = (float) rand();
    randomValue = randomValue / RAND_MAX;
    randomValue = randomValue * range;
    randomValue = randomValue + lower;

    return randomValue;
}
void sim_print_object(sim_object_t * object)
{
    printf("id:%d x:%.2f y:%.2f dx:%.2f dy:%.2f \n",
           object->uid,
           object->x,
           object->y,
           object->dx,
           object->dy);
}

void sim_serialize_state(sim_data_t * sim_data, char * buffer, int max_length)
{
    int i;
    sim_object_t * object;
    char tempString[SIM_OBJECT_STRING_SIZE];
    
    strncpy(buffer, "", max_length - 1);
    sprintf(tempString,"%d,%d,%d;",sim_data->tick,sim_data->width,sim_data->height);
    strncat(buffer, tempString, max_length - strlen(buffer));
    for(i = 0; i< sim_data->size; i++)
    {
        object = sim_data->objects[i];
        sprintf(tempString,"%d,%.2f,%.2f;",
                object->uid,
                object->x,
                object->y);
        if(strlen(buffer) + strlen(tempString) < max_length)
        {
            strncat(buffer, tempString, max_length - strlen(buffer));
        }
    }
}



void sim_deserialize_state(sim_data_t * sim_data, char * message)
{
  int i;
  char * buffer;
  sim_object_t * object;

    i=0;
  buffer = tokenizer_next_udp_message(&message);
  /* Hopefully the first block */
  sscanf(buffer, "%d,%d,%d",&sim_data->tick,
      &sim_data->width, &sim_data->height);

  buffer = tokenizer_next_udp_message(&message);
  while(buffer != NULL) {
    object = sim_data->objects[i];
    sscanf(buffer, "%d,%f,%f", &object->uid, &object->x, &object->y);
    i++;
  }
}









