//
//  SimLib.c
//  simtest
//
//  Created by John Uiterwyk on 9/22/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include "SimLib.h"


Sim_Data * Sim_New()
{
    Sim_Data * simData;
    simData = (Sim_Data*) calloc(1, sizeof(Sim_Data));
    if (simData == NULL)
	{
        perror("calloc encountered an error");
        exit(EXIT_FAILURE);
    }
    return simData;

}
void Sim_Init(Sim_Data * simData,int numOfObjects, int width, int height)
{
    Sim_Object * object;
    int i;

    simData->objects = (Sim_Object**) calloc(numOfObjects, sizeof(Sim_Object *));
    if (simData->objects == NULL)
	{
        perror("calloc encountered an error");
        exit(EXIT_FAILURE);
    }
    simData->size = numOfObjects;
    simData->width = width;
    simData->height = height;
    simData->tick = 0;

    srand((int)time(NULL));

    for(i = 0; i< simData->size;i++)
    {
        simData->objects[i] = calloc(1,sizeof(Sim_Object));
        object = simData->objects[i];
        object->x = Sim_RandomFloat(0, simData->width);
        object->y = Sim_RandomFloat(0, simData->height);
        object->dx = Sim_RandomFloat(-10, 10);
        object->dy = Sim_RandomFloat(-10, 10);
        object->uid = i;
    }
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
    simData->tick++;
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

void Sim_SerializeState(Sim_Data * simData, char * buffer, int maxLength)
{
    int i;
    Sim_Object * object;
    char tempString[SIM_OBJECT_STRING_SIZE];
    
    strncpy(buffer, "", maxLength - 1);
    sprintf(tempString,"%d,%d,%d;",simData->tick,simData->width,simData->height);
    strncat(buffer, tempString, maxLength - strlen(buffer));
    for(i = 0; i< simData->size; i++)
    {
        object = simData->objects[i];
        sprintf(tempString,"%d,%.2f,%.2f;",
                object->uid,
                object->x,
                object->y);
        if(strlen(buffer) + strlen(tempString) < maxLength)
        {
            strncat(buffer, tempString, maxLength - strlen(buffer));
        }
    }
}



void Sim_UnserializeData(Sim_Data * simData, char * message)
{
  int objectsReceived, i;
  char * buffer;
  Sim_Object * object;
  GError *error = NULL;

  buffer = tokenizer_next_udp_message(&message, &error);
  /* Hopefully the first block */
  sscanf(buffer, "%d,%d,%d",simData->tick,
      simData->width, simData->height);

  buffer = tokenizer_next_udp_message(&message, &error);
  while(buffer != NULL) {
    object = simData->objects[i];
    sscanf(buffer, "%d,%.2f,%.2f", object->uid, object->x, object->y);
    i++;
  }
}









