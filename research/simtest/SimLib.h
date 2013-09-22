//
//  SimLib.h
//  simtest
//
//  Created by John Uiterwyk on 9/22/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#ifndef simtest_SimLib_h
#define simtest_SimLib_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define SIM_OBJECT_STRING_SIZE 15
//100,1000.99,1000.99;
typedef struct
{
    int uid;
    float x;
    float y;
    float dx;
    float dy;
    int size;    
}Sim_Object;


typedef struct
{
    int size;
    int width;
    int height;
    Sim_Object ** objects;
}Sim_Data;

Sim_Data * Sim_New();
void Sim_Init(Sim_Data * simData,int numOfObjects, int width, int height);
void Sim_Tick(Sim_Data * simData);
void Sim_Dispose(Sim_Data * simData);
void Sim_CheckWalls(Sim_Object * object, int width, int height);
float Sim_RandomFloat(float lower, float upper);
void Sim_PrintObject(Sim_Object * object);
void Sim_SerializeState(Sim_Data * simData, char * string, int maxLength);
#endif
