//
//  SimLib.c
//  simtest
//
//  Created by John Uiterwyk on 9/22/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include "SimLib.h"


Sim_Data * Sim_New(int numOfObjects, int width, int height)
{
    Sim_Data * simData;
    Sim_Object * object;
    int i;
    simData = (Sim_Data*) malloc(sizeof(Sim_Data));
   
    simData->objects = (Sim_Object**) malloc(numOfObjects * sizeof(Sim_Object *));
    simData->size = numOfObjects;
    simData->width = width;
    simData->height = height;
    
    srand((int)time(NULL));
    
    for(i = 0; i< simData->size;i++)
    {
        simData->objects[i] = malloc(sizeof(Sim_Object));
        object = simData->objects[i];
        object->x = Sim_RandomFloat(0, simData->width);
        object->y = Sim_RandomFloat(0, simData->height);
        object->dx = Sim_RandomFloat(-10, 10);
        object->dy = Sim_RandomFloat(-10, 10);
        object->uid = i;
    }

    return simData;
}
void Sim_Tick(Sim_Data * simData)
{
    int i;
    Sim_Object * object;
    for(i = 0; i< simData->size; i++)
    {
        
        object = simData->objects[i];
        Sim_CheckWalls(object, simData->width, simData->height);
        //update position
        object->x += object->dx;
        object->y += object->dy;
    }
}

void Sim_Dispose(Sim_Data * simData)
{
    int i;
    for (i = 0; i != simData->size ; i++)
    {
        free(simData->objects[i]);
    }
    free(simData->objects);
    free(simData);
}

void Sim_CheckWalls(Sim_Object * object, int width, int height)
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


float Sim_RandomFloat(float lower, float upper)
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
void Sim_PrintObject(Sim_Object * object)
{
    printf("id:%d x:%.2f y:%.2f dx:%.2f dy:%.2f \n",
           object->uid,
           object->x,
           object->y,
           object->dx,
           object->dy);
}














