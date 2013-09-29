//
//  SimLib.h
//  simtest
//
//  Created by John Uiterwyk on 9/22/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#ifndef _SIM_LIB_H
#define _SIM_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "tokenizer.h"

#define MAX_BUF_SIZE 255

#define SIM_OBJECT_STRING_SIZE 20
//100,1000.99,1000.99;
typedef struct
{
    int uid;
    float x;
    float y;
    float dx;
    float dy;
    int size;
}sim_object_t;


typedef struct
{
    int size;
    int width;
    int height;
    int tick;
    sim_object_t ** objects;
}sim_data_t;

sim_data_t * sim_new();
void sim_init(sim_data_t * sim_data,int num_of_objects, int width, int height);
void sim_tick(sim_data_t * sim_data);
void sim_dispose(sim_data_t * sim_data);
void sim_check_walls(sim_object_t * object, int width, int height);
float sim_random_float(float lower, float upper);
void sim_print_object(sim_object_t * object);
void sim_serialize_state(sim_data_t * sim_data, char * string, int max_length);
void sim_deserialize_state(sim_data_t * sim_data, char * message);
#endif
