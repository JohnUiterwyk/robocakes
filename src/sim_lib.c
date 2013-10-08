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
    sim_data_t * sim_data;
    sim_data = (sim_data_t*) calloc(1, sizeof(sim_data_t));
    if (sim_data == NULL)
	{
        perror("calloc encountered an error");
        exit(EXIT_FAILURE);
    }
    sim_data->size = 0;
    sim_data->width = 0;
    sim_data->height = 0;
    sim_data->tick = 0;
    sim_data->objects = NULL;

    return sim_data;

}
void sim_init(sim_data_t * sim_data, int clients, int num_of_objects, int width, int height)
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
    sim_data->clients = clients;

    srand((int)time(NULL));

    for(i = 0; i< sim_data->size;i++)
    {
        sim_data->objects[i] = sim_object_new();
        object = sim_data->objects[i];
        object->x = sim_random_float(0, sim_data->width);
        object->y = sim_random_float(0, sim_data->height);
        object->dx = sim_random_float(-5, 5);
        object->dy = sim_random_float(-5, 5);
        object->uid = i;
    }
}
sim_object_t * sim_object_new()
{
    sim_object_t * object;
    object = calloc(1,sizeof(sim_object_t));
    if (object == NULL)
	{
        perror("calloc encountered an error");
        exit(EXIT_FAILURE);
    }
    object->x = 0;
    object->y = 0;
    object->dx = 0;
    object->dy = 0;
    object->uid = 0;
    return object;
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
    sprintf(tempString,"{%d,%d,%d,%d,%d;",
            sim_data->tick,
            sim_data->clients,
            sim_data->width,
            sim_data->height,
            sim_data->size);
    
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
    
    //overwrite the last ; with a }
    buffer[strlen(buffer)-1] = '}';
}



void sim_deserialize_state(sim_data_t * sim_data, char * message)
{
    char * token;
    char * saveptr1;
    int num_objs_prev,i,result;
    
    
    
    if(message[0] != '{' || message[strlen(message)-1] != '}' || sim_data == NULL)
    {
        //open and closing brackets not found so message is not formatted right
        return;
    }
    
    //save the current size, then grab the msg header data
    num_objs_prev = sim_data->size;
     token = strtok_r(message, ";", &saveptr1);
    if(token != NULL)
    {
        result = sscanf(token, "{%d,%d,%d,%d,%d",
                        &sim_data->tick,
                        &sim_data->clients,
                        &sim_data->width,
                        &sim_data->height,
                        &sim_data->size);
        if(result != 5)
            printf("mismatch on header values.\n");
    }
    
    //sort out the size of the object array
    if(sim_data->objects == NULL)
    {
        //no array, so initiate the objects array
        sim_data->objects = (sim_object_t**) calloc(sim_data->size, sizeof(sim_object_t *));
        if (sim_data->objects == NULL)
        {
            perror("calloc encountered an error");
            exit(EXIT_FAILURE);
        }
        for(i = 0; i<sim_data->size;i++)
        {
            sim_data->objects[i] = sim_object_new();
        }
    }
    else
    {
        
        if(sim_data->size < num_objs_prev)
        {
            //if we are shrinking, free the extra space
            for(i = sim_data->size; i<num_objs_prev;i++)
            {
                free(sim_data->objects[i]);
            }
        }
        sim_data->objects = realloc(sim_data->objects,
                                    sizeof(sim_object_t *) * sim_data->size);
        if(sim_data->size > num_objs_prev)
        {
        
            for(i = num_objs_prev; i<sim_data->size;i++)
            {
                sim_data->objects[i] = sim_object_new();
            }
        }
    }
    
    token = strtok_r(NULL, ";", &saveptr1);
    i=0;
    //we assume the size isn't lying
    while(token != NULL && i < sim_data->size)
    {
        result = sscanf(token, "%d,%f,%f",
                        &sim_data->objects[i]->uid,
                        &sim_data->objects[i]->x,
                        &sim_data->objects[i]->y);
        token = strtok_r(NULL, ";", &saveptr1);
        i++;
    }
    printf("parsed %d object \n",i);
}









